#ifndef IMAGE
#define IMAGE

#include <vector>
#include <string>
#include <png.h>

class Image{
    protected:
    size_t _width = 0;
    size_t _height = 0;
    size_t _channelCount = 0;
    public:
    virtual void setWidth(size_t width);
    virtual void setHeight(size_t height);
    virtual size_t getPixelCount();
    virtual void setChannelCount(size_t channelCount);
    virtual void createBuffer() = 0;
    virtual void write(unsigned char * buffer, size_t size) = 0;
    virtual void save(std::string filename) = 0;
};

class PngImage : public Image{
    size_t _pos = 0;
    std::vector<unsigned char> _buffer;
    public:
    void createBuffer() override;
    void write(unsigned char * buffer,size_t size) override;
    void save(std::string filepath) override;
};

#endif /*IMAGE*/