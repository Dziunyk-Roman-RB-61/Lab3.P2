#include <stdio.h>  // Бібліотека для введення даних функцією scanf() і виведення функцією printf()
#include <math.h>   // Бібліотека для математичної функції fabs()
#include <time.h>   // Бібліотека для визначення часу обчислення функцією clock()

// Оголошення власних функцій програми
double functionValue(double x);          // Обчислення значення заданої функції f(x)
double firstDerivative(double x);        // Обчислення першої похідної f'(x)
double secondDerivative(double x);       // Обчислення другої похідної f''(x)
int checkInterval(double left, double right); // Перевірка проміжку ізоляції кореня
int continueCalculation(int *limit, int amount, int *unlimited); // Вибір дії після досягнення ліміту

// Функція розв'язання рівняння методом хорд
double chordMethod(double left, double right, double epsilon,
                   int maxIterations, int debugMode,
                   int *iterations, int *rootFound);

// Функція розв'язання рівняння методом половинного ділення
double bisectionMethod(double left, double right, double epsilon,
                       int maxIterations, int debugMode,
                       int *iterations, int *rootFound);

// Основна функція програми
int main(void)
{
    int method;             // Номер обраного методу
    int debugMode;          // Ознака режиму налагодження: 1 - увімкнено, 0 - вимкнено
    int maxIterations;      // Максимальна кількість ітерацій
    int iterations;         // Кількість виконаних ітерацій
    int rootFound;          // Ознака знаходження кореня: 1 - знайдено, 0 - не знайдено
    double left;            // Ліва межа проміжку alpha
    double right;           // Права межа проміжку beta
    double epsilon;         // Задана похибка обчислення
    double root;            // Наближене значення кореня
    double calculationTime; // Час пошуку кореня в секундах
    clock_t startTime;      // Час початку обчислення
    clock_t endTime;        // Час завершення обчислення

    // Виведення назви лабораторної роботи та заданого рівняння
    printf("============================================================\n"); // Верхня межа заголовка
    printf(" LABORATORY WORK No. 3, PART 2. VARIANT 4\n"); // Назва роботи
    printf(" Equation: x^3 / 30 - 4*x^2 + 50 = 0\n"); // Рівняння четвертого варіанта
    printf("============================================================\n"); // Нижня межа заголовка

    // Виведення меню вибору методу
    printf("1 - Chord method\n"); // Перший пункт меню
    printf("2 - Bisection method\n"); // Другий пункт меню
    printf("Choose a method: "); // Запрошення до вибору методу
    scanf("%d", &method); // Введення номера методу

    printf("Enter alpha: "); // Запрошення до введення лівої межі
    scanf("%lf", &left); // Введення лівої межі проміжку

    printf("Enter beta: "); // Запрошення до введення правої межі
    scanf("%lf", &right); // Введення правої межі проміжку

    printf("Enter epsilon: "); // Запрошення до введення похибки
    scanf("%lf", &epsilon); // Введення похибки обчислення

    printf("Enter the maximum number of iterations: "); // Запрошення до введення ліміту
    scanf("%d", &maxIterations); // Введення максимальної кількості ітерацій

    printf("Enable debug mode? (1 - yes, 0 - no): "); // Запит режиму налагодження
    scanf("%d", &debugMode); // Введення режиму налагодження

    // Перевірка номера обраного методу
    if (method != 1 && method != 2)
    {
        printf("Incorrect method number.\n"); // Повідомлення про помилку
        return 1; // Завершення програми з кодом помилки
    }

    // Перевірка меж проміжку, похибки та кількості ітерацій
    if (left >= right || epsilon <= 0.0 || maxIterations <= 0)
    {
        printf("Incorrect input data.\n"); // Повідомлення про помилкові дані
        return 1; // Завершення програми з кодом помилки
    }

    // Перевірка наявності одного кореня на заданому проміжку
    if (!checkInterval(left, right))
    {
        printf("Choose another isolation interval.\n"); // Потрібно ввести інший проміжок
        return 1; // Завершення програми з кодом помилки
    }

    startTime = clock(); // Запам'ятовування часу перед початком пошуку кореня

    // Вибір методу відповідно до введеного номера
    if (method == 1)
    {
        // Виклик функції методу хорд
        root = chordMethod(left, right, epsilon, maxIterations,
                           debugMode, &iterations, &rootFound);
    }
    else
    {
        // Виклик функції методу половинного ділення
        root = bisectionMethod(left, right, epsilon, maxIterations,
                               debugMode, &iterations, &rootFound);
    }

    endTime = clock(); // Запам'ятовування часу після завершення пошуку кореня

    // Переведення різниці показників clock() у секунди
    calculationTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;

    printf("\n============================================================\n"); // Початок блока результатів

    // Перевірка результату обчислення
    if (rootFound)
    {
        printf("The root was found.\n"); // Корінь знайдено із заданою похибкою
    }
    else
    {
        printf("The calculation was stopped. Intermediate result:\n"); // Виведення проміжного результату
    }

    printf("Root x           = %.12f\n", root); // Виведення значення кореня
    printf("Function f(x)    = %.12e\n", functionValue(root)); // Виведення функції в точці кореня
    printf("Iterations       = %d\n", iterations); // Виведення кількості ітерацій
    printf("Calculation time = %.9f seconds\n", calculationTime); // Виведення часу обчислення
    printf("============================================================\n"); // Кінець блока результатів

    return 0; // Нормальне завершення програми
}

