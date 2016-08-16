CC=g++
CFLAGS=-c
LDFLAGS= -g

SOURCEDIR=src
BUILDDIR=bin

EXECUTABLE=$(BUILDDIR)/EasyImageTest
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS=$(patsubst $(SOURCEDIR)/%.cpp, $(BUILDDIR)/%.o, $(SOURCES))

all: $(BUILDDIR) $(EXECUTABLE)

$(BUILDDIR):
	@echo "Got here"
	mkdir -p $(BUILDDIR)

$(EXECUTABLE): $(OBJECTS)
	$(info $(OBJECTS))
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJECTS) : $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BUILDDIR)
