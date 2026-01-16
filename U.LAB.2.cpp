#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

/**
 * Заполнение массива случайными числами в заданном диапазоне
 *
 * @param arr указатель на массив.
 * @param N размер массива.
 * @param min минимальное значение.
 * @param max максимальное значение.
 */
void fillArray(double* arr, int N, double min, double max) {
    for (int i = 0; i < N; i++) {
        arr[i] = min + static_cast<double>(rand()) / RAND_MAX * (max - min);
    }
}

/**
 * Поиск месяцев с максимальным и минимальным доходом
 *
 * @param arr указатель на массив.
 * @param N размер массива.
 * @param maxMonth ссылка для сохранения номера месяца с максимальным доходом.
 * @param minMonth ссылка для сохранения номера месяца с минимальным доходом.
 */
void findExtremes(double* arr, int N, int& maxMonth, int& minMonth) {
    maxMonth = minMonth = 0;

    for (int i = 1; i < N; i++) {
        if (arr[i] > arr[maxMonth]) {
            maxMonth = i;
        }
        if (arr[i] < arr[minMonth]) {
            minMonth = i;
        }
    }
}

/**
 * Вычисление среднегодового дохода
 *
 * @param arr указатель на массив.
 * @param N размер массива.
 * @return возвращает среднее арифметическое значение массива.
 */
double calculateAverage(double* arr, int N) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        sum += arr[i];
    }
    return sum / N;
}

/**
 * Вычисление стандартного отклонения
 *
 * @param arr указатель на массив.
 * @param N размер массива.
 * @param average среднее значение массива.
 * @return возвращает стандартное отклонение.
 */
double calculateStandardDeviation(double* arr, int N, double average) {
    double variance = 0.0;
    for (int i = 0; i < N; i++) {
        variance += pow((arr[i] - average), 2);
    }
    variance /= N;
    return sqrt(variance);
}

/**
 * Сортировка массива по возрастанию (передача по указателю)
 *
 * @param arr указатель на массив.
 * @param N размер массива.
 */
void sortArrayPointer(double* arr, int N) {
    std::sort(arr, arr + N);
}

/**
 * Создание отсортированной копии массива (передача по значению)
 *
 * @param arr копия массива.
 * @param N размер массива.
 * @return возвращает отсортированную копию массива.
 */
double* sortArrayValue(double arr[], int N) {
    double* sortedArr = new double[N];
    // Копируем данные из исходного массива
    for (int i = 0; i < N; i++) {
        sortedArr[i] = arr[i];
    }
    // Сортируем копию массива
    std::sort(sortedArr, sortedArr + N);
    return sortedArr; // Возвращаем указатель на отсортированную копию
}

/**
 * Вывод массива на экран
 *
 * @param arr указатель на массив.
 * @param N размер массива.
 * @param title заголовок для вывода.
 */
void printArray(double* arr, int N, const std::string& title) {
    std::cout << title << ": ";
    for (int i = 0; i < N; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    srand(time(0));

    int N;
    std::cout << "Enter the number of months (N): ";
    std::cin >> N;

    double* income = new double[N];

    fillArray(income, N, 10.0, 100.0);

    std::cout << "\nInitial data:" << std::endl;
    printArray(income, N, "Monthly income");

    int choice;
    do {
        std::cout << "\nChoose an action:" << std::endl;
        std::cout << "1. Find month with maximum and minimum income" << std::endl;
        std::cout << "2. Calculate average annual income and standard deviation" << std::endl;
        std::cout << "3. Sort array in ascending order" << std::endl;
        std::cout << "4. Compare pass by value and pass by reference" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            // Поиск месяцев с максимальным и минимальным доходом
            int maxMonth, minMonth;
            findExtremes(income, N, maxMonth, minMonth);
            std::cout << "Maximum income: month " << maxMonth + 1
                << " (" << income[maxMonth] << ")" << std::endl;
            std::cout << "Minimum income: month " << minMonth + 1
                << " (" << income[minMonth] << ")" << std::endl;
            break;
        }

        case 2: {
            // Вычисление статистических показателей
            double average = calculateAverage(income, N);
            double deviation = calculateStandardDeviation(income, N, average);
            std::cout << "Average annual income: " << average << std::endl;
            std::cout << "Standard deviation: " << deviation << std::endl;
            break;
        }

        case 3: {
            // Создание копии массива для демонстрации сортировки
            double* incomeCopy = new double[N];
            for (int i = 0; i < N; i++) {
                incomeCopy[i] = income[i];
            }

            sortArrayPointer(incomeCopy, N);
            printArray(incomeCopy, N, "Sorted array");
            delete[] incomeCopy;
            break;
        }

        case 4: {
            // Демонстрация разницы между передачей по указателю и по значению
            std::cout << "\nComparison of pass by value and pass by reference:" << std::endl;

            double* arr1 = new double[N];
            for (int i = 0; i < N; i++) arr1[i] = income[i];

            std::cout << "Before sorting (pass by pointer): ";
            printArray(arr1, N, "");
            sortArrayPointer(arr1, N);
            std::cout << "After sorting (pass by pointer): ";
            printArray(arr1, N, "");
            delete[] arr1;

            std::cout << "Original array before pass by value: ";
            printArray(income, N, "");
            double* sortedArr = sortArrayValue(income, N);
            std::cout << "Sorted copy (pass by value): ";
            printArray(sortedArr, N, "");
            std::cout << "Original array after pass by value: ";
            printArray(income, N, "");
            delete[] sortedArr;

            std::cout << "\nConclusion: pass by pointer/reference modifies the original array, ";
            std::cout << "pass by value creates a copy." << std::endl;
            break;
        }

        case 0:
            std::cout << "Exiting program." << std::endl;
            break;

        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 0);

    // Освобождение памяти, выделенной для основного массива
    delete[] income;

    return 0;
}