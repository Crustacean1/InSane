#include "Image.h"
#include <png.h>
#include <cstring>
#include <iostream>

/*Image::Image() {
    _image = new Magick::Image(width,height, channels ==3  ? "RGB" :"K",Magick::CharPixel,static_cast<const void*>(raw));
    delete[] raw;
}
void Image::save(std::string filename){
    _image->write(filename);
}
Image::~Image(){
    delete _image;
}*/
size_t Image::getPixelCount(){return _width*_height*_channelCount;}
void Image::setWidth(size_t width){_width = width;}
void Image::setHeight(size_t height){_height = height;}
void Image::setChannelCount(size_t channelCount){_channelCount = channelCount;}

void PngImage::createBuffer(){
    if(_width&&_height&&_channelCount){
        std::cout<<"creating buffer of size: "<<_buffer.size()<<" "<<_width<<"\t"<<_height<<"\t"<<_channelCount<<std::endl;
        _buffer.reserve(_width*_height*_channelCount);
    }
}

void PngImage::read(unsigned char * buffer,size_t size){
    _buffer.insert(_buffer.end(),buffer,buffer +size);
}
void PngImage::writeToFile(std::string filename){
    if(_height == 0){
        _height = _pos/_width;
    }
    std::cout<<"writing buffer of size: "<<_buffer.size()<<std::endl;
    std::cout<<_width<<"\t"<<_height<<"\t"<<_channelCount<<std::endl;
    Magick::Image outputImage(_width,_height,_channelCount == 3 ? "RGB" : "K",Magick::CharPixel,static_cast<const void*>(_buffer.data()));
    outputImage.write(filename);
}

void PngImage::writeToMemory(){
    Magick::Image outputImage(_width,_height,_channelCount == 3 ? "RGB" : "K",Magick::CharPixel,static_cast<const void*>(_buffer.data()));
    outputImage.write(&_blob);
}

size_t PngImage::getImageSize(){
    return _blob.length(); 
}
const void * PngImage::getImageMemory(){
    return _blob.data();
}