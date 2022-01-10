#include "Image.h"
#include <png.h>
#include <cstring>
#include <Magick++.h>
#include <iostream>

Image::Image(unsigned char * raw,size_t width,size_t height, size_t channels) {
    _image = new Magick::Image(width,height, channels ==3  ? "RGB" :"K",Magick::CharPixel,static_cast<const void*>(raw));
    delete[] raw;
}
void Image::save(std::string filename){
    _image->write(filename);
}
Image::~Image(){
    delete _image;
}