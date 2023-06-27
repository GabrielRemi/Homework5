CC = g++
INCDIR = header
CFLAGS = -Wall -pedantic -O3 -I $(INCDIR)
CFILES = $(filter-out ./test.cpp, $(wildcard *.cpp))
CFILES = main.cpp misc.cpp odesolver.cpp
BUILDDIR = build
COBJ = $(patsubst %, $(BUILDDIR)/%, $(CFILES:.cpp=.o))
BIN = $(BUILDDIR)/app

all: mkdirs $(BIN)
	$(BIN)
	python3 plots.py

mkdirs:
	mkdir -p $(BUILDDIR)

$(BIN): $(COBJ)
	$(CC) $(CFLAGS) $^ -o $@ -pthread

$(BUILDDIR)/%.o: %.cpp	
	$(CC) $(CFLAGS) -c $^ -o $@ 

clean:
	rm -r $(BUILDDIR)

test: test.cpp misc.cpp
	$(CC) $(CFLAGS) $^ -o test -pthread
	./test


echo:
	echo $(CFILES)
	echo $(COBJ)