.PHONY: clean all run
CC = g++
CCFLAGS = -Wall -std=c++11
TARGET = client/Client
SRCEXT := cpp
SOURCES = $(shell find . -type f -name *.$(SRCEXT))
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)
 
# To obtain object files
%.o: %.cpp
	$(CC) -c $(CCFLAGS) $< -o $@
 
# To remove generated files
clean:
	rm -f $(TARGET) $(OBJECTS)
all: clean $(TARGET)

run:	all
	./$(TARGET)

doxygen:
	doxygen .doxygen.conf
