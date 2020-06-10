#Meluleki Dube
#

CC := gcc 
SRCDIR := src
BUILDDIR := build
BINDIR := bin
SPIKEDIR := spike
TARGET := bin/runner
WFIND := C:\cygwin64\bin\find

SRCEXT := c
ifeq ($(OS),Windows_NT)
	SOURCES := $(shell $(WFIND) $(SRCDIR) -type f -name *.$(SRCEXT))
else
	SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
endif

OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall
#LIB := 
INC := -I include
LINKLIB:= 


$(TARGET): $(OBJECTS)
	@echo " Linking...";
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $(TARGET) $(LINKLIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) clean
	@mkdir -p $(BUILDDIR);
	@echo "compiling the source files"
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

run:
	./$(TARGET)

clean:
	@echo " Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET) $(BINDIR)

.PHONY: clean