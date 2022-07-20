/*============================ LIBRARIES && MACROS =============================*/
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "stack.h"
#include "calc.h"

#define SIZE 100
#define NUM_EVENT 1
#define SIZE_EVENT 9
#define ASCII_SIZE 128
#define ZERO_DIVISION 0xBADC0DE

/*============================== TYPEDEFS && ENUMS ==================================*/

typedef enum
{
    wait_for_num_state,
    wait_for_op_state,
    error_state,
    last_state
}state_t;

typedef enum
{
    recieve_plus_event = '+',
    recieve_minus_event = '-',
    recieve_multi_event = '*',
    recieve_div_event = '/',
    recieve_pow_event = '^',
    recieve_close_par_event = ')' ,
    recieve_open_par_event = '(',
    space_event = ' ',
    end_of_string_event = '\0'
} event_t;

typedef enum math
{
    PLUS = 1,
    MINUS = 1,
    MULTI = 2,
    DIVSION = 2,
    POW = 3
} math_priority_t;

typedef double (*MathAction)(double num1, double num2);
typedef state_t (*EventHandler[last_state][ASCII_SIZE])(calc_t *calc,char **data, double *res);

typedef struct Op
{
    MathAction action;
    math_priority_t priority;
}op_t;

struct calc
{
    state_t curr_state;
    struct stack *num_stack;
    struct stack *op_stack;
    EventHandler state_machine;
    op_t op_action_lut[ASCII_SIZE];
};


/*========================== FUNCTION DECLARATION ============================*/
/*init luts*/
static void InitEventHandler(EventHandler state_machine);
static void InitOpLut(op_t op_action_lut[ASCII_SIZE]);


/*handlers*/
static state_t SpaceHandler(calc_t *calc, char **expression, double *res);
static state_t RecieveNumHandler(calc_t *calc, char **expression, double *res);
static state_t RecieveOpHandler(calc_t *calc, char **expression, double *res);
static state_t RecieveCloseParHandler(calc_t *calc, char **expression, double *res);
static state_t RecieveOpenParHandler(calc_t *calc, char **expression, double *res);
static state_t EndOfStringHandler(calc_t *calc, char **expression, double *res);
static state_t ErrorHandler(calc_t *calc, char **expression, double *res);

/*math*/
static double CalcNums(calc_t *calc);
static double Multiplication(double num1, double num2);
static double Division(double num1, double num2);
static double Sum(double num1, double num2);
static double Subt(double num1, double num2);
static double Pow(double base, double exp);

/*STATUS*/
static calculator_status_t CalcStatus(calc_t *calc);
/*========================= FUNCTION DEFINITION =========================*/
calc_t *CalcCreate()
{
    calc_t *calc = (calc_t *)malloc(sizeof(calc_t));
    calc->curr_state = wait_for_num_state;

    calc->num_stack = StackCreate(SIZE, sizeof(double));
    if(NULL == calc->num_stack)
    {
        free(calc);
        return (NULL);
    }

    calc->op_stack = StackCreate(SIZE, sizeof(char));
    if (NULL == calc->op_stack)
    {
        free(calc->num_stack);
        free(calc);
        return (NULL);
    }

    InitEventHandler(calc->state_machine); 
    InitOpLut(calc->op_action_lut); 


    return (calc);
}

void CalcDestroy(calc_t *calc)
{
    StackDestroy(calc->num_stack);
    StackDestroy(calc->op_stack);
    free(calc);
}

calculator_status_t Calculate(calc_t *calc, const char *expression, double *res)
{
    assert(NULL != calc);
    assert(NULL != expression);
    assert(NULL != res);

    while (calc->curr_state != last_state && calc->curr_state != error_state)
    {
        calc->curr_state = (*calc->state_machine[calc->curr_state][(int)*expression])(calc, (char **)&expression, res);
    }
    
    
    return (CalcStatus(calc));
}


static state_t RecieveNumHandler(calc_t *calc, char **expression, double *res)
{
    double num = strtod(*expression, expression);
    (void)res;
    StackPush(calc->num_stack, &num);
    return (wait_for_op_state);
}


static state_t RecieveOpHandler(calc_t *calc, char **expression, double *res)
{
    double num;
    (void)res;

    while(!StackIsEmpty(calc->op_stack)
            && calc->op_action_lut[(int)(**expression)].priority <= calc->op_action_lut[(int)*(char*)StackPeek(calc->op_stack)].priority)
    {
        num = CalcNums(calc); 
        if(num == ZERO_DIVISION)
        {
            return (error_state);
        }
    }
    StackPush(calc->op_stack, *expression);
    ++(*expression);
    return (wait_for_num_state);
}


static state_t RecieveCloseParHandler(calc_t *calc, char **expression, double *res)
{
    double num;
    (void)res;

    while (!StackIsEmpty(calc->op_stack) && *(char *)StackPeek(calc->op_stack) != '(')
    {
        num = CalcNums(calc);
        if(num == ZERO_DIVISION)
        {
            return (error_state);
        }
    }

    /*when we get ')'but no '(' exist*/
    if (StackIsEmpty(calc->op_stack))
    {
        return (error_state);
    }

    StackPop(calc->op_stack);
    ++(*expression);
    return (wait_for_op_state);
}


