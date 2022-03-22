CXX = g++
TARGET = ./ana
OBJDIR=./obj
SOURCES = $(wildcard *.cpp)
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o)) $(OBJDIR)/config.o

CFLAGS = -O2 -Wall -mtune=native -march=native ${ROOTFLAGS}
LIBS = ${ROOTLIBS}
DEBAG = -g

all: ${TARGET}
${TARGET}: $(OBJECTS)
	${CXX} -o $@ $^ ${CFLAGS} ${LIBS}

$(OBJDIR)/config.o: config.c
	gcc config.c -c -o $(OBJDIR)/config.o

$(OBJDIR)/%.o: %.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) ${CFLAGS} ${LIBS} -o $@ -c $<


clean:
	${RM} ${OBJDIR}/*.o ${TARGET}
