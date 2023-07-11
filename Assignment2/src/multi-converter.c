/**** multi-converter.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <converters.h>


#define _XOPEN_SOURCE 700



int main(int argc, char **argv)
{
    
    /**** TODO ****/
    int i;
    double amount, target_amount;
    char *ptr;
    amount = strtod(argv[2], &ptr);

    printf("Conversion for: %s %s\n", argv[1], argv[2]);

    for (i = 0; i < NB_CONVERTERS; i ++){
        target_amount = convert(argv[1], determine_currency(i), amount);
        printf("\t%s %lf\n", determine_currency(i), target_amount);
    }

    printf("End of conversion\n");
    return EXIT_SUCCESS;
}
