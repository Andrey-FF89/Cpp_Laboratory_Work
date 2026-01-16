#include <iostream>
#include <cmath>
#include <stdexcept>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>

class Figure {
protected:
    double size1, size2, size3;

public:
    Figure() : size1(0), size2(0), size3(0) {}
    Figure(double p1, double p2, double p3) : size1(p1), size2(p2), size3(p3) {}

    virtual ~Figure() = default;

    /*
     * Вычисляет площадь фигуры
     *
     * @return возвращает площадь фигуры как double
     */
    virtual double square() const = 0;

    /*
     * Вычисляет периметр фигуры
     *
     * @return возвращает периметр фигуры как double
     */
    virtual double perimeter() const = 0;

    double getSize1() const { return size1; }
    double getSize2() const { return size2; }
    double getSize3() const { return size3; }

    /*
     * Устанавливает первый размерный параметр
     *
     * @param p1 новое значение для size1
     * @throws std::invalid_argument если p1 не положительный
     */
    void setSize1(double p1) {
        if (p1 >= 0) size1 = p1;
        else throw std::invalid_argument("Size must be greater than zero");
    }

    /*
     * Устанавливает второй размерный параметр
     *
     * @param p2 новое значение для size2
     * @throws std::invalid_argument если p2 не положительный
     */
    void setSize2(double p2) {
        if (p2 >= 0) size2 = p2;
        else throw std::invalid_argument("Size must be greater than zero");
    }

    /*
     * Устанавливает третий размерный параметр
     *
     * @param p3 новое значение для size3
     * @throws std::invalid_argument если p3 не положительный
     */
    void setSize3(double p3) {
        if (p3 >= 0) size3 = p3;
        else throw std::invalid_argument("Size must be greater than zero");
    }

    /*
     * Выводит данные фигуры в консоль
     */
    virtual void Data() const {
        std::cout << "Figure: size1=" << size1
            << ", size2=" << size2
            << ", size3=" << size3 << std::endl;
    }

    /*
     * Получает тип фигуры в виде строки
     *
     * @return возвращает название типа фигуры
     */
    virtual std::string getType() const = 0;

    /*
     * Сравнивает две фигуры по их площади
     *
     * @param a указатель на первую фигуру
     * @param b указатель на вторую фигуру
     * @return возвращает true если площадь a меньше площади b
     */
    static bool compareBySquare(const Figure* a, const Figure* b) {
        return a->square() < b->square();
    }
};

class Square : public Figure {
public:
    /*
     * Создает квадрат с заданной длиной стороны
     *
     * @param side длина стороны квадрата
     * @throws std::invalid_argument если side не положительный
     */
    Square(double side) {
        setSize1(side);
        setSize2(0);
        setSize3(0);
    }

    /*
     * Вычисляет площадь квадрата
     *
     * @return возвращает площадь как side * side
     */
    double square() const override {
        return size1 * size1;
    }

    /*
     * Вычисляет периметр квадрата
     *
     * @return возвращает периметр как 4 * side
     */
    double perimeter() const override {
        return size1 * 4;
    }

    double getSide() const { return size1; }

    /*
     * Устанавливает длину стороны квадрата
     *
     * @param side новая длина стороны
     * @throws std::invalid_argument если side не положительный
     */
    void setSide(double side) { setSize1(side); }

    /*
     * Получает тип фигуры
     *
     * @return возвращает "Square" как строку
     */
    std::string getType() const override {
        return "Square";
    }

    /*
     * Выводит данные квадрата в требуемом формате
     */
    void Data() const override {
        std::cout << "Square {side=" << std::fixed << std::setprecision(1) << size1
            << "} S=" << std::fixed << std::setprecision(2) << square()
            << " P=" << std::fixed << std::setprecision(1) << perimeter() << std::endl;
    }
};

class Rectangle : public Figure {
public:
    /*
     * Создает прямоугольник с заданной шириной и высотой
     *
     * @param width ширина прямоугольника
     * @param height высота прямоугольника
     * @throws std::invalid_argument если width или height не положительные
     */
    Rectangle(double width, double height) {
        setSize1(width);
        setSize2(height);
        setSize3(0);
    }

    /*
     * Вычисляет площадь прямоугольника
     *
     * @return возвращает площадь как width * height
     */
    double square() const override {
        return size1 * size2;
    }

    /*
     * Вычисляет периметр прямоугольника
     *
     * @return возвращает периметр как 2 * (width + height)
     */
    double perimeter() const override {
        return 2 * (size1 + size2);
    }

    double getWidth() const { return size1; }
    double getHeight() const { return size2; }

    /*
     * Устанавливает ширину прямоугольника
     *
     * @param width новая ширина
     * @throws std::invalid_argument если width не положительный
     */
    void setWidth(double width) { setSize1(width); }

    /*
     * Устанавливает высоту прямоугольника
     *
     * @param height новая высота
     * @throws std::invalid_argument если height не положительный
     */
    void setHeight(double height) { setSize2(height); }

    /*
     * Получает тип фигуры
     *
     * @return возвращает "Rectangle" как строку
     */
    std::string getType() const override {
        return "Rectangle";
    }

