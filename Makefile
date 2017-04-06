CC=g++
CFLAGS=-c -Wall
LDFLAGS=-std=c++11
SOURCES=main.cpp utility/readFile.cpp utility/outputFormatting.cpp utility/sorting.cpp proc/processInfo.cpp proc/cpuInfo.cpp proc/memoryInfo.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=process_list

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lncurses

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o process_list
	rm -rf proc/*.o
	rm -rf utility/*.o
