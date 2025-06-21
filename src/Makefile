# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./networking

TARGET = main
SRC = main.cpp networking/connection.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
