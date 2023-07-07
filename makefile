CC = g++
INCDIR = header
CFLAGS = -Wall -pedantic -O3 -I $(INCDIR)
CFILES = $(filter-out ./test.cpp, $(wildcard *.cpp))
CFILES = main.cpp misc.cpp odesolver.cpp
BUILDDIR = build
RESDIR = results
FIGDIR = figs
COBJ = $(patsubst %, $(BUILDDIR)/%, $(CFILES:.cpp=.o))
BIN = $(BUILDDIR)/app

all: mkdirs $(BIN)
	$(BIN)
	
mkdirs:
	mkdir -p $(BUILDDIR) $(RESDIR) $(FIGDIR)

$(BIN): $(COBJ)
	$(CC) $(CFLAGS) $^ -o $@ -pthread

$(BUILDDIR)/%.o: %.cpp	
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -r $(BUILDDIR) $(RESDIR) $(FIGDIR)

test: test.cpp misc.cpp
	$(CC) $(CFLAGS) $^ -o test
	./test
	rm -r test

login:
	ssh s6garemi@cip-pool.physik.uni-bonn.de

reference:
	gcc zg-full.c $(CFLAGS) -o zg -lm
	./zg
	rm -r zg

echo:
	echo $(CFILES)
	echo $(COBJ)