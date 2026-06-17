#include <stdio.h>
#include <string.h>
#include "type.h"
#include "mp3.h"

status read_and_validate_view(FILE* fptr)
{
    char str[4];
    fread(str, 3, 1, fptr);
    str[3] = '\0';

    if(strcmp(str, "ID3") != 0)
    {
        printf("Error : file is not an ID3 one\n");
        return e_failure;
    }

    short int ver;
    fread(&ver, 2, 1, fptr);

    if(ver != 3)
    {
        printf("Error : file is not a version 2.3\n");
        return e_failure;
    }

    return e_success;
}

void big_endian(int* size)
{
    char* ptr = (char*)size;
    for(int i = 0; i < 2; i++)
    {
        char temp = ptr[i];
        ptr[i] = ptr[3 - i];
        ptr[3 - i] = temp;
    }
}

void print_the_details(char** str, int j, int size, FILE* fptr)
{
    printf("%s\t\t:\t", str[j]);
    for(int i = 0; i < size - 1; i++)
    {
        char ch;
        fread(&ch, 1, 1, fptr);
        printf("%c", ch);
    }
    printf("\n");
}

status view_details(char* src_file)
{
    FILE* fptr = fopen(src_file, "r");
    if(fptr == NULL)
    {
        printf("Error : file opening failed\n");
        return e_failure;
    }

    if((read_and_validate_view(fptr)) == e_failure)
    {
        return e_failure;
    }

    char* arr[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    char* tags[6] = {"Title", "Artist", "Album", "Year", "Music", "Comment"};

    fseek(fptr, 5, SEEK_CUR);

    printf("-------------------------------------------------------------------------------------------\n");
    printf("MP3 Tag Reader and Editor for ID3v2\n");
    printf("-------------------------------------------------------------------------------------------\n");

    for(int i = 0; i < 6; i++)
    {
        char str[5];
        fread(str, 4, 1, fptr);
        str[4] = '\0';

        int size;
        fread(&size, 4, 1, fptr);

        big_endian(&size);
        fseek(fptr, 3, SEEK_CUR);

        for(int j = 0; j < 6; j++)
        {
            if((strcmp(str, arr[j])) == 0)
            {
                print_the_details(tags, j, size, fptr);
                break;
            }
        }
    }

    printf("-------------------------------------------------------------------------------------------\n");

    fclose(fptr);
    return e_success;
}

void copy_new_data(FILE* fsrc, FILE* fdup, char* new_data, int size)
{
    int new_size = strlen(new_data) + 1;
    big_endian(&new_size);

    fwrite(&new_size, 4, 1, fdup);

    char flag[3];
    fread(flag, 3, 1, fsrc);
    fwrite(flag, 3, 1, fdup);

    big_endian(&size);
    fseek(fsrc, size - 1, SEEK_CUR);

    fwrite(new_data, strlen(new_data), 1, fdup);
}

void copy_remaining_data(FILE* fsrc, FILE* fdup)
{
    char ch;
    while(fread(&ch, 1, 1, fsrc) == 1)
    {
        fwrite(&ch, 1, 1, fdup);
    }
}

status edit_data(char* new_data, char* src_file, char* tag)
{
    FILE* fsrc = fopen(src_file, "r");
    FILE* fdup = fopen("duplicate.mp3", "w");

    if(fsrc == NULL || fdup == NULL)
    {
        printf("Error in file opening\n");
        return e_failure;
    }

    char header[10];
    fread(header, 10, 1, fsrc);
    fwrite(header, 10, 1, fdup);

    for(int i = 0; i < 6; i++)
    {
        char str[5];
        fread(str, 4, 1, fsrc);
        fwrite(str, 4, 1, fdup);
        str[4] = '\0';

        int size;
        fread(&size, 4, 1, fsrc);

        if((strcmp(str, tag)) == 0)
        {
            copy_new_data(fsrc, fdup, new_data, size);
            continue;
        }

        fwrite(&size, 4, 1, fdup);
        big_endian(&size);

        char data[2 + size];
        fread(data, 2 + size, 1, fsrc);
        fwrite(data, 2 + size, 1, fdup);
    }

    copy_remaining_data(fsrc, fdup);

    fclose(fsrc);
    fclose(fdup);
    return e_success;
}

status edit_details(char* argv[])
{
    char* tag[6] = {"TIT2", "TPE1", "TALB", "TCON", "TYER", "COMM"};

    printf("----------------------------Select Edit option---------------------------\n");

    if(strcmp(argv[2], "-t") == 0)
    {
        printf("------------------------Select Title change option------------------------\n");
        if((edit_data(argv[3], argv[4], tag[0])) == 0)
        {
            printf("\tTitle\t:\t%s\n------------------------Title changed successfully------------------------\n", argv[3]);
            return e_success;
        }
        return e_failure;
    }
    else if(strcmp(argv[2], "-a") == 0)
    {
        printf("------------------------Select Artist name change option------------------------\n");
        if((edit_data(argv[3], argv[4], tag[1])) == 0)
        {
            printf("\tArtist\t:\t%s\n------------------------Artist name changed successfully------------------------\n", argv[3]);
            return e_success;
        }
        return e_failure;
    }
    else if(strcmp(argv[2], "-A") == 0)
    {
        printf("------------------------Select Album name change option------------------------\n");
        if((edit_data(argv[3], argv[4], tag[2])) == 0)
        {
            printf("\tAlbum\t:\t%s\n------------------------Album name changed successfully------------------------\n", argv[3]);
            return e_success;
        }
        return e_failure;
    }
    else if(strcmp(argv[2], "-m") == 0)
    {
        printf("------------------------Select content change option------------------------\n");
        if((edit_data(argv[3], argv[4], tag[3])) == 0)
        {
            printf("\tContent\t:\t%s\n------------------------Content changed successfully------------------------\n", argv[3]);
            return e_success;
        }
        return e_failure;
    }
    else if(strcmp(argv[2], "-y") == 0)
    {
        printf("------------------------Select Year change option------------------------\n");
        if((edit_data(argv[3], argv[4], tag[4])) == 0)
        {
            printf("\tYear\t:\t%s\n------------------------Year changed successfully------------------------\n", argv[3]);
            return e_success;
        }
        return e_failure;
    }
    else if(strcmp(argv[2], "-c") == 0)
    {
        printf("------------------------Select comment change option------------------------\n");
        if((edit_data(argv[3], argv[4], tag[5])) == 0)
        {
            printf("\tComment\t:\t%s\n------------------------Comment changed successfully------------------------\n", argv[3]);
            return e_success;
        }
        return e_failure;
    }
    else
    {
        printf("Error : Pass a valid argument in third position (-t/-a/-A/-m/-y/-c)\n");
        return e_failure;
    }
}
