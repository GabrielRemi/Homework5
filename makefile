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
PDF = main.pdf
TAR = fischer_remiszewski_h5.tar

all: mkdirs $(BIN)
	$(BIN)
	python3 plots.py
	
mkdirs:
	mkdir -p $(BUILDDIR) $(RESDIR) $(FIGDIR)

$(BIN): $(COBJ)
	$(CC) $(CFLAGS) $^ -o $@ -pthread

$(BUILDDIR)/%.o: %.cpp	
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -r $(BUILDDIR) $(RESDIR) $(FIGDIR)


login:
	ssh s6garemi@cip-pool.physik.uni-bonn.de

distribute: tar
	scp $(TAR) s6garemi@cip-pool.physik.uni-bonn.de:~/Computer_Physik


tar: clean
	tar -cvf $(TAR) header/* $(PDF) $(CFILES) plots.py makefile
