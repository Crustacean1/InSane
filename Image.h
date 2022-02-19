#ifndef IMAGE
#define IMAGE

#include <png.h>
#include <string>
#include <vector>

class Image {
protected:
  size_t _width = 0;
  size_t _height = 0;
  size_t _channelCount = 1;
  size_t _depth = 8;

public:
  class ImageBuffer {
  private:
    unsigned char * _data;
    size_t _position;
    size_t _size;

  public:
    ImageBuffer();
    ImageBuffer(const ImageBuffer&) = delete;
    ImageBuffer(ImageBuffer&&) = delete;
    ImageBuffer& operator=(const ImageBuffer&) = delete;

    size_t size();
    size_t pos();
    const unsigned char* data();
    void reserve(size_t length);
    void append(unsigned char *ptr, size_t length);
    ~ImageBuffer();
  };

  virtual void setWidth(size_t width);
  virtual void setHeight(size_t height);
  virtual void setDepth(size_t depth);
  virtual void setChannelCount(size_t channelCount);

  virtual void createBuffer() = 0;
  virtual void read(unsigned char *buffer, size_t size) = 0;
  virtual void flushBuffer() = 0;

  virtual void writeToBuffer(ImageBuffer & ptr) = 0;
  virtual void writeToFile(FILE *file) = 0;

  virtual size_t getImageSize() = 0;
};

class PngImage : public Image {

public:

private:
  size_t _pos = 0;
  std::vector<unsigned char> _buffer;
  png_struct *_pngPtr = nullptr;
  png_info *_pngInfoPtr = nullptr;
  unsigned char **_rows = nullptr;

  static size_t PngColorMappings[];

  void *_png_error_buffer;

  static void pngErrorHandler(png_struct *png_ptr, png_const_charp details);
  static void pngWarningHandler(png_struct *png_ptr, png_const_charp details);

  static void pngWriteToBuffer(png_struct *pngPtr, unsigned char *data,
                               png_size_t length);
  static void pngFlush(png_struct *pngPtr);

public:
  void createBuffer() override;
  void read(unsigned char *buffer, size_t size) override;
  void flushBuffer() override;

  void writeToBuffer(ImageBuffer &buffer) override;
  void writeToFile(FILE *file) override;

  size_t getImageSize() override;

  ~PngImage();
};

#endif /*IMAGE*/