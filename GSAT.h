#ifndef GSAT_H_INCLUDED
#define GSAT_H_INCLUDED

#define varNum 70
int GSAT(CNFPtr &F, int formulaNum, //int varNum,
        int maxTries, int maxSteps);
void generateCNFArray(CNFPtr &F, int formulaNum, int CNFA[][varNum]);
int calcuUnsatisfied(int CNFA[][varNum], int formulaNum, int tempSol[]);

void generateScoreArray(int CNFA[][varNum], int formulaNum, int tempSol[],
                        int satStatus[], int scoreArray[]);
void generateVarDependeceArray(int CNFA[][varNum], int formulaNum, int VD[][varNum]);
void generateFormulaDependeceArray(int CNFA[][varNum], int formulaNum, int FD[][varNum]);
int filp(int CNFA[][varNum], int formulaNum, int tempSol[], int SA[], int satStatus[],
            int FD[][varNum], int VD[][varNum]);
#endif // GSAT_H_INCLUDED
