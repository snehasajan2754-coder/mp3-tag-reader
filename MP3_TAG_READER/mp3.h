#ifndef MP3_H
#define MP3_H

#include <stdio.h>
#include "type.h"

status view_details(char*);
status read_and_validate_view(FILE*);
void big_endian(int*);
void print_the_details(char**, int, int, FILE*);
status edit_details(char* argv[]);
status edit_data(char*, char*, char*);
void copy_new_data(FILE*, FILE*, char*, int);
void copy_remaining_data(FILE*, FILE*);

#endif
