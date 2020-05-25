#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// 

int getCPSR(int *registers){
    return registers[14];
}

