ROOTFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --libs`
DEBUG = -Wall

all: ana.cxx
	c++ $(ROOTLIBS) $(ROOTFLAGS) $(DEBUG) -o ana decoder.cxx ana.cxx

clean:
	rm -f ana *.o
