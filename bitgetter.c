#include <stdio.h>

int count = 0;
int getBit(int bitnum, int bitmap) 
{ 
    return (bitmap >> 31-bitnum) & 1;
} 

int setBit(int bitnum, int bitmap) 
{ 
    return bitmap & ~(1 << 31-bitnum);
} 
int get(int bitmap) 
{ 
    for (int i = 31; i >= 0; i--) { 
        count++;
        int k = bitmap >> 31-i; 
        if (k & 1) 
            printf("1"); 
        else
        {
            printf("0"); 
        } 
    }
} 

int main() 
{ 
    //int n = 4294967295; 
    int n = 4294966271; // 11111111111111111111101111111111 [11.sırada]
    get(n);
    printf("\ninum: %d\n", getBit(21, n));
    n = setBit(21,n);
    printf("\n");
    printf("\ninum: %d\n", getBit(21, n));
    get(n);
    int i = 0;
    while(getBit(i, n) != 0) 
    {
        i++;
    }
    printf("\nsa: %d", i-1);

    printf("\n");
}  