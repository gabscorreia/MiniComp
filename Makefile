CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = mincomp
SOURCES = src/main.cpp src/lexer.cpp

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) include/lexer.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe