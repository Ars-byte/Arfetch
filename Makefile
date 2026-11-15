CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET = Arfetch
SRC = arfetch.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

static: $(SRC)
	$(CXX) -std=c++17 -O3 -static $(SRC) -o $(TARGET)
	strip $(TARGET)

.PHONY: all static
