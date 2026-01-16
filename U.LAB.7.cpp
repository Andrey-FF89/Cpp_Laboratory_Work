#include <iostream>
#include <string>
#include <vector>
#include <memory>   
#include <concepts>   
#include <cmath>       
#include <numbers>  // Для std::numbers::pi (C++20)
#include <iomanip>    

// --- 1. Интерфейс и Концепт (Основа для обоих вариантов) ---

/*
 * Концепт Numeric
 * Ограничивает тип T только целыми (int, long) или
 * вещественными (float, double) типами.
 */
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

/*
 * Интерфейс IShape
 * Абстрактный базовый класс, который должны реализовать все фигуры.
 * Гарантирует, что любая фигура будет иметь методы area() и name().
 */
class IShape {
public:
    // Виртуальный деструктор по умолчанию
    virtual ~IShape() = default;

    /*
     * Вычисляет площадь фигуры.
     * @return возвращает площадь в *стандартных* единицах (м²) как double
     */
    virtual double area() const = 0;

    /*
     * Возвращает имя (тип) фигуры.
     * @return возвращает std::string с именем
     */
    virtual std::string name() const = 0;
};

// --- 2. Классы "Базового варианта" ---

/*
 * Шаблонный класс Rectangle (Базовый)
 * Реализует IShape и работает с любым Numeric типом.
 */
template <Numeric T>
class Rectangle : public IShape {
    T width;
    T height;

public:
    Rectangle(T w, T h) : width(w), height(h) {}

    /*
     * Вычисляет площадь.
     * Приводит T к double для универсальности.
     */
    double area() const override {
        return static_cast<double>(width) * static_cast<double>(height);
    }

    std::string name() const override {
        return "Rectangle (Base)";
    }
};

/*
 * Шаблонный класс Circle (Базовый)
 * Реализует IShape и работает с любым Numeric типом.
 */
template <Numeric T>
class Circle : public IShape {
    T radius;

public:
    Circle(T r) : radius(r) {}

    /*
     * Вычисляет площадь.
     * Использует C++20 std::numbers::pi для точности.
     */
    double area() const override {
        return std::numbers::pi * static_cast<double>(radius) * static_cast<double>(radius);
    }

    std::string name() const override {
        return "Circle (Base)";
    }
};

// --- 3. Классы "Усложнённого варианта" (Единицы измерения) ---

// --- 3.1. Типы Единиц (Unit Tags) ---
/*
 * Классы-теги для представления единиц измерения.
 * Они содержат статический коэффициент для перевода
 * в базовую единицу (метры).
 */
struct Meters {
    static constexpr double to_base = 1.0; // Базовая единица
    static constexpr const char* symbol = "m";
};

struct Centimeters {
    static constexpr double to_base = 0.01; // 1 см = 0.01 м
    static constexpr const char* symbol = "cm";
};

// --- 3.2. Класс Quantity ---

/*
 * Шаблонный класс Quantity (Количество)
 * Хранит значение (value) типа T с привязкой к единице Unit.
 */
template <Numeric T, typename Unit>
class Quantity {
    T value;

public:
    explicit Quantity(T v) : value(v) {}

    // Метод get() возвращает числовое значение
    T get() const { return value; }

    // Вспомогательный метод для перевода в базовые единицы (метры)
    double to_base_units() const {
        return static_cast<double>(value) * Unit::to_base;
    }
};

/*
 * Перегрузка оператора сложения для Quantity.
 * Сработает только если T и Unit *полностью* совпадают.
 * (Например, Quantity<int, Meters> + Quantity<int, Meters>)
 */
template <Numeric T, typename Unit>
Quantity<T, Unit> operator+(const Quantity<T, Unit>& lhs, const Quantity<T, Unit>& rhs) {
    return Quantity<T, Unit>(lhs.get() + rhs.get());
}

// --- 3.3. Специализированные фигуры (с единицами) ---

/*
 * Шаблонный класс RectangleWithUnits
 * Реализует IShape.
 * Хранит ширину и высоту как Quantity.
 */
template <Numeric T, typename Unit>
class RectangleWithUnits : public IShape {
    Quantity<T, Unit> width;
    Quantity<T, Unit> height;

public:
    RectangleWithUnits(Quantity<T, Unit> w, Quantity<T, Unit> h)
        : width(w), height(h) {
    }

    std::string name() const override {
        return "Rectangle (With Units)";
    }

    /*
     * Вычисляет площадь.
     * Сначала переводит размеры в базовые единицы (метры),
     * а затем вычисляет площадь в (м²).
     */
    double area() const override {
        return width.to_base_units() * height.to_base_units();
    }
};

/*
 * Шаблонный класс CircleWithUnits
 * Реализует IShape.
 * Хранит радиус как Quantity.
 */
template <Numeric T, typename Unit>
class CircleWithUnits : public IShape {
    Quantity<T, Unit> radius;

public:
    CircleWithUnits(Quantity<T, Unit> r) : radius(r) {}

    std::string name() const override {
        return "Circle (With Units)";
    }

