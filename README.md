# Calculator state machine

## Introduction

> This library can be used to calculate simple math problems.

## Code Samples

> char str[100] = "7+8"; </br>
> double res = 0;</br>
> calc_t \*calc = CalcCreate();</br>
> res = 0;</br>
> printf("status : %d need to be: %d\n",Calculate(calc, str, &res), SUCCESS);</br>
> printf("%d\n",res);</br>
> CalcDestroy(calc);</br>
> will print 15 on the terminal

## Installation

> comlie using the stack impl that is given + the calc.c file + your main file. </br>
> template for script can be download too (compile.sh).
