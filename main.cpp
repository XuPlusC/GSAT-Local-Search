#include <stdio.h>
#include "CNF.h"
#include "GSAT.h"

int main()
{
    CNFPtr F;
    int formulaNum = constructCNF(F, "test.txt");
    printf("formulaNum :%d\n", formulaNum);
    int i;
    float sum = 0;
	sum += GSAT(F, formulaNum, 100, 350);
    printf("%f\n", sum);

    return 0;
}
