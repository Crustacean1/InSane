SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp) $(wildcard */*/*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := $(shell pwd | xargs basename)
IDIRS = -I/home/kamil2/Libraries/backends/include\
 -I/home/kamil2/Libraries/jsoncpp/build/include
CXX = g++
override LDFLAGS += -lsane -lz -lpthread
override LDFLAGS += -L/home/kamil2/Libraries/jsoncpp/build/lib -ljsoncpp -lpng
CXXFLAGS = -g -Wall  -std=c++20 -MMD ${IDIRS}

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
