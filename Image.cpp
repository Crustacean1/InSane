#include "Image.h"
#include <png.h>
#include <cstring>
#include <Magick++.h>
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
        _buffer.reserve(_width*_height*_channelCount);
        std::cout<<"created buffer of size: "<<_buffer.size()<<" "<<_width<<"\t"<<_height<<"\t"<<_channelCount<<std::endl;
    }
}

void PngImage::write(unsigned char * buffer,size_t size){
    _buffer.insert(_buffer.end(),buffer,buffer +size);
}
void PngImage::save(std::string filename){
    if(_height == 0){
        _height = _pos/_width;
    }
    std::cout<<"writing buffer of size: "<<_buffer.size()<<std::endl;
    std::cout<<_width<<"\t"<<_height<<"\t"<<_channelCount<<std::endl;
    Magick::Image outputImage(_width,_height,_channelCount == 3 ? "RGB" : "K",Magick::CharPixel,static_cast<const void*>(_buffer.data()));
    outputImage.write(filename);
}