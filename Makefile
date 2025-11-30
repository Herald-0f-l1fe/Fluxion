# Флаги для WSL (Windows Subsystem for Linux)
CXX = g++
CXXFLAGS = -D_DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -I/usr/include/ \
           -Weffc++ -Wcast-align -Wcast-qual -Wchar-subscripts \
           -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
           -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline \
           -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual \
           -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
           -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 \
           -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
           -Wswitch-default -Wswitch-enum -Wundef -Wunreachable-code \
           -Wunused -Wuseless-cast -Wvariadic-macros \
           -Wno-missing-field-initializers -Wno-narrowing \
           -Wno-old-style-cast -Wstack-protector -fcheck-new \
           -fsized-deallocation -fstack-protector -fstrict-overflow \
           -fno-omit-frame-pointer -pie -fPIE -Werror=vla \
           -fsanitize=address,undefined,leak,bounds \
           -fsanitize-address-use-after-scope \
           -fno-optimize-sibling-calls

INCLUDE_DIRS = -IHeaders -Icommon
# Расширенные флаги для линковки с AddressSanitizer
LDFLAGS = -pie -fPIE \
          -fsanitize=address,undefined,leak,bounds \
          -fsanitize-address-use-after-scope \
          -fno-optimize-sibling-calls

TARGET = fluxion
OBJS = flux_dump.o fluxion.o flux_tree_func.o FLUX_FILE_read.o FLUX_file_work.o Differentiation_functions.o calculator.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

flux_dump.o: flux_dump.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c flux_dump.cpp -o flux_dump.o

fluxion.o: fluxion.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c fluxion.cpp -o fluxion.o

flux_tree_func.o: flux_tree_func.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c flux_tree_func.cpp -o flux_tree_func.o

FLUX_FILE_read.o: FLUX_FILE_read.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c FLUX_FILE_read.cpp -o FLUX_FILE_read.o

FLUX_file_work.o: FLUX_file_work.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c FLUX_file_work.cpp -o FLUX_file_work.o

Differentiation_functions.o: Differentiation_functions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c Differentiation_functions.cpp -o Differentiation_functions.o

calculator.o: calculator.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c calculator.cpp -o calculator.o
    
clean:
	rm -rf *.o *.exe *.dmp *.log *.log.dmp *.exe.log.dmp $(TARGET)

# Дополнительная цель для запуска с переменными окружения ASAN
run: $(TARGET)
	ASAN_OPTIONS=detect_stack_use_after_return=1:check_initialization_order=1:detect_invalid_pointer_pairs=2:strict_string_checks=1 ./$(TARGET)

# Цель для отладки с ASAN
debug: $(TARGET)
	ASAN_OPTIONS=verbosity=1:log_path=asan.log ./$(TARGET)

.PHONY: all clean run debug