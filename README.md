# Heap Memory Allocator

## CS370 - Operating Systems
Instructor: Dr. Hammad Alizai  
TA: Ibrahim Moazzam

### Authors
Abdul Rafay - 24100173  
Arfa Yaseen - 24100xxx

## Introduction
This assignment is a simple implementation of a heap memory allocator. It is a dynamic memory allocator that is used to allocate and free memory at runtime. It is a general purpose allocator that can be used to allocate memory for any data type. The allocator works on the buddy system algorithm. It is a memory allocation scheme that divides memory into blocks of equal size and then combines them to form larger blocks. The allocator is implemented in C and uses one malloc call to get a large portion of heap available.

## How to run
The project can be run on any Linux machine. The project is compiled using the makefile. The makefile has multiple targets depending upon the need.  
To compile the project, run the following command:
```
make
```
To run the ackermann function with our implementation, and with the default values in the code, run the following command:
```
make out
```
However, in order to run with different values of basic block size and size of largest bloc, run the following command:
```
make run B=<basic_block_size> S=<largest_block_size>
```
For the analysis, use the following values for basic block size and largest block size: (16 Bytes and 32 MB respectively)
```
B=16 S=33554432
```
To run the ackermann function with standard `malloc` and `free`, run the following command: (Use this for comparison purposes.)
```
make standard_malloc
```
In order to run the file with `valgrind`, run the the following command:
```
make val_test
```