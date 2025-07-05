# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./networking -I./nodes -I./utils

TARGET = main
SRC = main.cpp networking/connection.cpp nodes/queue.cpp utils/helpers.cpp utils/message.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
