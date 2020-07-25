CC= g++
LD= $(CC)
CTAGS= ctags

#CFLAGS= -std=c++2a -O2
CFLAGS= -std=c++2a -O0 -ggdb
LFLAGS=

SRCDIR= src
INCLUDE= include $(SRCDIR)
BINDIR= bin
OBJDIR= obj

TARGET= main
SRCFILES= main.cpp \
	  $(SRCDIR)/SMatch.cpp \
	  $(SRCDIR)/SFormula.cpp \
	  $(SRCDIR)/CBinarySolver.cpp \
	  $(SRCDIR)/CGeneralSolver.cpp
OBJFILES= $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SRCFILES)))

TAGFILE= tags

.PHONY: clean tags

all: $(TARGET)

$(TARGET): $(OBJFILES)
	mkdir -p $(BINDIR)
	$(LD) $(LFLAGS) $(OBJFILES) -o $(BINDIR)/$(TARGET)

$(OBJFILES): $(SRCFILES)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(patsubst %,-I%,$(INCLUDE)) -c $(SRCFILES)
	mv *.o $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*
	rm -f $(TAGFILE)

tags:
	$(CTAGS) -R -f $(TAGFILE)
