#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "CNF.h"
#include "GSAT.h"
// input CNF formula F , positive integers maxTries and maxSteps
// output model of F or “no solution found”

//extern const int varNum;

int GSAT(CNFPtr &F, int formulaNum, //int varNum,
                int maxTries, int maxSteps)
{
    srand((unsigned)time(0));

    int tempSol[varNum], CNFA[formulaNum][varNum];
    int i, j, k;
    generateCNFArray(F, formulaNum, CNFA);

    // for (i = 0; i < formulaNum; ++i)
    // {
    //     for (j = 0; j < varNum; ++j)
    //         printf("%d\t", CNFA[i][j]);
    //     printf("\n");
    // }

    for (i = 1; i <= maxTries; ++i)
    {
        for(j = 0; j < varNum; ++j)
        {
            tempSol[j] = rand()%2;
            // printf("randomly generated %d : %d\n", j+1, tempSol[j]);    ////////////
        }

        int scoreArray[varNum] = {0};
        int satStatus[formulaNum] = {0};
        generateScoreArray(CNFA, formulaNum, tempSol, satStatus, scoreArray);

        int VD[varNum][varNum] = {};
        generateVarDependeceArray(CNFA, formulaNum, VD);

        int FD[formulaNum][varNum] = {};
        generateFormulaDependeceArray(CNFA, formulaNum, FD);

        int unsatisfied;
        unsatisfied = calcuUnsatisfied(CNFA, formulaNum, tempSol);
        // printf("original unsatisfied: %d\n", unsatisfied);  /////////////

        // printf("\nconfirming original var:\n");         ///////////
        // for(j = 0; j < varNum; ++j)
        // {
        //     printf("value for var %d is: %d\n", j+1, tempSol[j]);
        // }

        // printf("\nconfirming original sat status:\n");
        // for(j = 0; j < formulaNum; ++j)
        // {
        //     printf("status for clause %d is: %s\n", j+1, (satStatus[j] == 1)?"Yes":"No");
        // }

        // printf("\nconfirming original score:\n");
        // for(j = 0; j < varNum; ++j)
        // {
        //     printf("score for node %d is: %d\n", j+1, scoreArray[j]);
        // }

        //here comes the sat
        int satChange;
        for(int step = 1; step <= maxSteps; ++step)
        {
            if(unsatisfied == 0)
            {
                // print the result
                // for(k = 0; k < varNum; ++k)
                // {
                //     printf("value for node %d is %d\n", 1+k, tempSol[k]);
                // }
                printf("check:%d\n", calcuUnsatisfied(CNFA, formulaNum, tempSol));
                printf("try:%d step:%d\n", i, step);
                return i;
            }

            // printf("\n\n---------------------------------------\n");
            satChange = filp(CNFA, formulaNum, tempSol, scoreArray, satStatus, FD, VD);
            unsatisfied -= satChange;
            // printf("current unsatisfied:%d\n", unsatisfied);

            // printf("\nconfirming var:\n");
            // for(j = 0; j < varNum; ++j)
            // {
            //     printf("value for var %d is: %d\n", j+1, tempSol[j]);
            // }

            // printf("\nconfirming sat status:\n");
            // for(j = 0; j < formulaNum; ++j)
            // {
            //     printf("status for clause %d is: %s\n", j+1, (satStatus[j] == 1)?"Yes":"No");
            // }

            // printf("\nconfirming score:\n");
            // for(j = 0; j < varNum; ++j)
            // {
            //     printf("score for node %d is: %d\n", j+1, scoreArray[j]);
            // }

            // getchar(); //for debug
        }
    }
    return 0;// “no solution found”;
}

void generateCNFArray(CNFPtr &F, int formulaNum, int CNFA[][varNum])
{
    int i, j;
    FormulaPtr currentFormula = F->formulas;
    VarPtr currentVar;

    for(i = 0; i < formulaNum; ++i)
        for(j = 0; j < varNum; ++j)
            CNFA[i][j] = -1;

    for(i = 0; i < formulaNum; ++i)
    {
        j = 0;
        currentVar = currentFormula->vars->next;
        while(currentVar != NULL)
        {
            // printf("checking var %d mode %d\n", currentVar->var_name, currentVar->var_mode);
            if(currentVar->var_mode == 0)
            {
                CNFA[i][currentVar->var_name - 1] = 0;
                // printf("set formu %d, var %d to 0!\n", i, currentVar->var_name);
            }
            else
            {
                CNFA[i][currentVar->var_name - 1] = 1;
                // printf("set formu %d, var %d to 1!\n", i, currentVar->var_name);
            }
            currentVar = currentVar->next;
        }
        currentFormula = currentFormula->next;
    }
}

