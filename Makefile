CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude
TARGET = mincomp

SOURCES = src/main.cpp src/lexer.cpp src/ast.cpp src/parser.cpp src/symbol_table.cpp src/interpreter.cpp
HEADERS = include/lexer.h include/ast.h include/parser.h include/symbol_table.h include/interpreter.h

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe tests/test_symbol_table.exe tests/test_interpreter.exe

test: $(TARGET)
	@g++ $(CXXFLAGS) tests/test_symbol_table.cpp src/symbol_table.cpp -o tests/test_symbol_table.exe
	@./tests/test_symbol_table.exe
	@g++ $(CXXFLAGS) tests/test_interpreter.cpp src/lexer.cpp src/parser.cpp src/ast.cpp src/symbol_table.cpp src/interpreter.cpp -o tests/test_interpreter.exe
	@./tests/test_interpreter.exe
	@status=0; \
	for f in tests/valid/*.min; do \
		./$(TARGET).exe --check "$$f" > /dev/null || { echo "FALHOU (válido): $$f"; status=1; }; \
	done; \
	for f in tests/invalid/*.min; do \
		./$(TARGET).exe --check "$$f" > /dev/null 2>&1 && { echo "FALHOU (inválido): $$f"; status=1; }; \
	done; \
	for f in tests/semantic/*.min; do \
		case "$$f" in \
			*entrada_negativa.min) \
				if printf '%s\n' '-5' | ./$(TARGET).exe "$$f" > /dev/null 2>&1; then \
					echo "FALHOU (semântico): $$f"; status=1; \
				fi ;; \
			*) \
				if ./$(TARGET).exe "$$f" > /dev/null 2>&1; then \
					echo "FALHOU (semântico): $$f"; status=1; \
				fi ;; \
		esac; \
	done; \
	if [ $$status -eq 0 ]; then echo "Todos os testes passaram."; fi; \
	exit $$status