    /*
     * Выводит данные прямоугольника в требуемом формате
     */
    void Data() const override {
        std::cout << "Rectangle {width=" << std::fixed << std::setprecision(1) << size1
            << ", height=" << std::fixed << std::setprecision(1) << size2
            << "} S=" << std::fixed << std::setprecision(2) << square()
            << " P=" << std::fixed << std::setprecision(1) << perimeter() << std::endl;
    }
};

class Triangle : public Figure {
public:
    /*
     * Создает треугольник с заданными длинами сторон
     *
     * @param a длина первой стороны
     * @param b длина второй стороны
     * @param c длина третьей стороны
     * @throws std::invalid_argument если стороны нарушают неравенство треугольника или не положительные
     */
    Triangle(double a, double b, double c) {
        // Проверка неравенства треугольника
        if (a + b <= c || a + c <= b || b + c <= a) {
            throw std::invalid_argument("Triangle inequality violated");
        }
        setSize1(a);
        setSize2(b);
        setSize3(c);
    }

    /*
     * Вычисляет площадь треугольника по формуле Герона
     *
     * @return возвращает площадь треугольника
     */
    double square() const override {
        double p = perimeter() / 2;
        return sqrt(p * (p - size1) * (p - size2) * (p - size3));
    }

    /*
     * Вычисляет периметр треугольника
     *
     * @return возвращает периметр как сумму всех сторон
     */
    double perimeter() const override {
        return size1 + size2 + size3;
    }

    double getA() const { return size1; }
    double getB() const { return size2; }
    double getC() const { return size3; }

    /*
     * Устанавливает длину стороны A
     *
     * @param a новая длина для стороны A
     * @throws std::invalid_argument если a не положительный
     */
    void setA(double a) { setSize1(a); }

    /*
     * Устанавливает длину стороны B
     *
     * @param b новая длина для стороны B
     * @throws std::invalid_argument если b не положительный
     */
    void setB(double b) { setSize2(b); }

    /*
     * Устанавливает длину стороны C
     *
     * @param c новая длина для стороны C
     * @throws std::invalid_argument если c не положительный
     */
    void setC(double c) { setSize3(c); }

    /*
     * Получает тип фигуры
     *
     * @return возвращает "Triangle" как строку
     */
    std::string getType() const override {
        return "Triangle";
    }

    /*
     * Выводит данные треугольника в требуемом формате
     */
    void Data() const override {
        std::cout << "Triangle {a=" << std::fixed << std::setprecision(1) << size1
            << ", b=" << std::fixed << std::setprecision(1) << size2
            << ", c=" << std::fixed << std::setprecision(1) << size3
            << "} S=" << std::fixed << std::setprecision(2) << square()
            << " P=" << std::fixed << std::setprecision(1) << perimeter() << std::endl;
    }
};

class Geometry_Dash {
private:
    std::vector<Figure*> figures;
    size_t maxSize;

public:
    /*
     * Создает геометрическую коллекцию с максимальным размером
     *
     * @param max максимальное количество фигур в коллекции (по умолчанию 15)
     */
    Geometry_Dash(size_t max = 15) : maxSize(max) {}

    /*
     * Деструктор - автоматически очищает выделенную память
     */
    ~Geometry_Dash() {
        clear();
    }

    /*
     * Добавляет фигуру в коллекцию
     *
     * @param figure указатель на фигуру для добавления
     * @return возвращает true если фигура успешно добавлена, false если коллекция полна
     */
    bool addFigure(Figure* figure) {
        if (figures.size() < maxSize) {
            figures.push_back(figure);
            return true;
        }
        return false;
    }

    /*
     * Удаляет фигуру из коллекции по индексу
     *
     * @param index индекс фигуры для удаления
     * @return возвращает true если фигура успешно удалена, false если индекс невалидный
     */
    bool removeFigure(size_t index) {
        if (index < figures.size()) {
            delete figures[index];
            figures.erase(figures.begin() + index);
            return true;
        }
        return false;
    }

    /*
     * Очищает все фигуры из коллекции и освобождает память
     */
    void clear() {
        for (auto figure : figures) {
            delete figure;
        }
        figures.clear();
    }

    /*
     * Выводит информацию о всех фигурах в коллекции
     */
    void printAll() const {
        std::cout << "=== Geometry Collection (" << figures.size() << " figures) ===" << std::endl;
        for (size_t i = 0; i < figures.size(); ++i) {
            std::cout << i + 1 << ". ";
            figures[i]->Data();
        }
    }

    /*
     * Вычисляет общую площадь всех фигур в коллекции
     *
     * @return возвращает сумму всех площадей с точностью до 2 десятичных знаков
     */
    double totalSquare() const {
        double total = 0;
        for (auto figure : figures) {
            total += figure->square();
        }
        return total;
    }

