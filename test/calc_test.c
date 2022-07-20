#include <stdio.h>

#include "calc.h"

void TestAllFuncs(void);

int main(void)
{
	TestAllFuncs();
	return 0;
}


void TestAllFuncs(void)
{
    char str[100] = "7+8";                         
    char str2[100] = "8+8*3+-2^5";                   
    char str3[100] = "8+8*3-2^";                
    char str4[100] = "2/0";                      
    char str5[100] = "8++8*((3-2)*5)";
    char str6[100] = "3-2)*5";                 
    char str7[100] = "(3-2)*5+5*(4+4+4";
    
    double res = 0;
    calc_t *calc = CalcCreate();

    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str, &res), SUCCESS);
    printf("%d\n",15 == res);
    CalcDestroy(calc);

    calc = CalcCreate();
    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str2, &res), SUCCESS);
    printf("%d\n",0 == res);
    CalcDestroy(calc);
    
    calc = CalcCreate();
    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str3, &res), SYNTAX_ERROR);
    printf("%d\n",0 == res);
    CalcDestroy(calc);

    calc = CalcCreate();
    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str4, &res), MATH_ERROR);
    printf("%d\n",0 == res);
    CalcDestroy(calc);

    /*enter here*/
    calc = CalcCreate();
    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str5, &res), SUCCESS);
    printf("%d\n",48 == res);
    CalcDestroy(calc);

    calc = CalcCreate();
    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str6, &res), SYNTAX_ERROR);
    printf("%d\n",0 == res);
    CalcDestroy(calc);

    calc = CalcCreate();
    res = 0;
    printf("status : %d need to be: %d\n",Calculate(calc, str7, &res), SYNTAX_ERROR);
    printf("%d \n",0 == res);
    CalcDestroy(calc);
}