// Опис функції для обчислення f(x) = x^3 / 30 - 4x^2 + 50
double functionValue(double x)
{
    // Тип double дає змогу працювати з дійсними числами
    return x * x * x / 30.0 - 4.0 * x * x + 50.0; // Повернення значення f(x)
}

// Опис функції для обчислення першої похідної f'(x) = x^2 / 10 - 8x
double firstDerivative(double x)
{
    return x * x / 10.0 - 8.0 * x; // Повернення значення першої похідної
}

// Опис функції для обчислення другої похідної f''(x) = x / 5 - 8
double secondDerivative(double x)
{
    return x / 5.0 - 8.0; // Повернення значення другої похідної
}

// Опис функції перевірки проміжку ізоляції кореня
int checkInterval(double left, double right)
{
    double fLeft = functionValue(left);   // Значення функції на лівій межі
    double fRight = functionValue(right); // Значення функції на правій межі
    int derivativeHasOneSign;             // Ознака незмінності знака похідної

    // Виведення значень функції та похідної на межах проміжку
    printf("\nChecking the isolation interval:\n"); // Заголовок перевірки проміжку
    printf("f(alpha)  = %.10f\n", fLeft); // Значення функції на лівій межі
    printf("f(beta)   = %.10f\n", fRight); // Значення функції на правій межі
    printf("f'(alpha) = %.10f\n", firstDerivative(left)); // Похідна на лівій межі
    printf("f'(beta)  = %.10f\n", firstDerivative(right)); // Похідна на правій межі

    // f'(x) = x(x - 80) / 10; знак похідної змінюється в точках 0 і 80
    derivativeHasOneSign = (right <= 0.0)
                           || (left >= 0.0 && right <= 80.0)
                           || (left >= 80.0);

    // На кінцях проміжку функція повинна мати протилежні знаки
    if (fLeft * fRight > 0.0)
    {
        printf("The function has the same sign at both endpoints.\n"); // Корінь не підтверджено
        return 0; // Повернення хибного результату перевірки
    }

    // Перша похідна не повинна змінювати знак на проміжку
    if (!derivativeHasOneSign)
    {
        printf("The derivative changes sign on the interval.\n"); // На проміжку може бути кілька коренів
        return 0; // Повернення хибного результату перевірки
    }

    // Виконання обох умов підтверджує наявність одного кореня
    printf("The function has opposite signs at the endpoints.\n"); // Перша умова виконується
    printf("The derivative does not change sign on the interval.\n"); // Друга умова виконується
    printf("The interval contains exactly one root.\n"); // Підтвердження одного кореня

    return 1; // Повернення успішного результату перевірки
}

// Опис функції вибору дії після досягнення максимальної кількості ітерацій
int continueCalculation(int *limit, int amount, int *unlimited)
{
    char choice; // Символ обраного варіанта: a, b або c

    // Виведення повідомлення та можливих варіантів дій
    printf("\nThe maximum number of iterations has been reached.\n"); // Повідомлення про ліміт
    printf("a - continue with the same number of iterations\n"); // Варіант a
    printf("b - continue until the root is found\n"); // Варіант b
    printf("c - exit and show the intermediate result\n"); // Варіант c
    printf("Your choice: "); // Запрошення до введення символу
    scanf(" %c", &choice); // Введення символу з пропуском перед %c для пропуску Enter

    // Варіант a: додавання такої самої кількості ітерацій
    if (choice == 'a' || choice == 'A')
    {
        *limit = *limit + amount; // Збільшення загального ліміту ітерацій
        return 1; // Продовження обчислення
    }

    // Варіант b: скасування обмеження кількості ітерацій
    if (choice == 'b' || choice == 'B')
    {
        *unlimited = 1; // Установлення режиму без обмеження
        return 1; // Продовження обчислення
    }

    return 0; // Варіант c: завершення обчислення
}

