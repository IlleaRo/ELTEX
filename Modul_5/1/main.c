#include <stdio.h>

#define A 63
#define B 45

extern long multiply(int, int);
extern float division(int, int);

int main(){
    int a = A;
    int b = B;
    printf("We have two number:"
    "\n1) %d\n2) %d\nA * B = %ld\nA / B = %f\n",
    a,b, multiply(a,b),division(a,b));
    return 0;
}