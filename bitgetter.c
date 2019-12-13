#include <stdio.h>

int getBit(int n) 
{ 
    for (int i = 31; i >= 0; i--) { 
        int k = n >> i; 
        if (k & 1) 
            printf("1"); 
        else
            printf("0"); 
    } 
} 
  
int main() 
{ 
    int n = 4294967295; 
    getBit(n); 
    printf("\n");
} 