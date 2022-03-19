CXX = g++
TARGET = ana
OBJDIR=./
SOURCE = $(OBJDIR)/main.o $(OBJDIR)/analysis.o $(OBJDIR)/anapla.o $(OBJDIR)/anagr.o $(OBJDIR)/anassd.o $(OBJDIR)/decoder.o $(OBJDIR)/histdef.o $(OBJDIR)/treedef.o $(OBJDIR)/config.o
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
