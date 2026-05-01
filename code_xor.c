#include <stdio.h>

int main(void) {
    int a = 15;      // In binary: 1111
    int b = 9;       // In binary: 1001
    int c = a ^ b;   // Result should be 6 (binary: 0110)
    
    return c;
}