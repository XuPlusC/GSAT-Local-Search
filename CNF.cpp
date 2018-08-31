#include <stdio.h>
#include <stdlib.h>
#include "CNF.h"

int constructCNF(CNFPtr &F, char *filepath)
{
    int formula_num, var_num;
    FILE *fp = fopen(filepath, "r");
    if(fp == NULL)
        return 0;
	fscanf(fp, "%d%d",  &var_num, &formula_num);

    CNFPtr headCNF = (CNFPtr)malloc(sizeof(CNF));
    headCNF->formula_num = formula_num;
    headCNF->formulas = (FormulaPtr)malloc( sizeof(formula));

    FormulaPtr currentFormula = headCNF->formulas;
    currentFormula->vars = (VarPtr)malloc(sizeof(var));
    VarPtr currentVar = currentFormula->vars;

    char readChar;
    fscanf(fp, "%c", &readChar);    //read the additional '\n'
    int local_var_num = 0, local_var_mode = 1;
    int searchingNextNumber = 1;
    while(1)
    {
        fscanf(fp, "%c", &readChar);
        if(feof(fp))
        {
            // if(!local_var_mode)
            // {
            //     printf("found neg var! ~%d\n", local_var_num);
            // }
            // else
            // {
            //     printf("found pos var! %d\n", local_var_num);   
            // }
            currentVar->var_name = local_var_num;
            currentVar->var_mode = local_var_mode;
            currentVar->next = NULL;
            searchingNextNumber = 1;
            break;
        }
        // printf("\'%c\' read!   ", readChar);
        if(readChar == '\n')
        {
            if(!local_var_mode)
            {
                // printf("found neg var! ~%d\n", local_var_num);
            }
            else
            {
                // printf("found pos var! %d\n", local_var_num);   
            }
            currentVar->var_name = local_var_num;
            currentVar->var_mode = local_var_mode;
            currentVar->next = NULL;
            searchingNextNumber = 1;

            // printf("this line read:");
            currentVar = currentFormula->vars->next;
            while(currentVar != NULL)
            {
                // if(!currentVar->var_mode)
                //     printf("~");
                // printf("%d\t", currentVar->var_name);
                currentVar = currentVar->next;
            }
            // printf("\n");

            currentFormula->next = (FormulaPtr)malloc(sizeof(formula));
            currentFormula = currentFormula->next;
            currentFormula->vars = (VarPtr)malloc(sizeof(var));
            currentVar = currentFormula->vars;
            // the first var is set as an empty headnode
            //currentVar = (var*)malloc(sizeof(var));
            currentVar->next = NULL;
            //reset the local variables
            local_var_num = 0;
            local_var_mode = 1;
        }
        else if(readChar == '~')
        {
            local_var_mode = 0;
        }
        else if((readChar >= '0' && readChar <= '9'))
        {
            if(searchingNextNumber)
            {
                currentVar->next = (VarPtr)malloc(sizeof(var));
                currentVar = currentVar->next;
                searchingNextNumber = 0;
            }
            local_var_num *= 10;
            local_var_num += readChar - '0';
        }
        else
        {
            if(searchingNextNumber)
                continue;
            else 
            {
                // if(!local_var_mode)
                // {
                //     printf("found neg var! ~%d\n", local_var_num);
                // }
                // else
                // {
                //     printf("found pos var! %d\n", local_var_num);   
                // }
                currentVar->var_name = local_var_num;
                currentVar->var_mode = local_var_mode;
                currentVar->next = NULL;
                //turn on the "switch". The program will not meaninglessly set value for currentVar.
                //until the system read the next number in file.                

                searchingNextNumber = 1;

                //reset the local variables
                local_var_num = 0;
                local_var_mode = 1;
            }            
        }
    }

    // printf("\nthis line read:");
    // currentVar = currentFormula->vars->next;
    // while (currentVar != NULL)
    // {
    //     if (!currentVar->var_mode)
    //         printf("~");
    //     printf("%d\t", currentVar->var_name);
    //     currentVar = currentVar->next;
    // }
    // printf("\n");
    F = headCNF;
    return formula_num;
}
