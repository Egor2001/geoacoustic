CC= g++
LD= $(CC)

ROOT= ../src
IFLAGS= -I$(ROOT) -I$(ROOT)/static

CFLAGS= -std=c++14 -Ofast -Wall -Wextra -pedantic $(IFLAGS)
LFLAGS= 

BINDIR= bin
OBJDIR= obj

SRCFILE= $(ROOT)/main.cpp
OBJFILE= $(OBJDIR)/main.o
TARGET= $(BINDIR)/main

all: $(TARGET)

$(TARGET): $(OBJFILE) $(BINDIR)
	$(LD) $(LFLAGS) $(OBJFILE) -o $(TARGET)

$(OBJFILE): $(SRCFILE) $(OBJDIR)
	$(CC) $(CFLAGS) -c $(SRCFILE) -o $(OBJFILE)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)
