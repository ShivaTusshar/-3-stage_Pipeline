#include <stdio.h>

int main(void) {
    int n = 7; // Let's calculate the 7th Fibonacci number (13)
    int a = 0;
    int b = 1;
    int next_term;

    // Handle base cases
    if (n == 0) return a;
    if (n == 1) return b;

    // Calculate Fibonacci up to n
    for (int i = 2; i <= n; i++) {
        next_term = a + b;
        a = b;
        b = next_term;
    }
    
    // The final result will be stored in register a0 (x10)
    return b; 
}