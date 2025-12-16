CXX = g++
CXXFLAGS = -D_DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -I/usr/include/ \
           -Weffc++ -Wcast-align -Wcast-qual -Wchar-subscripts \
           -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
           -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline \
           -Wlogical-op -Wnon-virtual-dtor -Woverloaded-virtual \
           -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow \
           -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 \
           -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
           -Wswitch-default -Wundef -Wunreachable-code \
           -Wunused -Wuseless-cast -Wvariadic-macros \
           -Wno-missing-field-initializers -Wno-narrowing \
           -Wno-old-style-cast -Wstack-protector -fcheck-new \
           -fsized-deallocation -fstack-protector -fstrict-overflow \
           -fno-omit-frame-pointer -pie -fPIE -Werror=vla \
           -fsanitize=address,undefined,leak,bounds \
           -fsanitize-address-use-after-scope \
           -fno-optimize-sibling-calls

INCLUDE_DIRS = -IHeaders -I/common

LDFLAGS = -pie -fPIE \
          -fsanitize=address,undefined,leak,bounds \
          -fsanitize-address-use-after-scope \
          -fno-optimize-sibling-calls

TARGET = fluxion
OBJS = $(PTO)flux_dump.o $(PTO)fluxion.o $(PTO)flux_tree_func.o $(PTO)FLUX_FILE_read.o $(PTO)FLUX_file_work.o $(PTO)Differentiation_functions.o $(PTO)calculator.o $(PTO)SimplifyTree.o $(PTO)GNUPlot.o
PTS = Source/
PTO = Obj/
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

$(PTO)flux_dump.o: $(PTS)flux_dump.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)flux_dump.cpp -o $(PTO)flux_dump.o

$(PTO)fluxion.o: $(PTS)fluxion.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)fluxion.cpp -o $(PTO)fluxion.o

$(PTO)flux_tree_func.o: $(PTS)flux_tree_func.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)flux_tree_func.cpp -o $(PTO)flux_tree_func.o

$(PTO)FLUX_FILE_read.o: $(PTS)FLUX_FILE_read.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)FLUX_FILE_read.cpp -o $(PTO)FLUX_FILE_read.o

$(PTO)FLUX_file_work.o: $(PTS)FLUX_file_work.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)FLUX_file_work.cpp -o $(PTO)FLUX_file_work.o

$(PTO)Differentiation_functions.o: $(PTS)Differentiation_functions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)Differentiation_functions.cpp -o $(PTO)Differentiation_functions.o

$(PTO)calculator.o: $(PTS)calculator.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)calculator.cpp -o $(PTO)calculator.o

$(PTO)SimplifyTree.o: $(PTS)SimplifyTree.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)SimplifyTree.cpp -o $(PTO)SimplifyTree.o

$(PTO)GNUPlot.o: $(PTS)GNUPlot.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $(PTS)GNUPlot.cpp -o $(PTO)GNUPlot.o    
clean:
	rm -rf $(PTO)*.o *.exe *.dmp *.log *.log.dmp *.exe.log.dmp $(TARGET)

png_clean:
	rm -rf  photo/*.png
# Дополнительная цель для запуска с переменными окружения ASAN
run: $(TARGET)
	ASAN_OPTIONS=detect_stack_use_after_return=1:check_initialization_order=1:detect_invalid_pointer_pairs=2:strict_string_checks=1 ./$(TARGET)

# Цель для отладки с ASAN
debug: $(TARGET)
	ASAN_OPTIONS=verbosity=1:log_path=asan.log ./$(TARGET)

.PHONY: all clean run debug png_clean