CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET = Arfetch
SRC = arfetch.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

.PHONY: all