int calcuUnsatisfied(int CNFA[][varNum], int formulaNum, int tempSol[])
{
    int i, j, result = 0, change = 0;
    int ruler = 1;
    for(i = 0; i < formulaNum; ++i)
    {
        result += 1;
        for(j = 0; j < varNum; ++j)
        {
            if(CNFA[i][j] != -1)
            {
                change = (tempSol[j] + CNFA[i][j]) & ruler;
                if(change == 0)
                {
                    result -= 1;
                    break;
                }
            }
        }
    }
    return result;
}

void generateScoreArray(int CNFA[][varNum], int formulaNum, int tempSol[],
                        int satStatus[], int scoreArray[])
{
    int i, j, status = 0, change, satisfieCount;
    int ruler = 1;
    for(i = 0; i < formulaNum; ++i)
    {
        status = 0; //initialize status of current formula: unsatisfied
        satisfieCount = 0; // initialize counter of statisfied vars
        for(j = 0; j < varNum; ++j)
        {
            if(CNFA[i][j] != -1)
            {
                change = (tempSol[j] + CNFA[i][j]) & ruler;
                if(change == 0)
                {
                    status = 1;
                    satisfieCount++;
                }
            }
        }

        if(status == 0) // current formula is unstatisfied
        {
            satStatus[i] = 0;
            //increase score of all dependent vars
            for(j = 0; j < varNum; ++j)
            {
                if(CNFA[i][j] != -1)
                {
                    scoreArray[j]++;
                }
            }
        }
        else if(status == 1 && satisfieCount == 1) // current formula is critically dependent on a var
        {
            satStatus[i] = 1;
            for(j = 0; j < varNum; ++j)
            {
                if(CNFA[i][j] != -1)
                {
                    change = (tempSol[j] + CNFA[i][j]) & ruler;
                    if(change == 0)
                    {
                        scoreArray[j]--;
                        break;
                    }
                }
            }
        }
        else
        {
            satStatus[i] = 1;
        }
    }

    // printf("score:\n");
    // for(i = 0; i < varNum; ++i)
    // {
    //     printf("%d\t", scoreArray[i]);
    // }
    // printf("\n");
}

void generateVarDependeceArray(int CNFA[][varNum], int formulaNum, int VD[][varNum])
{
    int i, j, k, varcount;
    int occur[varNum];

    for(i = 0; i < varNum; ++i)
        for(j = 0; j < varNum; ++j)
            VD[i][j] = 0;

    for(i = 0; i < formulaNum; ++i)
    {
        varcount = 0;
        for(j = 0; j < varNum; ++j)
            occur[j] = 0;

        for(j = 0; j < varNum; ++j)
        {
            if(CNFA[i][j] != -1)
            {
                occur[varcount] = j;
                varcount++;
                for(k = 1; k < varcount; k++)
                {
                    VD[occur[k-1]][j] = 1;
                    VD[j][occur[k-1]] = 1;
                    // printf("dependence:var %d and var %d\n", k, j+1);
                }
            }
        }
    }
}

void generateFormulaDependeceArray(int CNFA[][varNum], int formulaNum, int FD[][varNum])
{
    int i, j;

    for(i = 0; i < varNum; ++i)
        for(j = 0; j < varNum; ++j)
            FD[i][j] = 0;

    for(i = 0; i < formulaNum; ++i)
    {
        for(j = 0; j < varNum; ++j)
        {
            if(CNFA[i][j] != -1)
            {
                FD[i][j] = 1;
            }
        }
    }
}

