SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := $(shell pwd | xargs basename)
IDIRS = -I/home/kamil2/Libraries/uWebSockets/src\
 -I/home/kamil2/Libraries/uSockets/src\
 -I/home/kamil2/Libraries/backends/include\
 -I/home/kamil2/Libraries/json/include\
 -I/home/kamil2/Libraries/ImageMagick\
 -I/home/kamil2/Libraries/ImageMagick/Magick++/lib\
 -I/home/kamil2/Libraries/jsoncpp/build/include
CXX = g++
override LDFLAGS += /home/kamil2/Libraries/uSockets/*.o -lsane -lz -L/home/kamil2/Libraries/ImageMagic/Magick++/lib/.libs/ -lMagick++-7.Q16HDRI -lpthread
override LDFLAGS += -L/home/kamil2/Libraries/jsoncpp/build/lib -ljsoncpp
CXXFLAGS = -g -Wall  -std=c++20 -MMD ${IDIRS} -DMAGICKCORE_QUANTUM_DEPTH=16 -DMAGICKCORE_HDRI_ENABLE=1

DEPENDS = ${OBJECTS:.o=.d} main.d

${TARGET}: ${OBJECTS}
	g++ -g -std=c++20 ${LDFLAGS} ${OBJECTS}  -o ${TARGET} 

-include ${DEPENDS}

clean:
	rm -f *.o
	rm -f */*.o
	rm -f */*/*.o
	rm -f *.d
	rm -f */*.d
	rm -f */*/*.d
