CXX = g++
TARGET = ./ana
OBJDIR=./obj
SOURCES = $(wildcard *.cpp)
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs) -lRHTTP
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o)) $(OBJDIR)/config.o

CFLAGS = -O2 -Wall -Wno-write-strings -mtune=native -march=native -lncurses ${ROOTFLAGS}
LIBS = ${ROOTLIBS}
DEBAG = -g

all: ${TARGET}
${TARGET}: $(OBJECTS)
	${CXX} -o $@ $^ ${CFLAGS} ${LIBS}

$(OBJDIR)/config.o: config.c
	gcc $< -c -o $@ -lm -DLinux

$(OBJDIR)/%.o: %.cpp
	@[ -d $(OBJDIR) ]
	$(CXX) ${CFLAGS} ${LIBS} -o $@ -c $<


clean:
	${RM} ${OBJDIR}/*.o ${TARGET}