int filp(int CNFA[][varNum], int formulaNum, int tempSol[], int SA[], int satStatus[],
            int FD[][varNum], int VD[][varNum])
{
    int i, maxScore = SA[0]-1, maxOrder = 0, sameCounter = 0;
    for(i = 0; i < varNum; ++i)
    {
        if(SA[i] > maxScore)
        {
            maxScore = SA[i];
            maxOrder = i;
            sameCounter = 1;
        }
        else if(SA[i] == maxScore)
        {
            if(rand()%(sameCounter+1) == 0)
            {
                maxScore = SA[i];
                maxOrder = i;
            }
            sameCounter++;
        }
    }

    // printf("flip confirmed: node %d from %d to %d\n", maxOrder+1, tempSol[maxOrder], !tempSol[maxOrder]);
    tempSol[maxOrder] = !tempSol[maxOrder];
    SA[maxOrder] = -SA[maxOrder];

    // maxScore is the change on num of unsat clause after the flip,
    // the value is the return value of the function

    //update sat status of dependent clause
    int j, change, ruler = 1, satisfyCount, status, lastSat;

    for(i = 0; i < formulaNum; ++i)
    {
        if(FD[i][maxOrder] == 1)    // find dependent clause
        {
            // printf("find dependent clause %d\n", i+1);
            // checking sat status of these dependent clause
            satisfyCount = 0;
            status = 0;
            lastSat = 0;
            for(j = 0; j < varNum; ++j)
            {
                if(CNFA[i][j] != -1)
                {
                    change = (tempSol[j] + CNFA[i][j]) & ruler;
                    if(change == 0)
                    {
                        status = 1;
                        satisfyCount++;
                        lastSat = j;
                    }
                }
            }

            if(status == 0)
            {
                // after the flip, the clause turns from cri.satisfied to unsatisfied
                // all other vars appear in this clause increase score
                for(j = 0; j < varNum; ++j)
                {
                    if(CNFA[i][j] != -1 && j != maxOrder)
                    {
                        SA[j]++;
                    }
                }
            }
            else if(status == 1 && satisfyCount == 1)
            {
                // after the flip, the clause turns to cri.satisfied
                // there are two possiblities: from unsat to cri.sat, from 2-sat to cri.sat
                // in the former situation, other appearing vars need to decrease score;
                // in the latter situation, only the cri.sat var need to decrease score

                // So we cancel the flip temporarily to check
                // if the clause used to be unsatisfied
                tempSol[maxOrder] = !tempSol[maxOrder];
                satisfyCount = 0;

                for(j = 0; j < varNum; ++j)
                {
                    if(CNFA[i][j] != -1)
                    {
                        change = (tempSol[j] + CNFA[i][j]) & ruler;
                        if(change == 0)
                        {
                            satisfyCount++;
                            if(j != maxOrder)
                                lastSat = j;        // use lastSat to store the original-sat var
                        }
                    }
                }

                if(satisfyCount == 0)
                {
                    // the clause used to be unsat.
                    // other appearing vars need to decrease score;
                    for(j = 0; j < varNum; ++j)
                    {
                        if(CNFA[i][j] != -1 && j != maxOrder)
                        {
                            SA[j]--;
                        }
                    }
                    // printf("unsat->sat clause NO.%d\n", i+1);
                }
                else if(satisfyCount == 2)
                // the clause used to be 2-sat.
                // only the cri.sat var need to decrease score
                    SA[lastSat]--;
                // else
                    // printf("wrong !!!!!!!!!\n");

                // redo the flip
                tempSol[maxOrder] = !tempSol[maxOrder];
            }
            else if(status == 1 && satisfyCount == 2)
            {
                // a possibility: the clause turns from critically satisfied to normal satisfied
                // in such case, score of the original critical var need to increase

                // There are 2 possiblity with status of 1 and satisfyCount of 2
                // one is that from 3-sat to 2-sat, one is from cri.sat to 2-sat.
                // So we cancel the flip temporarily to check
                // if the clause used to be critically satisfied
                tempSol[maxOrder] = !tempSol[maxOrder];
                satisfyCount = 0;

                for(j = 0; j < varNum; ++j)
                {
                    if(CNFA[i][j] != -1)
                    {
                        change = (tempSol[j] + CNFA[i][j]) & ruler;
                        if(change == 0)
                        {
                            satisfyCount++;
                            lastSat = j;
                        }
                    }
                }
                if(satisfyCount == 1)   //the clause used to be critically satisfied
                    SA[lastSat]++;

                // redo the flip
                tempSol[maxOrder] = !tempSol[maxOrder];
            }

            if(status == 0)
                satStatus[i] = 0;
            else
                satStatus[i] = 1;
        }
    }
    return maxScore;
}
