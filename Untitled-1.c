#include <stdio.h>
#include <math.h>
#include <time.h>

double functionValue(double x);
double firstDerivative(double x);
double secondDerivative(double x);
int checkInterval(double left, double right);
int continueCalculation(int *limit, int amount, int *unlimited);

double chordMethod(double left, double right, double epsilon,
                   int maxIterations, int debugMode,
                   int *iterations, int *rootFound);

double bisectionMethod(double left, double right, double epsilon,
                       int maxIterations, int debugMode,
                       int *iterations, int *rootFound);

int main(void)
{
    int method;
    int debugMode;
    int maxIterations;
    int iterations;
    int rootFound;
    double left;
    double right;
    double epsilon;
    double root;
    double calculationTime;
    clock_t startTime;
    clock_t endTime;

    printf("============================================================\n");
    printf(" LABORATORY WORK No. 3, PART 2. VARIANT 4\n");
    printf(" Equation: x^3 / 30 - 4*x^2 + 50 = 0\n");
    printf("============================================================\n");

    printf("1 - Chord method\n");
    printf("2 - Bisection method\n");
    printf("Choose a method: ");
    scanf("%d", &method);

    printf("Enter alpha: ");
    scanf("%lf", &left);

    printf("Enter beta: ");
    scanf("%lf", &right);

    printf("Enter epsilon: ");
    scanf("%lf", &epsilon);

    printf("Enter the maximum number of iterations: ");
    scanf("%d", &maxIterations);

    printf("Enable debug mode? (1 - yes, 0 - no): ");
    scanf("%d", &debugMode);

    if (method != 1 && method != 2)
    {
        printf("Incorrect method number.\n");
        return 1;
    }

    if (left >= right || epsilon <= 0.0 || maxIterations <= 0)
    {
        printf("Incorrect input data.\n");
        return 1;
    }

    if (!checkInterval(left, right))
    {
        printf("Choose another isolation interval.\n");
        return 1;
    }

    startTime = clock();

    if (method == 1)
    {

        root = chordMethod(left, right, epsilon, maxIterations,
                           debugMode, &iterations, &rootFound);
    }
    else
    {

        root = bisectionMethod(left, right, epsilon, maxIterations,
                               debugMode, &iterations, &rootFound);
    }

    endTime = clock();

    calculationTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("\n============================================================\n");

    if (rootFound)
    {
        printf("The root was found.\n");
    }
    else
    {
        printf("The calculation was stopped. Intermediate result:\n");
    }

    printf("Root x           = %.12f\n", root);
    printf("Function f(x)    = %.12e\n", functionValue(root));
    printf("Iterations       = %d\n", iterations);
    printf("Calculation time = %.9f seconds\n", calculationTime);
    printf("============================================================\n");

    return 0;
}

double functionValue(double x)
{

    return x * x * x / 30.0 - 4.0 * x * x + 50.0;
}

double firstDerivative(double x)
{
    return x * x / 10.0 - 8.0 * x;
}

double secondDerivative(double x)
{
    return x / 5.0 - 8.0;
}

int checkInterval(double left, double right)
{
    double fLeft = functionValue(left);
    double fRight = functionValue(right);
    int derivativeHasOneSign;

    printf("\nChecking the isolation interval:\n");
    printf("f(alpha)  = %.10f\n", fLeft);
    printf("f(beta)   = %.10f\n", fRight);
    printf("f'(alpha) = %.10f\n", firstDerivative(left));
    printf("f'(beta)  = %.10f\n", firstDerivative(right));

    derivativeHasOneSign = (right <= 0.0)
                           || (left >= 0.0 && right <= 80.0)
                           || (left >= 80.0);

    if (fLeft * fRight > 0.0)
    {
        printf("The function has the same sign at both endpoints.\n");
        return 0;
    }

    if (!derivativeHasOneSign)
    {
        printf("The derivative changes sign on the interval.\n");
        return 0;
    }

    printf("The function has opposite signs at the endpoints.\n");
    printf("The derivative does not change sign on the interval.\n");
    printf("The interval contains exactly one root.\n");

    return 1;
}

int continueCalculation(int *limit, int amount, int *unlimited)
{
    char choice;

    printf("\nThe maximum number of iterations has been reached.\n");
    printf("a - continue with the same number of iterations\n");
    printf("b - continue until the root is found\n");
    printf("c - exit and show the intermediate result\n");
    printf("Your choice: ");
    scanf(" %c", &choice);

    if (choice == 'a' || choice == 'A')
    {
        *limit = *limit + amount;
        return 1;
    }

    if (choice == 'b' || choice == 'B')
    {
        *unlimited = 1;
        return 1;
    }

    return 0;
}

double chordMethod(double left, double right, double epsilon,
                   int maxIterations, int debugMode,
                   int *iterations, int *rootFound)
{
    double fixedPoint;
    double currentPoint;
    double nextPoint;
    double fixedValue;
    double currentValue;
    int functionSign = 1;
    int limit = maxIterations;
    int unlimited = 0;

    *iterations = 0;
    *rootFound = 0;

    if (secondDerivative((left + right) / 2.0) < 0.0)
    {
        functionSign = -1;
    }

    if (functionSign * functionValue(left) > 0.0)
    {
        fixedPoint = left;
        currentPoint = right;
    }
    else
    {
        fixedPoint = right;
        currentPoint = left;
    }

    fixedValue = functionSign * functionValue(fixedPoint);

    if (debugMode == 1)
    {
        printf("\nChord method iterations:\n");
        printf(" No.          x                 f(x)\n");
    }

    while (fabs(functionValue(currentPoint)) > epsilon)
    {

        if (!unlimited && *iterations >= limit)
        {

            if (!continueCalculation(&limit, maxIterations, &unlimited))
            {
                return currentPoint;
            }
        }

        currentValue = functionSign * functionValue(currentPoint);

        nextPoint = currentPoint
                    - currentValue * (currentPoint - fixedPoint)
                    / (currentValue - fixedValue);

        currentPoint = nextPoint;
        (*iterations)++;

        if (debugMode == 1)
        {
            printf("%4d  %18.10f  %18.10e\n",
                   *iterations, currentPoint, functionValue(currentPoint));
        }
    }

    *rootFound = 1;
    return currentPoint;
}

double bisectionMethod(double left, double right, double epsilon,
                       int maxIterations, int debugMode,
                       int *iterations, int *rootFound)
{
    double middle = (left + right) / 2.0;
    double middleValue;
    int limit = maxIterations;
    int unlimited = 0;

    *iterations = 0;
    *rootFound = 0;

    if (debugMode == 1)
    {
        printf("\nBisection method iterations:\n");
        printf(" No.          left              right             middle            f(middle)\n");
    }

    while ((right - left) / 2.0 > epsilon)
    {

        if (!unlimited && *iterations >= limit)
        {

            if (!continueCalculation(&limit, maxIterations, &unlimited))
            {
                return middle;
            }
        }

        middle = (left + right) / 2.0;
        middleValue = functionValue(middle);
        (*iterations)++;

        if (debugMode == 1)
        {
            printf("%4d  %18.10f  %18.10f  %18.10f  %18.10e\n",
                   *iterations, left, right, middle, middleValue);
        }

        if (middleValue == 0.0)
        {
            *rootFound = 1;
            return middle;
        }

        if (functionValue(left) * middleValue < 0.0)
        {
            right = middle;
        }
        else
        {
            left = middle;
        }
    }

    middle = (left + right) / 2.0;
    *rootFound = 1;
    return middle;
}

