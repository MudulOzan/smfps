#include <stdio.h>

int count = 0;
int getBit(int n) 
{ 
    for (int i = 31; i >= 0; i--) { 
        count++;
        int k = n >> 31-i; 
        if (k & 1) 
            printf("1"); 
        else
        {
            printf("0"); 
            return --count;
        } 
    }
} 

int main() 
{ 
    //int n = 4294967295; 
    int n = 4294966271; // 11111111111111111111101111111111 [11.sırada]
    printf("\ninum: %d", getBit(n));
    printf("\n");
}  