#ifndef __CALC_H_
#define __CALC_H_
/**
 * @brief return status values and meanning
 *
 */
typedef enum
{
    SUCCESS,
    MATH_ERROR,
    SYNTAX_ERROR
} calculator_status_t;

typedef struct calc calc_t;

/**
 * @brief Create and alloc resources for the calculator.
 *
 * @return calc_t* A pointer to the Calculator.
 */
calc_t *CalcCreate();

/**
 * @brief Destroy and realase all the allocated memory of the calculator.
 *
 * @param calc Pointer to the Calculator.
 */
void CalcDestroy(calc_t *calc);

/**
 * @brief The main function of the library, with this we will calculate
 *        the desired formula.
 *
 * @param calc Pointer to the Calculator.
 * @param expression A string to evaluate its value.
 * @param res A pointer to the address to save the value in.
 * @return calculator_status_t -> can be SUCCESS, MATH_ERROR, SYNTAX_ERROR.
 */
calculator_status_t Calculate(calc_t *calc, const char *expression, double *res);

#endif /*__CALC_H_*/