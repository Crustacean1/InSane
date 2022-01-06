#ifndef IMAGE
#define IMAGE

#include <vector>
#include <string>
#include <png.h>

namespace Magick{
    class Image;
};

class Image{
    Magick::Image * _image;
    public:
    Image(unsigned char * pixels,size_t width,size_t height,size_t channels);
    void save(std::string filepath);
    ~Image();
};

#endif /*IMAGE*/