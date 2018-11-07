#include "native.h"

void hello_cpp()
{
    printf("you are in cpp now.\n");
}

double f1_cpp(int n)
{
    double s = 0;
    for(int i=0; i<=n; i++)
        s += sqrt(i);
    return s;
}
