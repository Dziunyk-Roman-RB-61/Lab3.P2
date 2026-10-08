#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>

double math_function(double x); // 2*pow(x,3)-0.2*pow(x,2)+0.5*x+3

double derivative_1_math_funct (double x); // перша похідна функції
                                                 // аналітична

double derivative_2_math_funct (double x); // друга похідна функції
                                                 // аналітична

double digit_deriv_1_math_left (double x, double h); // перша похідна функції
                                                               // чисельна

double digit_deriv_1_math_r (double x, double h); // перша похідна функції
                                                            // чисельна

double digit_deriv_2_math_funct (double x, double h); // друга похідна
                                                                // функції

void input_for_der(double *x, double *h);

void result(double f);

int main()
{
    for(;;)
    {
        double x, h;
        unsigned int var;

        printf("1 -  original der.\n2 -  der. digit left \n3 -  der. r c.\n");
        scanf("%d",&var);

        switch(var)
        {
            case 1:
                printf("Input x\n");
                scanf("%lf",&x);
                printf("x = %lf\n",x);
                result(derivative_1_math_funct(x));
            break;

            case 2:
                input_for_der(&x, &h);
                result(digit_deriv_1_math_left(x, h));
            break;

            case 3:
                input_for_der(&x, &h);
                result(digit_deriv_1_math_r(x, h));
            break;
        }
    }

    return 0;
}

double math_function(double x)// 2*pow(x,3)-0.2*pow(x,2)+0.5*x+3
{
    return 2*pow(x,3)-0.2*pow(x,2)+0.5*x+3;
}

double derivative_1_math_funct (double x) // перша похідна функції
{
    return  6*pow(x,2)-0.4*x+0.5 ;
}

double derivative_2_math_funct (double x) // друга похідна функції
{
    return 12*x-0.4;
}

double digit_deriv_1_math_left (double x, double h)
{
    // Точка для похідної
    // Крок з яким обчислюється похідна
    // 1e-10 Derivative error
    if (h<1e-10)
        return ((math_function(x) - math_function(x-h))/h);
    else
        printf("Too much value for h\n");

    return 1;
}

double digit_deriv_1_math_r (double x, double h)
{
    // 1e-10 Derivative error
    if (h<1e-10)
        return ((math_function(x+h) - math_function(x))/h);
    else
        printf("Too much value for h\n");

    return 1;
}

double digit_deriv_2_math_funct (double x, double h) // друга похідна функції
{
    ;
}

void input_for_der(double *x, double *h)
{
    printf("Input x, h 0.00000000001\n");
    scanf("%lf",x);
    scanf("%lf",h);
    printf("x = %lf\n",*x);
    printf("h  = %lf\n",*h);
}

void result(double f)
{
    printf("Result der 1 is = %lf\n",f);
}

