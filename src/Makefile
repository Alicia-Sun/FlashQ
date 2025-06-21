# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./networking -I./nodes

TARGET = main
SRC = main.cpp networking/connection.cpp nodes/consumer.cpp nodes/producer.cpp nodes/queue.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
