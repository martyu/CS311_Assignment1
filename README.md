CS311_Assignment1
=================

Simple C++ program to copy a file from one location to another.


## Overview

This program takes in one file and copies it to a new location. It was designed to help up understand new tools that we hadn’t used before, and to the relation between time and block size loaded into the buffer when copying files. I varied the block sizes in my testing from 1 to 8192, using every power of two in between. The results were as can be expected. The smaller the buffer size, the longer the file took to copy.


## Functions

The program only contains one source file, ”main.cpp”. The source file is writted in C++. The main function takes in two parameters, argc and argv[] - which are the standard inputs for the main function of a C++ program - and parses two constant character arrays from argv[]. The character arrays hold path names to the input file and where it is to be copied to. It checks to make sure the path names are valid, and opens the files. If there is a file already at the output path name, it asks for user input on how to handle. It then passes the file descriptors of the files, which are held in the integer variables ”input file desc” and ”output file desc”, to the function ”read write file” for read and write handling.
The function ”read write file” takes the two integer file descriptors and reads in the data from the input file, in block sizes defined by the global constant ”BUFFER SIZE”, and writes it to the file described by ”output file desc”. Once the file is copied, the function ends, and the main function closes the files and ends.
