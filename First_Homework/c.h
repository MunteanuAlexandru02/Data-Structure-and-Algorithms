#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*Functia are rolul de a calcula C conform cerinei*/
double omega(int i, int k){
    double numarator, numitor, nr;
    numarator = (i*1.0/(k-1))*(i*1.0/(k-1))*0.9+0.1;
    numitor = 0;
    for(int q = 0; q < k; q++){
        nr = (q*1.0/(k-1))*(q*1.0/(k-1))*0.9+0.1;
        numitor += nr;
    }
    return numarator/numitor;
}
