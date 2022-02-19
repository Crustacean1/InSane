#include "Image.h"
#include <cstring>
#include <iostream>
#include <png.h>

void Image::setWidth(size_t width) { _width = width; }
void Image::setHeight(size_t height) { _height = height; }
void Image::setChannelCount(size_t channelCount) {
  _channelCount = channelCount;
}
void Image::setDepth(size_t depth) { _depth = depth; }

size_t PngImage::PngColorMappings[] = {0, PNG_COLOR_TYPE_GRAY, 0,
                                       PNG_COLOR_TYPE_RGB, PNG_COLOR_TYPE_RGBA};

void PngImage::createBuffer() {
  if (_width && _height && _channelCount) {
    std::cout << "creating buffer of size: " << _buffer.size() << " " << _width
              << "\t" << _height << "\t" << _channelCount << "\t" << _depth
              << std::endl;
    _buffer.reserve(_width * _height * _channelCount);

    _pngPtr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING, (png_voidp)_png_error_buffer,
        PngImage::pngErrorHandler, PngImage::pngWarningHandler);

    _pngInfoPtr = png_create_info_struct(_pngPtr);

    if (_pngInfoPtr == NULL) {
      throw std::runtime_error(
          "In PngImage::createBuffer: Failed to initalize png image");
    }

    png_set_IHDR(_pngPtr, _pngInfoPtr, _width, _height, _depth,
                 PngColorMappings[_channelCount], PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    _rows = static_cast<unsigned char **>(
        png_malloc(_pngPtr, _height * sizeof(png_byte *)));
    size_t bytesInLine = _width * _channelCount;
    for (size_t i = 0; i < _height; ++i) {
      _rows[i] = _buffer.data() + i * bytesInLine;
    }
  }
}

void PngImage::flushBuffer() { png_set_rows(_pngPtr, _pngInfoPtr, _rows); }

void PngImage::read(unsigned char *buffer, size_t size) {
  _buffer.insert(_buffer.end(), buffer, buffer + size);
}

void PngImage::writeToBuffer(ImageBuffer &buffer) {
  buffer.reserve(1024);
  png_set_write_fn(_pngPtr, &buffer, PngImage::pngWriteToBuffer,
                   PngImage::pngFlush);
  png_write_png(_pngPtr, _pngInfoPtr, PNG_TRANSFORM_IDENTITY, NULL);
}

void PngImage::writeToFile(FILE *file) {
  png_init_io(_pngPtr, file);
  png_set_rows(_pngPtr, _pngInfoPtr, _rows);
  png_write_png(_pngPtr, _pngInfoPtr, PNG_TRANSFORM_IDENTITY, NULL);
}

size_t PngImage::getImageSize() { return _width * _height * _channelCount; }

PngImage::~PngImage() {
  if (_rows != nullptr) {
    png_free(_pngPtr, _rows);
  }
  png_destroy_write_struct(&_pngPtr, &_pngInfoPtr);
}

void PngImage::pngWarningHandler(png_struct *png_ptr, png_const_charp details) {
  std::cerr << "Warning while processing png image: " << details << std::endl;
}

void PngImage::pngErrorHandler(png_struct *png_ptr, png_const_charp details) {
  std::cerr << "Failed while processing png image: " << details << std::endl;
}

void PngImage::pngFlush(png_struct *pngPng) {}

Image::ImageBuffer::ImageBuffer()
    : _data(new unsigned char[1]), _position(0), _size(1) {}
PngImage::ImageBuffer::~ImageBuffer() { delete[] _data; }

void Image::ImageBuffer::reserve(size_t length) {
  size_t newSize = _size;
  while (newSize < length) {
    newSize <<= 1;
  }
  auto newData = new unsigned char[newSize];
  memcpy(newData, _data, _size);
  delete[] _data;
  _size = newSize;
  _data = newData;
}

void Image::ImageBuffer::append(unsigned char *ptr, size_t length) {
  memcpy(_data + _position, ptr, length);
  _position += length;
}

size_t Image::ImageBuffer::size() { return _size; }
size_t Image::ImageBuffer::pos() { return _position; }
const unsigned char *Image::ImageBuffer::data() { return _data; }

void PngImage::pngWriteToBuffer(png_struct *pngPtr, unsigned char *data,
                                png_size_t length) {
  ImageBuffer *pngBuffer = static_cast<ImageBuffer *>(png_get_io_ptr(pngPtr));

  pngBuffer->reserve(pngBuffer->pos() + length);
  pngBuffer->append(data, length);
}