static state_t RecieveOpenParHandler(calc_t *calc, char **expression, double *res)
{
    (void)res;
    StackPush(calc->op_stack, *expression);
    ++(*expression);
    return (wait_for_num_state);
}


static state_t EndOfStringHandler(calc_t *calc, char **expression, double *res)
{
    (void)expression;

    if(StackGetSize(calc->num_stack) - StackGetSize(calc->op_stack) != 1)
    {
        return (error_state);
    }
   while(StackGetSize(calc->num_stack) > 1)
   {
        if(CalcNums(calc) == ZERO_DIVISION)
        {
            return (error_state);
        }
   }
    *res = *(double*)StackPop(calc->num_stack);

    return (last_state);
}


static state_t ErrorHandler(calc_t *calc, char **expression, double *res)
{
    (void)expression;
    (void)res;
    (void)calc;
    return (error_state);
}


static state_t SpaceHandler(calc_t *calc, char **expression, double *res)
{
    (void)res;
    while(**expression == ' ')
    {
        ++(*expression);
    }

    return (calc->curr_state);
}


static double CalcNums(calc_t *calc)
{
    double num1 = 0, num2 = 0, ans = 0;
    unsigned char op = ' ';

    op = *(char *)StackPop(calc->op_stack);

    num2 = *(double *)StackPop(calc->num_stack);
    num1 = *(double *)StackPop(calc->num_stack);
    
    ans = calc->op_action_lut[op].action(num1, num2);
    
    StackPush(calc->num_stack, (void *)&ans);
    return (ans);   
}



static calculator_status_t CalcStatus(calc_t *calc)
{
    if (calc->curr_state == error_state)
    {
        if(!StackIsEmpty(calc->num_stack) && *(double*)StackPeek(calc->num_stack) == ZERO_DIVISION)
        {
            return (MATH_ERROR);
        }
        return (SYNTAX_ERROR);
    }
    return (SUCCESS);
}


static void InitEventHandler(EventHandler state_machine)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < last_state; ++i)
    {
        for (j = 0; j < ASCII_SIZE; ++j)
        {
            state_machine[i][j] = ErrorHandler;  
        }
        
    }
    

    for (i = '0'; i <= '9'; ++i)
    {
        state_machine[wait_for_num_state][i] = RecieveNumHandler;
    }

    state_machine[wait_for_num_state][recieve_open_par_event] = RecieveOpenParHandler;
    state_machine[wait_for_num_state][recieve_plus_event] = RecieveNumHandler;
    state_machine[wait_for_num_state][recieve_minus_event] = RecieveNumHandler;
    state_machine[wait_for_num_state][space_event] = SpaceHandler;
    state_machine[wait_for_num_state][end_of_string_event] = EndOfStringHandler;

    state_machine[wait_for_op_state][recieve_plus_event] = RecieveOpHandler;
    state_machine[wait_for_op_state][recieve_minus_event] = RecieveOpHandler;
    state_machine[wait_for_op_state][recieve_multi_event] = RecieveOpHandler;
    state_machine[wait_for_op_state][recieve_div_event] = RecieveOpHandler;
    state_machine[wait_for_op_state][recieve_pow_event] = RecieveOpHandler;
    state_machine[wait_for_op_state][recieve_open_par_event] = RecieveOpenParHandler;
    state_machine[wait_for_op_state][recieve_close_par_event] = RecieveCloseParHandler;
    state_machine[wait_for_op_state][space_event] = SpaceHandler;
    state_machine[wait_for_op_state][end_of_string_event] = EndOfStringHandler;
}

static void InitOpLut(op_t op_action_lut[ASCII_SIZE])
{
    int i = 0;
    for (i = 0; i < ASCII_SIZE; ++i)
    {
        op_action_lut[i].action = NULL;
        op_action_lut[i].priority = 0;
    }
    
    op_action_lut['+'].action = Sum;
    op_action_lut['+'].priority = PLUS;

    op_action_lut['-'].action = Subt;
    op_action_lut['-'].priority = MINUS;

    op_action_lut['*'].action = Multiplication;
    op_action_lut['*'].priority = MULTI;

    op_action_lut['/'].action = Division;
    op_action_lut['/'].priority = DIVSION;

    op_action_lut['^'].action = Pow;
    op_action_lut['^'].priority = POW;
}


static double Multiplication(double num1, double num2)
{
    return (num1 * num2);
}

static double Division(double num1, double num2)
{
    return (num2 == 0)? ZERO_DIVISION : (num1 / num2);
}

static double Sum(double num1, double num2)
{
    return (num1 + num2);
}

static double Subt(double num1, double num2)
{
    return (num1 - num2);
}

static double Pow(double base, double exp)
{
    double sum = 1;
    size_t i = 0;

    if (exp <= 0)
    {
        base = 1/base;
        exp *= -1;
    }
    for (i = 0; i < exp; ++i)
    {
        sum *= base;
    }

    return (sum);
}