// Опис функції розв'язання рівняння методом хорд
double chordMethod(double left, double right, double epsilon,
                   int maxIterations, int debugMode,
                   int *iterations, int *rootFound)
{
    double fixedPoint;   // Нерухомий кінець хорди
    double currentPoint; // Поточне наближення до кореня
    double nextPoint;    // Наступне наближення до кореня
    double fixedValue;   // Значення функції в нерухомій точці
    double currentValue; // Значення функції в поточній точці
    int functionSign = 1; // Множник для зміни знака функції
    int limit = maxIterations; // Поточний ліміт ітерацій
    int unlimited = 0; // Ознака роботи без обмеження кількості ітерацій

    *iterations = 0; // Початкове значення лічильника ітерацій
    *rootFound = 0;  // На початку корінь ще не знайдено

    // Якщо f''(x) < 0, відповідно до методички розглядається функція -f(x)
    if (secondDerivative((left + right) / 2.0) < 0.0)
    {
        functionSign = -1; // Зміна знака функції
    }

    // Вибір нерухомого кінця хорди за знаком функції
    if (functionSign * functionValue(left) > 0.0)
    {
        fixedPoint = left;   // Ліва межа залишається нерухомою
        currentPoint = right; // Початкове наближення дорівнює правій межі
    }
    else
    {
        fixedPoint = right; // Права межа залишається нерухомою
        currentPoint = left; // Початкове наближення дорівнює лівій межі
    }

    // Обчислення значення функції в нерухомій точці
    fixedValue = functionSign * functionValue(fixedPoint);

    // Виведення заголовка таблиці в режимі налагодження
    if (debugMode == 1)
    {
        printf("\nChord method iterations:\n"); // Назва таблиці ітерацій
        printf(" No.          x                 f(x)\n"); // Назви стовпців таблиці
    }

    // Повторення обчислень, поки значення |f(x)| перевищує задану похибку
    while (fabs(functionValue(currentPoint)) > epsilon)
    {
        // Перевірка досягнення максимальної кількості ітерацій
        if (!unlimited && *iterations >= limit)
        {
            // Запит подальшої дії; значення 0 означає завершення обчислення
            if (!continueCalculation(&limit, maxIterations, &unlimited))
            {
                return currentPoint; // Повернення проміжного результату
            }
        }

        // Обчислення значення функції в поточній точці
        currentValue = functionSign * functionValue(currentPoint);

        // Обчислення наступного наближення за формулою методу хорд
        nextPoint = currentPoint
                    - currentValue * (currentPoint - fixedPoint)
                    / (currentValue - fixedValue);

        currentPoint = nextPoint; // Перехід до наступного наближення
        (*iterations)++;          // Збільшення лічильника ітерацій на одиницю

        // Виведення номера ітерації, наближення та значення функції
        if (debugMode == 1)
        {
            printf("%4d  %18.10f  %18.10e\n",
                   *iterations, currentPoint, functionValue(currentPoint));
        }
    }

    *rootFound = 1; // Корінь знайдено із заданою похибкою
    return currentPoint; // Повернення знайденого кореня
}

// Опис функції розв'язання рівняння методом половинного ділення
double bisectionMethod(double left, double right, double epsilon,
                       int maxIterations, int debugMode,
                       int *iterations, int *rootFound)
{
    double middle = (left + right) / 2.0; // Середина початкового проміжку
    double middleValue; // Значення функції в середині проміжку
    int limit = maxIterations; // Поточний ліміт ітерацій
    int unlimited = 0; // Ознака роботи без обмеження кількості ітерацій

    *iterations = 0; // Початкове значення лічильника ітерацій
    *rootFound = 0;  // На початку корінь ще не знайдено

    // Виведення заголовка таблиці в режимі налагодження
    if (debugMode == 1)
    {
        printf("\nBisection method iterations:\n"); // Назва таблиці ітерацій
        printf(" No.          left              right             middle            f(middle)\n"); // Назви стовпців
    }

    // Продовження обчислень, поки половина проміжку перевищує похибку
    while ((right - left) / 2.0 > epsilon)
    {
        // Перевірка досягнення максимальної кількості ітерацій
        if (!unlimited && *iterations >= limit)
        {
            // Запит подальшої дії; значення 0 означає завершення обчислення
            if (!continueCalculation(&limit, maxIterations, &unlimited))
            {
                return middle; // Повернення проміжного результату
            }
        }

        middle = (left + right) / 2.0; // Обчислення середини поточного проміжку
        middleValue = functionValue(middle); // Обчислення функції в середній точці
        (*iterations)++; // Збільшення лічильника ітерацій на одиницю

        // Виведення даних поточної ітерації в режимі налагодження
        if (debugMode == 1)
        {
            printf("%4d  %18.10f  %18.10f  %18.10f  %18.10e\n",
                   *iterations, left, right, middle, middleValue);
        }

        // Перевірка точного потрапляння в корінь
        if (middleValue == 0.0)
        {
            *rootFound = 1; // Корінь знайдено
            return middle;  // Повернення точного значення кореня
        }

        // Вибір половини, на кінцях якої функція має протилежні знаки
        if (functionValue(left) * middleValue < 0.0)
        {
            right = middle; // Корінь міститься в лівій половині проміжку
        }
        else
        {
            left = middle; // Корінь міститься в правій половині проміжку
        }
    }

    middle = (left + right) / 2.0; // Остаточне наближення до кореня
    *rootFound = 1; // Корінь знайдено із заданою похибкою
    return middle; // Повернення знайденого кореня
}

