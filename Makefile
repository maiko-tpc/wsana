CXX = g++
TARGET = ana
SOURCE = main.o analysis.o anapla.o anagr.o anassd.o decoder.o histdef.o treedef.o
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

CFLAGS = -O2 -Wall -mtune=native -march=native ${ROOTFLAGS}
LIBS = ${ROOTLIBS}
DEBAG = -g

all: ${TARGET}
${TARGET}: ${SOURCE}
	${CXX} $^ -o $@ ${CFLAGS} ${LIBS}
#	$(RM) *.o

.cpp.o:
	${CXX} -c ${CFLAGS} $<

clean:
	${RM} *.o ${TARGET}

refresh:
	${RM} *.o