    /*
     * Находит фигуру с максимальным периметром
     *
     * @return возвращает указатель на фигуру с максимальным периметром, или nullptr если коллекция пуста
     */
    Figure* findMaxPerimeter() const {
        if (figures.empty()) return nullptr;

        Figure* maxFigure = figures[0];
        for (size_t i = 1; i < figures.size(); ++i) {
            if (figures[i]->perimeter() > maxFigure->perimeter()) {
                maxFigure = figures[i];
            }
        }
        return maxFigure;
    }

    /*
     * Сортирует все фигуры в коллекции по площади (по возрастанию)
     */
    void sortBySquare() {
        std::sort(figures.begin(), figures.end(), Figure::compareBySquare);
    }

    /*
 * Генерирует случайные фигуры для коллекции
 * Создает 5-15 случайных фигур разных типов с заданными диапазонами параметров
 */
    void generateRandomFigures() {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Генерирует случайное количество фигур от 5 до 15
        std::uniform_int_distribution<> countDist(5, 15);
        size_t count = countDist(gen);

        std::uniform_int_distribution<> typeDist(0, 2); // 0 - Square, 1 - Rectangle, 2 - Triangle
        std::uniform_real_distribution<> squareDist(1.0, 10.0);
        std::uniform_real_distribution<> rectWidthDist(1.0, 8.0);
        std::uniform_real_distribution<> rectHeightDist(1.0, 6.0);
        std::uniform_real_distribution<> triangleDist(3.0, 7.0);

        std::cout << "Generating " << count << " random figures..." << std::endl;

        for (size_t i = 0; i < count && figures.size() < maxSize; ++i) {
            int type = typeDist(gen);

            try {
                switch (type) {
                case 0: { // Square
                    double side = squareDist(gen);
                    figures.push_back(new Square(side));
                    break;
                }
                case 1: { // Rectangle
                    double width = rectWidthDist(gen);
                    double height = rectHeightDist(gen);
                    figures.push_back(new Rectangle(width, height));
                    break;
                }
                case 2: { // Triangle
                    // Оптимизированная генерация треугольника
                    // Генерируем две стороны и вычисляем третью так, чтобы удовлетворять неравенству
                    double a = triangleDist(gen);
                    double b = triangleDist(gen);

                    // Третья сторона должна быть меньше суммы первых двух и больше их разности
                    double min_c = std::abs(a - b) + 0.1; // +0.1 чтобы гарантировать >
                    double max_c = a + b - 0.1; // -0.1 чтобы гарантировать <

                    if (min_c < max_c) {
                        std::uniform_real_distribution<> cDist(min_c, max_c);
                        double c = cDist(gen);
                        figures.push_back(new Triangle(a, b, c));
                    }
                    else {
                        // Если не удалось сгенерировать валидный треугольник, создаем квадрат вместо него
                        figures.push_back(new Square(squareDist(gen)));
                    }
                    break;
                }
                }
            }
            catch (const std::exception& e) {
                // В случае ошибки создаем квадрат вместо проблемной фигуры
                figures.push_back(new Square(squareDist(gen)));
            }
        }

        std::cout << "Successfully generated " << figures.size() << " figures" << std::endl;
    }

    /*
     * Получает количество фигур в коллекции
     *
     * @return возвращает текущее количество фигур
     */
    size_t size() const {
        return figures.size();
    }

    /*
     * Получает фигуру по индексу
     *
     * @param index индекс фигуры для получения
     * @return возвращает указатель на фигуру, или nullptr если индекс невалидный
     */
    Figure* getFigure(size_t index) const {
        if (index < figures.size()) {
            return figures[index];
        }
        return nullptr;
    }

    /*
     * Выводит общую площадь всех фигур
     */
    void printTotalSquare() const {
        double total = totalSquare();
        std::cout << "Total area of all figures: " << std::fixed << std::setprecision(2)
            << total << std::endl;
    }
};

/*
 * Выполняет расширенное тестирование геометрической коллекции
 * Включает генерацию, сортировку и поиск максимального периметра
 */
void advancedTest() {
    Geometry_Dash collection;

    std::cout << "=== RANDOM FIGURES GENERATION ===" << std::endl;
    collection.generateRandomFigures();
    collection.printAll();

    std::cout << "\n=== TOTAL AREA ===" << std::endl;
    collection.printTotalSquare();

    std::cout << "\n=== SORTING BY AREA ===" << std::endl;
    collection.sortBySquare();
    collection.printAll();

    std::cout << "\n=== FIGURE WITH MAXIMUM PERIMETER ===" << std::endl;
    Figure* maxPerimeterFigure = collection.findMaxPerimeter();
    if (maxPerimeterFigure) {
        std::cout << "Figure with maximum perimeter: ";
        maxPerimeterFigure->Data();
    }

    // Вывод отсортированного списка с позициями
    std::cout << "\n=== SORTED FIGURES LIST ===" << std::endl;
    for (size_t i = 0; i < collection.size(); ++i) {
        Figure* fig = collection.getFigure(i);
        std::cout << "Position " << (i + 1) << ": ";
        fig->Data();
    }
}

/*
 * Главная функция - точка входа в программу
 *
 * @return возвращает 0 при успешном выполнении
 */
int main() {
    try {
        std::cout << std::fixed << std::setprecision(2);

        advancedTest();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}