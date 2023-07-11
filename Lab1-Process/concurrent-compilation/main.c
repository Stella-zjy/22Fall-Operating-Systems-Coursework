#include "titi.h"
#include "tata.h"
#include "toto.h"

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char** argv) {


    titi();
    tata();
    toto();
}


