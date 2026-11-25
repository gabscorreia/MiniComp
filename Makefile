CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = mincomp

SOURCES = src/main.cpp src/lexer.cpp src/ast.cpp src/parser.cpp
HEADERS = include/lexer.h include/ast.h include/parser.h

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe

test: $(TARGET)
	@status=0; \
	for f in tests/valid/*.min; do \
		./$(TARGET).exe "$$f" > /dev/null || { echo "FALHOU (válido): $$f"; status=1; }; \
	done; \
	for f in tests/invalid/*.min; do \
		./$(TARGET).exe "$$f" > /dev/null && { echo "FALHOU (inválido): $$f"; status=1; }; \
	done; \
	if [ $$status -eq 0 ]; then echo "Todos os testes passaram."; fi; \
	exit $$status
