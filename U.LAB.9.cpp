#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <utility>   // std::pair

/*
 * Генерирует случайное число в заданном диапазоне.
 *
 * @param min Нижняя граница диапазона.
 * @param max Верхняя граница диапазона.
 * @return Случайное целое число от min до max.
 */
int getRandomNumber(int const min, int const max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

/**
 * Создает и заполняет карту данными.
 * Ключ: целое число от 1 до N.
 * Значение: вектор из 5 случайных чисел (1-100).
 *
 * @param n Количество элементов в map.
 * @return Заполненный контейнер std::map.
 */
std::map<int, std::vector<int>> createDataMap(int const n) {
    std::map<int, std::vector<int>> result;
    for (int i = 1; i <= n; ++i) {
        std::vector<int> vec;
        for (int j = 0; j < 5; ++j) {
            vec.push_back(getRandomNumber(1, 100));
        }
        result[i] = vec;
    }
    return result;
}

/**
 * Шаблонная функция для вывода содержимого карты.
 * Использует вложенный for_each для обхода ключей и значений вектора.
 *
 * @tparam K Тип ключа.
 * @tparam V Тип значения (должен поддерживать итерацию).
 * @param container Контейнер map для вывода.
 */
template <typename K, typename V>
void printMap(const std::map<K, V>& container) {
    std::cout << "--- Container Contents ---" << std::endl;

    // Алгоритм для обхода пар (ключ, значение)
    std::for_each(container.begin(), container.end(), [](const auto& pair) {
        std::cout << "Key " << pair.first << " : [  ";

        // Вывод элементов вектора
        std::for_each(pair.second.begin(), pair.second.end(), [](const auto& val) {
            std::cout << std::left << std::setw(4) << val;
            });

        std::cout << "]" << std::endl;
        });
    std::cout << "--------------------------" << std::endl;
}

/**
 * Функция сортирует каждый вектор внутри карты по возрастанию.
 * Использует std::sort внутри std::for_each.
 *
 * @param data Ссылка на карту для модификации.
 */
void sortMapVectors(std::map<int, std::vector<int>>& data) {
    std::for_each(data.begin(), data.end(), [](auto& pair) {
        // pair.second - это vector<int>
        std::sort(pair.second.begin(), pair.second.end());
        });
}

/**
 * Функция ищет вектор по ключу и первое число, кратное 7, внутри этого вектора.
 *
 * @param data Карта с данными.
 * @param key Ключ для поиска.
 */
void findDivisibleBySeven(const std::map<int, std::vector<int>>& data, int const key) {
    std::cout << "\nSearch for key " << key << ":" << std::endl;

    auto mapIt = data.find(key);

    if (mapIt != data.end()) {
        const std::vector<int>& vec = mapIt->second;

        auto vecIt = std::find_if(vec.begin(), vec.end(), [](int n) {
            return n % 7 == 0;
            });

        if (vecIt != vec.end()) {
            std::cout << "Number divisible by 7 found: " << *vecIt << std::endl;
        }
        else {
            std::cout << "No numbers divisible by 7 found in the vector." << std::endl;
        }
    }
    else {
        std::cout << "Key not found." << std::endl;
    }
}

int main() {
    // Контейнер и данные
    const int N = 5;
    auto myMap = createDataMap(N);

    // Вывод
    std::cout << "Initial Data:" << std::endl;
    printMap(myMap);

    // Сортировка
    sortMapVectors(myMap);

    std::cout << "\nAfter sorting the vectors:" << std::endl;
    printMap(myMap);

    // Поиск по ключу и значению
    findDivisibleBySeven(myMap, 2);

    std::vector<int> specialNumbers;

    // Лямбда захватывает specialNumbers по ссылке
    auto collectSpecials = [&specialNumbers](const std::pair<const int, std::vector<int>>& pair) {
        const auto& vec = pair.second;
        // Проверяем числа в диапазоне
        for (int num : vec) {
            if (num >= 80 && num <= 100) {
                specialNumbers.push_back(num);
            }
        }
        };

    // Применение лямбды ко всем элементам
    std::for_each(myMap.begin(), myMap.end(), collectSpecials);

    // Вывод чисел из диапозона
    std::cout << "\nCollected special numbers (80-100): ";
    if (specialNumbers.empty()) {
        std::cout << "No numbers found in the specified range." << std::endl;
    }
    else {
        for (int num : specialNumbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}