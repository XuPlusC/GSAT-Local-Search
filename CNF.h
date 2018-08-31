#ifndef CNF_H_INCLUDED
#define CNF_H_INCLUDED

typedef struct var
{
    //int var_name;
    int var_name;
    int var_mode;   //0 for ~F, 1 for F
    struct var *next;
}var, *VarPtr;

typedef struct formula
{
    //int var_num;
    var *vars;
    struct formula *next;
}formula, *FormulaPtr;

typedef struct CNF
{
    int formula_num;
    formula *formulas;
}CNF, *CNFPtr;

int constructCNF(CNFPtr &F, char *filepath);

#endif // CNF_H_INCLUDED

