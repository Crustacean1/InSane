SOURCES := $(wildcard *.cpp) $(wildcard */*.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := $(shell pwd | xargs basename)
CXX = g++
CXXFLAGS = -g -Wall -Werror -std=c++20 -MMD -I ./ -I/home/kamil2/Libraries/uWebSockets/src -I/home/kamil2/Libraries/uSockets/src -I/home/kamil2/Libraries/backends/include -I/home/kamil2/Libraries/json/include
override LDFLAGS += /home/kamil2/Libraries/uSockets/*.o -lz  -lsane
DEPENDS = ${OBJECTS:.o=.d} main.d

${TARGET}: ${OBJECTS}
	g++ -g -std=c++20 ${LDFLAGS} ${OBJECTS}  -o ${TARGET} 

-include ${DEPENDS}

clean:
	rm -f *.o
	rm -f */*.o
	rm -f *.d
	rm -f */*.d
