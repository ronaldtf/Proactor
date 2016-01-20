CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := ../src
BUILDDIR := ./build
TARGET := Proactor
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11
LIB := #-pthread -L lib
INC := -I include

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@echo "Creating folder "$(BUILDDIR)"..."
	@mkdir -p $(BUILDDIR)
	@echo "Creating target "$(TARGET)"..."
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning previous compilations..."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

all: clean $(TARGET)

.PHONY: clean
