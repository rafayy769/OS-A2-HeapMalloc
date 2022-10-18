# makefile for the "make" utility

CC = gcc
CFLAGS = -g -Wall
RM = rm -f
TARGET = bin/memtest
VALG_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

ALLOC_SRC = src/my_allocator.c
ALLOC_OBJ = obj/my_allocator.o
ALLOC_INC = include/my_allocator.h

ACKER_SRC = src/ackermann.c
ACKER_OBJ = obj/ackermann.o
ACKER_INC = include/ackermann.h

MEM_SRC = src/memtest.c

all: memtest

ackermann.o: $(ACKER_SRC) $(ACKER_INC)
	gcc -c -g $(ACKER_SRC) -o $(ACKER_OBJ)

my_allocator.o : $(ALLOC_SRC) $(ALLOC_INC)
	gcc -c -g $(ALLOC_SRC) -o $(ALLOC_OBJ)

memtest: ackermann.o my_allocator.o
	gcc -o $(TARGET) $(MEM_SRC) $(ACKER_OBJ) $(ALLOC_OBJ)

out: all
	clear
	$(TARGET)

valgrind: all
	valgrind $(VALG_FLAGS) $(TARGET)

clean:
	$(RM) $(TARGET) $(ACKER_OBJ) $(ALLOC_OBJ)