    /*
     * Вычисляет площадь.
     * Переводит радиус в базовые единицы (метры),
     * а затем вычисляет площадь в (м²).
     */
    double area() const override {
        double r_base = radius.to_base_units();
        return std::numbers::pi * r_base * r_base;
    }
};

// --- 4. Класс Scene (Агрегатор) ---

/*
 * Класс Scene
 * Агрегирует (собирает) коллекцию различных фигур.
 * Использует std::vector из std::unique_ptr<IShape>.
 */
class Scene {
    // Вектор умных указателей на IShape.
    // unique_ptr автоматически управляет памятью (удаляет объекты).
    std::vector<std::unique_ptr<IShape>> shapes;

public:
    /*
     * Добавляет фигуру в сцену.
     * Принимает unique_ptr и использует std::move для передачи владения.
     */
    void add(std::unique_ptr<IShape> shape) {
        if (shape) {
            shapes.push_back(std::move(shape));
        }
    }

    /*
     * Вычисляет суммарную площадь всех фигур в сцене.
     * Возвращает double (в м²).
     */
    double total_area() const {
        double total = 0.0;
        // Проходим по всем фигурам в векторе
        for (const auto& shape : shapes) {
            total += shape->area(); // Полиморфный вызов area()
        }
        return total;
    }

    /*
     * Вспомогательный метод для печати содержимого сцены.
     */
    void print_all() const {
        std::cout << "--- Scene Contents ---" << std::endl;
        if (shapes.empty()) {
            std::cout << "Scene is empty." << std::endl;
            return;
        }
        int i = 1;
        for (const auto& shape : shapes) {
            // Мы можем вызывать только методы из IShape
            std::cout << i++ << ". " << shape->name()
                << ", Area: " << std::fixed << std::setprecision(4)
                << shape->area() << " m^2" << std::endl;
        }
        std::cout << "----------------------" << std::endl;
    }
};


// --- 5. Главная функция (main) ---

int main() {
    // Настраиваем точность вывода для double
    std::cout << std::fixed << std::setprecision(4);

    std::cout << "=== 1. Base Variant Figures (No Units) ===" << std::endl;
    Rectangle<int> base_rect(10, 5); // 10x5 "единиц"
    Circle<double> base_circle(3.0);   // радиус 3.0 "единицы"

    // Площади считаются просто как числа
    std::cout << base_rect.name() << " Area: " << base_rect.area() << std::endl;   // 50.0
    std::cout << base_circle.name() << " Area: " << base_circle.area() << std::endl; // ~28.2743


    std::cout << "\n=== 2. Quantity Operations ===" << std::endl;
    Quantity<int, Meters> q_m1(10);
    Quantity<int, Meters> q_m2(5);
    auto sum_m = q_m1 + q_m2;
    std::cout << "10m + 5m = " << sum_m.get() << "m" << std::endl; // 15m

    // Попытка сложить метры и сантиметры не скомпилируется:
    // Quantity<int, Centimeters> q_cm1(100);
    // auto bad_sum = q_m1 + q_cm1; // <- Ошибка компиляции: нет оператора + для разных Unit


    std::cout << "\n=== 3. Advanced Variant Figures (With Units) ===" << std::endl;
    // Создаем Quantity
    auto width_m = Quantity<double, Meters>(2.0);    // 2.0 m
    auto height_m = Quantity<double, Meters>(3.0);   // 3.0 m
    auto radius_cm = Quantity<int, Centimeters>(50); // 50 cm

    // Создаем фигуры
    RectangleWithUnits<double, Meters> rect_units(width_m, height_m);
    CircleWithUnits<int, Centimeters> circle_units(radius_cm);

    // area() автоматически конвертирует в м²
    std::cout << rect_units.name() << " (2m x 3m) Area: " << rect_units.area() << " m^2" << std::endl; // 6.0 m^2
    // 50cm = 0.5m. Площадь = pi * 0.5 * 0.5
    std::cout << circle_units.name() << " (50cm) Area: " << circle_units.area() << " m^2" << std::endl; // ~0.7854 m^2


    std::cout << "\n=== 4. Scene (Aggregator) ===" << std::endl;
    Scene scene;

    // Добавляем в сцену ВСЕ типы фигур

    // Добавляем "базовые" (площади 50.0 и ~3.1416)
    scene.add(std::make_unique<Rectangle<int>>(10, 5));
    scene.add(std::make_unique<Circle<double>>(3.0)); // r=1

    // Добавляем "усложненные" (площади 6.0 и ~0.7854)
    scene.add(std::make_unique<RectangleWithUnits<double, Meters>>(
        Quantity<double, Meters>(2.0),
        Quantity<double, Meters>(3.0)
    ));
    scene.add(std::make_unique<CircleWithUnits<int, Centimeters>>(
        Quantity<int, Centimeters>(50)
    ));

    // Печатаем содержимое сцены
    scene.print_all();

    // Считаем общую площадь
    // Ожидаем: 50.0 + 3.1416 + 6.0 + 0.7854 = 59.9270
    std::cout << "\nTotal Area in Scene: " << scene.total_area() << " m^2" << std::endl;

    return 0;
}