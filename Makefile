CXX := g++
CXXFLAGS := -std=c++11 -O3 -ggdb

SRC := src
BIN := bin
LIBS := `pkg-config --libs --cflags opencv4`

CXXFLAGS += $(LIBS)
SRCFILES = $(shell find $(SRC) -name *.h) $(shell find $(SRC) -name *.cpp)

all: bin/OpMode


bin/OpMode: setup bin/OpMode.o
	$(CXX) $(CXXFLAGS) bin/OpMode.o -o bin/OpMode
	rm bin/OpMode.o

bin/OpMode.o: $(SRCFILES)
	$(CXX) $(CXXFLAGS) src/OpMode.cpp -c -o bin/OpMode.o

setup:
	mkdir -p $(BIN)

clean:
	rm -rf $(BIN)