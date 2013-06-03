 /*
 * Original Author: Marty Ulrich (ulrichm)
 * File: main.cpp
 * Created: 2012 January 14, 10:54 by ulrichm
 * Last Modified: 2012 January 24, 12:28 by ulrichm
 * 
 * This file contains a functions that copy a file from one location to another.
 */ 


// for inputting and outputing data
#include <iostream>
//#include <iomanip>

// used for opening/closing/reading/writing files
#include <fcntl.h>

// to display errors
#include <errno.h>
#include <string.h>

// needed for displaying errors and other standard operations
#include <stdlib.h>
#include <stdio.h>


// size of char array to use as buffer for copying data
#define BUFFER_SIZE 8192

// file path of input and output file
const char *input_path;
const char *output_path;

// read/write file from input to output
void read_write_file(int in_desc, int out_desc);


/*
 * ----OVERVIEW----
 * Main Function, takes two command-line arguments and 
 * uses them as the variables "input_path" and "output_path".
 * Checks integrity of both paths.  If file exists at output_path,
 * asks user if they want to overwrite.  Opens path and sends file 
 * descriptor to read_write_file function.
 * 
 * ----PARAMETERS----
 * Standard C++ "main" parameters.
 *
 * ----NOTABLE FUNCTION VARIABLES----
 * input_path - file path of input data file.
 * output_path - file path of output data file.
 * input_file_desc - int of file descriptor for file loaded from input_path
 * output_file_desc - int of file descriptor for file loaded from input_path
 */
 
int main(int argc, char *argv[]){
	input_path = argv[1];
	output_path = argv[2];
	
	int input_file_desc;
	int output_file_desc;
	
	// checks that user provided input/output paths as command line arguments
	if (input_path == NULL || output_path == NULL){
		std::cout << "Error with input/output path. Exiting...\n";
		return -1;
	}
	
	std::cout << "input path: " << input_path << "\noutput path: " << output_path << "\n";
	
	// attempts to open file at input/output paths, shows error and exits if necessary
	input_file_desc = open(input_path, O_RDONLY);
	if (errno < 0){
		std::cout << "Input file error" << errno << ": " << strerror(errno) << "\n";
	}
	
	output_file_desc = open(output_path, O_WRONLY | O_CREAT | O_EXCL, S_IWUSR);
	if (errno < 0){
		std::cout << "Output file error" << errno << ": " << strerror(errno) << "\n";
	}
	
	// checks for errors (existing file at output_path, etc), and handles accordingly
	if (output_file_desc < 0){
		if (errno == EEXIST){	// file exists at output path
			char yes_no = 'z';
			while ((yes_no != 'y') && (yes_no != 'n')){
				std::cout << "A file already exists at your write location."
						  << "Overwrite? (y/n)\n";
				read(0, &yes_no, 2);
			}
			
			if (yes_no == 'n'){
				std::cout << "Thanks anyway! Exiting...\n";
				return errno;
			}else{
				output_file_desc = open(output_path, 
										O_CREAT | O_WRONLY | O_TRUNC,
										S_IRUSR | S_IWUSR);
			}
			
			if (output_file_desc < 0){
				std::cout << "Error writing file. Exiting...\n" << strerror(errno);
				return errno;
			}
		}else{
			std::cout << "Error: " << strerror(errno) << "\n";
			return errno;
		}
	}
		
	if (input_file_desc < 0){
		std::cout << strerror(errno);
		return errno;
	}
		
	read_write_file(input_file_desc, output_file_desc);
	
	close(input_file_desc);
	close(output_file_desc);
	
	return 0;
}


/*
 * ----OVERVIEW----
 * Reads/writes data from file with file descriptor in_desc to out_desc
 *
 * ----PARAMETERS----
 * in_desc - the int file descriptor of the file to be copied.
 * out_desc - the int file descriptor of the file to copy to.
 *
 * ----NOTABLE FUNCTION PARAMETERS----
 * the_buffer - char array of size determined by global constant BUFFER_SIZE used
 *				as a buffer for file copying.
 * bytes_read - float containing how many bytes were read, so we can write that many.
 */
 
void read_write_file(int in_desc, int out_desc){
	char the_buffer [BUFFER_SIZE];
	float bytes_read = read(in_desc, the_buffer, (BUFFER_SIZE * sizeof(char)));
	
	// reads data in BUFFER_SIZE blocks, writes out to output file
	while (bytes_read > 0){
		write(out_desc, the_buffer, bytes_read);
		bytes_read = read(in_desc, the_buffer, (BUFFER_SIZE * sizeof(char)));		
	}
}
