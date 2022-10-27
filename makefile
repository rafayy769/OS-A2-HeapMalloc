# makefile for the "make" utility

CC = gcc
CFLAGS = -O3
RM = rm -f
TARGET = bin/memtest
TEST_TARGET = bin/test
STD_TEST_TARGET = bin/standard_malloc
VALG_FLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

ALLOC_SRC = src/my_allocator.c
ALLOC_OBJ = obj/my_allocator.o
ALLOC_INC = include/my_allocator.h

ACKER_SRC = src/ackermann.c
ACKER_OBJ = obj/ackermann.o
ACKER_INC = include/ackermann.h

MEM_SRC = src/memtest.c
TEST_SRC = src/test.c

all: memtest

ackermann.o: $(ACKER_SRC) $(ACKER_INC)
	$(CC) -c $(ACKER_SRC) -o $(ACKER_OBJ) $(CFLAGS)

my_allocator.o : $(ALLOC_SRC) $(ALLOC_INC)
	$(CC) -c $(ALLOC_SRC) -o $(ALLOC_OBJ) $(CFLAGS)

memtest: ackermann.o my_allocator.o
	$(CC) -o $(TARGET) $(MEM_SRC) $(ACKER_OBJ) $(ALLOC_OBJ)

out:
	$(TARGET)

run:
	$(TARGET) -b $(B) -s $(S)

valgrind: all
	valgrind $(VALG_FLAGS) $(TARGET)

clean:
	$(RM) $(TARGET) $(ACKER_OBJ) $(ALLOC_OBJ) $(TEST_TARGET) $(STD_TEST_TARGET)

test: ackermann.o my_allocator.o
	$(CC) -o $(TEST_TARGET) $(TEST_SRC) $(ACKER_OBJ) $(ALLOC_OBJ)

val_test: test
	valgrind $(VALG_FLAGS) $(TARGET)

standard_malloc: src/standard_malloc.c
	$(CC) -o $(STD_TEST_TARGET) src/standard_malloc.c $(CFLAGS)
	$(STD_TEST_TARGET)