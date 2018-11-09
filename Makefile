ROOTFLAGS = `root-config --cflags`
ROOTLIBS = `root-config --libs`
DEBUG = -Wall

all: ana.cxx
	$(CXX) $(ROOTLIBS) $(ROOTFLAGS) $(DEBUG) -o ana decoder.cxx vdc.c ana.cxx

clean:
	rm -f ana *.o
