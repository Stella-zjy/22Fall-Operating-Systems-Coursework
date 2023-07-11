#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


int main (int arg, char * argv [])
{
    int i, j, p, k;
    for (i = 0; i <3; i ++)
        if ((p = fork ()) == 0){
            printf( "i =% d \n", i);
            j = 0;
            while (j < i && ((p = fork ()) == 0))
                j ++;
            if (p == 0)
                printf ( "j =% d \n", j);
            exit(j);
        } 
        else {
            for (k = 0; k <= i; k ++)
                wait(NULL);
        }
    return (0);
}
