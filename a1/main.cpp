#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


/*#define*/int BUFFER_SIZE;


const char *input_path;
const char *output_path;
int output_file_desc;
int input_file_desc;


void readfile();


int main(int argc, const char *argv[])
{
	input_path = argv[1];
	output_path = argv[2];
	BUFFER_SIZE = atoi(argv[3]);
	
	printf("iiinnnttt %i", BUFFER_SIZE);
	
	if( input_path == NULL || output_path == NULL )
	{
		std::cout << "Error with input/output path. Exiting...";
		return -1;
	}
	
	std::cout << "input path: " << input_path << "\noutput path: " << output_path << "\n";
	
	input_file_desc = open(input_path, O_RDONLY);
	if (errno < 0)
		std::cout << "Input file error" << errno << ": " << strerror(errno) << "\n";

	output_file_desc = open(output_path, O_WRONLY | O_CREAT | O_EXCL, S_IWUSR);
	if (errno < 0)
		std::cout << "Output file error" << errno << ": " << strerror(errno) << "\n";
	
	if (output_file_desc < 0)
	{
		if (errno == EEXIST)
		{
			char yes_no = 'z';
			while ( ( yes_no != 'y' ) && ( yes_no != 'n')  )
			{
				std::cout << "A file already exists at your write location. Overwrite? (y/n)\n";
				read( 0, &yes_no, 2 );
			}
			
			if (yes_no == 'n')
			{
				std::cout << "Thanks anyway! Exiting...";
				return errno;
			}
			else
				output_file_desc = open(output_path, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
			
			if( output_file_desc < 0 )
			{
				std::cout << "Error writing file. Exiting..." << strerror(errno);
				return errno;
			}
		}
		else
		{
			{
				std::cout << "Error: " << strerror(errno);
				return errno;
			}
		}
	}
		
	if(input_file_desc < 0)
	{
		std::cout << strerror(errno);
		return errno;
	}
	
	time_t time_begin = clock();
	
	readfile();
	std::cout << "with buffer size at " << BUFFER_SIZE << "copying took " << difftime(clock(), time_begin)/CLOCKS_PER_SEC << " seconds";
	
	close(input_file_desc);
	close(output_file_desc);
	
	return 0;
}

void readfile()
{

	char the_buffer [BUFFER_SIZE];
	
	float retval = read( input_file_desc, the_buffer, ( BUFFER_SIZE * sizeof(char) )  );
	
	while ( retval > 0 )
	{
		write(output_file_desc, the_buffer, retval );
		retval = read( input_file_desc, the_buffer, ( BUFFER_SIZE * sizeof(char) ) );		
	}
}
