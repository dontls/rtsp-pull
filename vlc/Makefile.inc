SHELL =/bin/bash

CC	:= gcc
CXX	:= g++
LD	:= ld
AR	:= ar
STRIP	:= strip

CFLAGS += -g -Wall -fPIC -Wdeprecated-declarations
CFLAGS += $(INCLUDE)

%.o: %.cpp
	@echo "CXX  " $@;
	@$(CXX) $(CFLAGS) -c -o $@ $<


%.o: %.c
	@echo "CC  " $@;
	@$(CC) $(CFLAGS) -c -o $@ $<
