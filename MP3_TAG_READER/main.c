#include <stdio.h>
#include <string.h>
#include "type.h"
#include "mp3.h"

operationtype check_operation(char*);

int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        printf("----------------------------------------------------------------------------------\n");
        printf("ERROR : %s : INVALID ARGUMENTS\nUSAGE :\nTo view: ./a.out -v mp3filename\nTo edit: ./a.out -e -t/-a/-A/-m/-y/-c new_name mp3filename\n", argv[0]);
        printf("----------------------------------------------------------------------------------\n");
        return e_failure;
    }

    operationtype ret = check_operation(argv[1]);

    if(ret == e_view)
    {
        if(argc == 3)
        {
            if((view_details(argv[2])) == 0)
            {
                return e_success;
            }
            else
            {
                printf("Viewing details failed\n");
                return e_failure;
            }
        }
        else
        {

            
            printf("Error : 3 arguments are required to perform view function\n");
            printf("To view: ./a.out -v mp3filename\n");
            return e_failure;
        }
    }
    else if(ret == e_edit)
    {
        if(argc == 5)
        {
            if((edit_details(argv)) == 0)
            {
                remove(argv[4]);
                rename("duplicate.mp3", argv[4]);
                return e_success;
            }
            else
            {
                printf("Editing details failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("Error : 5 arguments are required to perform edit operation\n");
            printf("To edit: ./a.out -e -t/-a/-A/-m/-y/-c new_name mp3filename\n");
            return e_failure;
        }
    }
    else if(ret == e_help)
    {
        printf("--------------------------------------HELP MENU--------------------------------------\n");
        printf("1. -v -> view mp3 file contents\n2. -e -> edit mp3 file contents\n");
        printf("   -t -> edit song title\n   -a -> edit artist name\n   -A -> edit album name\n");
        printf("   -y -> edit year\n   -m -> edit content\n   -c -> edit comment\n");
        printf("--------------------------------------------------------------------------------------\n");
        return e_success;
    }
    else
    {
        printf("Error : Invalid argument. Use -e (edit), -v (view), --help\n");
        return e_failure;
    }
}

operationtype check_operation(char* symbol)
{
    if(strcmp(symbol, "-v") == 0)
    {
        return e_view;
    }
    else if(strcmp(symbol, "-e") == 0)
    {
        return e_edit;
    }
    else if(strcmp(symbol, "--help") == 0)
    {
        return e_help;
    }
    else
    {
        return e_unsupported;
    }
}
