#include <iostream>
#include <vector>
#include <climits>

using namespace std;

/**
 * Вычисление суммы элементов массива
 *
 * @param array массив чисел для суммирования
 * @return возвращает сумму всех элементов массива
 */
int calculateSum(const vector<int>& array) {
    int sum = 0;
    for (int num : array) {
        sum += num;
    }
    return sum;
}

/**
 * Поиск рыцаря с максимальной суммой сил
 *
 * @param knights вектор массивов сил всех рыцарей
 * @return возвращает индекс рыцаря с максимальной суммой сил
 */
int findKnightWithMaxSum(const vector<vector<int>>& knights) {
    int maxSum = INT_MIN;
    int knightIndex = -1;

    for (int i = 0; i < knights.size(); i++) {
        int currentSum = calculateSum(knights[i]);
        if (currentSum > maxSum) {
            maxSum = currentSum;
            knightIndex = i;
        }
    }

    return knightIndex;
}

/**
 * Поиск рыцаря с минимальной суммой сил
 *
 * @param knights вектор массивов сил всех рыцарей
 * @return возвращает индекс рыцаря с минимальной суммой сил
 */
int findKnightWithMinSum(const vector<vector<int>>& knights) {
    int minSum = INT_MAX;
    int knightIndex = -1;

    for (int i = 0; i < knights.size(); i++) {
        int currentSum = calculateSum(knights[i]);
        if (currentSum < minSum) {
            minSum = currentSum;
            knightIndex = i;
        }
    }

    return knightIndex;
}

/**
 * Ввод данных о силах рыцарей
 *
 * @param knights ссылка на вектор для заполнения данными
 * @param n количество рыцарей
 * @param m длина массива сил каждого рыцаря
 */
void inputKnightsData(vector<vector<int>>& knights, int n, int m) {
    for (int i = 0; i < n; i++) {
        cout << "Enter strengths of knight " << i + 1 << " (" << m << " numbers): ";
        for (int j = 0; j < m; j++) {
            cin >> knights[i][j];
        }
    }
}

/**
 * Вывод результатов поиска рыцарей
 *
 * @param knights вектор массивов сил всех рыцарей
 * @param maxIndex индекс рыцаря с максимальной суммой сил
 * @param minIndex индекс рыцаря с минимальной суммой сил
 */
void printResults(const vector<vector<int>>& knights, int maxIndex, int minIndex) {
    cout << "\n=== TOURNAMENT RESULTS ===" << endl;
    cout << "Knight with maximum strength sum: #" << maxIndex + 1 << endl;
    cout << "Strength sum: " << calculateSum(knights[maxIndex]) << endl;
    cout << "Strength array: ";
    for (int num : knights[maxIndex]) {
        cout << num << " ";
    }

    cout << "\n\nKnight with minimum strength sum: #" << minIndex + 1 << endl;
    cout << "Strength sum: " << calculateSum(knights[minIndex]) << endl;
    cout << "Strength array: ";
    for (int num : knights[minIndex]) {
        cout << num << " ";
    }
}

int main() {
    int n;
    int m;

    cout << "=== KINGDOM KNIGHTS TOURNAMENT ===" << endl;
    cout << "Enter number of knights: ";
    cin >> n;
    cout << "Enter length of strength array for each knight: ";
    cin >> m;

    vector<vector<int>> knights(n, vector<int>(m));

    // Ввод
    inputKnightsData(knights, n, m);

    // Поиск
    int maxKnightIndex = findKnightWithMaxSum(knights);
    int minKnightIndex = findKnightWithMinSum(knights);

    // Вывод
    printResults(knights, maxKnightIndex, minKnightIndex);

    return 0;
}