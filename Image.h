#ifndef IMAGE
#define IMAGE

#include <vector>
#include <string>
#include <png.h>
#include <Magick++.h>

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

    virtual void read(unsigned char * buffer,size_t size) = 0;
    virtual void writeToMemory() = 0;
    virtual void writeToFile(const std::string &filename) = 0;

    virtual size_t getImageSize() = 0;
    virtual const void * getImageMemory() = 0;
};

class PngImage : public Image{
    size_t _pos = 0;
    std::vector<unsigned char> _buffer;

    Magick::Blob _blob;
    public:
    void createBuffer() override;
    void read(unsigned char * buffer,size_t size) override;
    void writeToMemory() override;
    void writeToFile(const std::string &filepath) override;
    size_t getImageSize() override;
    const void * getImageMemory() override;
};

#endif /*IMAGE*/