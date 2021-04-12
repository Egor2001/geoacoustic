CC= g++ # aarch64-linux-gnu-g++
LD= $(CC)

ROOT= ../src
IFLAGS= -I$(ROOT) -I$(ROOT)/static -I$(shell pwd)

RELEASE_CFLAGS= -Ofast
DEBUG_CFLAGS= -O0 -ggdb

BENCH_CFLAGS= $(RELEASE_CFLAGS) -DGEO_BENCH
PROFILE_CFLAGS= $(BENCH_CFLAGS) -ggdb

CFLAGS= -std=c++14 -fopenmp -Wall -Wextra -pedantic $(IFLAGS)
LFLAGS= -fopenmp

BINDIR= bin
OBJDIR= obj

SRCFILE= $(ROOT)/main.cpp
OBJFILE= $(OBJDIR)/main.o
TARGET= $(BINDIR)/main

.PHONY: clean

all: release

release: release_obj $(TARGET)

debug: debug_obj $(TARGET)

bench: bench_obj $(TARGET)

profile: profile_obj $(TARGET)

$(TARGET): $(OBJFILE) $(BINDIR)
	$(LD) $(LFLAGS) $(OBJFILE) -o $(TARGET)

release_obj: $(SRCFILE) $(OBJDIR)
	$(CC) $(CFLAGS) $(RELEASE_CFLAGS) -c $(SRCFILE) -o $(OBJFILE)

debug_obj: $(SRCFILE) $(OBJDIR)
	$(CC) $(CFLAGS) $(DEBUG_CFLAGS) -c $(SRCFILE) -o $(OBJFILE)

bench_obj: $(SRCFILE) $(OBJDIR)
	$(CC) $(CFLAGS) $(BENCH_CFLAGS) -c $(SRCFILE) -o $(OBJFILE)

profile_obj: $(SRCFILE) $(OBJDIR)
	$(CC) $(CFLAGS) $(PROFILE_CFLAGS) -c $(SRCFILE) -o $(OBJFILE)

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJDIR)/$(OBJFILE)
