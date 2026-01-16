#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>

/**
 * Перечисление типов сенсоров
 */
enum class SensorType {
    TEMPERATURE,  // Температурный сенсор
    PRESSURE,     // Датчик давления  
    MOTION        // Датчик движения
};

/**
 * Структура данных температурного сенсора
 */
struct TemperatureSensor {
    double temperature;  // Температура в градусах Цельсия
    double humidity;     // Влажность в процентах

    /**
     * Конструктор по умолчанию
     */
    TemperatureSensor() : temperature(0.0), humidity(0.0) {}

    /**
     * Конструктор с параметрами
     */
    TemperatureSensor(double temp, double hum) : temperature(temp), humidity(hum) {}
};

/**
 * Структура данных датчика давления
 */
struct PressureSensor {
    double pressure;  // Давление в паскалях
    double altitude;  // Высота над уровнем моря

    /**
     * Конструктор по умолчанию
     */
    PressureSensor() : pressure(0.0), altitude(0.0) {}

    /**
     * Конструктор с параметрами
     */
    PressureSensor(double press, double alt) : pressure(press), altitude(alt) {}
};

/**
 * Структура данных датчика движения
 */
struct MotionSensor {
    bool motion_detected;   // Обнаружено ли движение
    double acceleration_x;  // Ускорение по оси X
    double acceleration_y;  // Ускорение по оси Y  
    double acceleration_z;  // Ускорение по оси Z

    /**
     * Конструктор по умолчанию
     */
    MotionSensor() : motion_detected(false), acceleration_x(0.0), acceleration_y(0.0), acceleration_z(0.0) {}

    /**
     * Конструктор с параметрами
     */
    MotionSensor(bool motion, double acc_x, double acc_y, double acc_z)
        : motion_detected(motion), acceleration_x(acc_x), acceleration_y(acc_y), acceleration_z(acc_z) {
    }
};

/**
 * Класс, представляющий сенсор в сети
 */
class Sensor {
private:
    void* sensor_data;      // Указатель на данные конкретного сенсора
    SensorType type;        // Тип сенсора
    int timestamp;          // Время создания (Unix timestamp)
    int id;                 // Уникальный идентификатор

public:
    /**
     * Конструктор по умолчанию
     */
    Sensor() : sensor_data(nullptr), type(SensorType::TEMPERATURE), timestamp(0), id(0) {}

    /**
     * Конструктор с параметрами
     *
     * @param sensor_type тип создаваемого сенсора
     * @param sensor_id уникальный идентификатор
     */
    Sensor(SensorType sensor_type, int sensor_id) : type(sensor_type), id(sensor_id) {
        timestamp = static_cast<int>(std::time(nullptr));
        allocateSensorData();
    }

    /**
     * Конструктор копирования
     *
     * @param other копируемый объект
     */
    Sensor(const Sensor& other) : type(other.type), timestamp(other.timestamp), id(other.id) {
        allocateSensorData();
        copySensorData(other);
    }

    /**
     * Оператор копирующего присваивания
     *
     * @param other копируемый объект
     * @return ссылка на текущий объект
     */
    Sensor& operator=(const Sensor& other) {
        if (this != &other) {
            deallocateSensorData();
            type = other.type;
            timestamp = other.timestamp;
            id = other.id;
            allocateSensorData();
            copySensorData(other);
        }
        return *this;
    }

    /**
     * Конструктор перемещения
     *
     * @param other перемещаемый объект
     */
    Sensor(Sensor&& other) noexcept
        : sensor_data(other.sensor_data), type(other.type), timestamp(other.timestamp), id(other.id) {
        other.sensor_data = nullptr;
        other.type = SensorType::TEMPERATURE;
        other.timestamp = 0;
        other.id = 0;
    }

    /**
     * Оператор перемещающего присваивания
     *
     * @param other перемещаемый объект
     * @return ссылка на текущий объект
     */
    Sensor& operator=(Sensor&& other) noexcept {
        if (this != &other) {
            deallocateSensorData();
            sensor_data = other.sensor_data;
            type = other.type;
            timestamp = other.timestamp;
            id = other.id;

            other.sensor_data = nullptr;
            other.type = SensorType::TEMPERATURE;
            other.timestamp = 0;
            other.id = 0;
        }
        return *this;
    }

    /**
     * Деструктор
     */
    ~Sensor() {
        deallocateSensorData();
    }

    /**
     * Опрос сенсора (генерация случайных данных)
     */
    void poll() {
        std::random_device rd;
        std::mt19937 gen(rd());

        switch (type) {
        case SensorType::TEMPERATURE: {
            std::uniform_real_distribution<> temp_dist(-50.0, 50.0);
            std::uniform_real_distribution<> hum_dist(0.0, 100.0);
            TemperatureSensor* data = static_cast<TemperatureSensor*>(sensor_data);
            data->temperature = temp_dist(gen);
            data->humidity = hum_dist(gen);
            break;
        }
        case SensorType::PRESSURE: {
            std::uniform_real_distribution<> press_dist(90000.0, 110000.0);
            std::uniform_real_distribution<> alt_dist(0.0, 5000.0);
            PressureSensor* data = static_cast<PressureSensor*>(sensor_data);
            data->pressure = press_dist(gen);
            data->altitude = alt_dist(gen);
            break;
        }
        case SensorType::MOTION: {
            std::uniform_int_distribution<> motion_dist(0, 1);
            std::uniform_real_distribution<> accel_dist(-10.0, 10.0);
            MotionSensor* data = static_cast<MotionSensor*>(sensor_data);
            data->motion_detected = (motion_dist(gen) == 1);
            data->acceleration_x = accel_dist(gen);
            data->acceleration_y = accel_dist(gen);
            data->acceleration_z = accel_dist(gen);
            break;
        }
        }
        timestamp = static_cast<int>(std::time(nullptr));
    }

    /**
     * Калибровка сенсора (сброс значений)
     */
    void calibrate() {
        switch (type) {
        case SensorType::TEMPERATURE: {
            TemperatureSensor* data = static_cast<TemperatureSensor*>(sensor_data);
            data->temperature = 0.0;
            data->humidity = 0.0;
            break;
        }
        case SensorType::PRESSURE: {
            PressureSensor* data = static_cast<PressureSensor*>(sensor_data);
            data->pressure = 0.0;
            data->altitude = 0.0;
            break;
        }
        case SensorType::MOTION: {
            MotionSensor* data = static_cast<MotionSensor*>(sensor_data);
            data->motion_detected = false;
            data->acceleration_x = 0.0;
            data->acceleration_y = 0.0;
            data->acceleration_z = 0.0;
            break;
        }
        }
    }

    /**
     * Получение типа сенсора
     *
     * @return тип сенсора
     */
    SensorType getType() const { return type; }

    /**
     * Получение времени создания
     *
     * @return время создания (Unix timestamp)
     */
    int getTimestamp() const { return timestamp; }

    /**
     * Получение ID сенсора
     *
     * @return уникальный идентификатор
     */
    int getId() const { return id; }

    /**
     * Вывод данных сенсора в консоль
     */
    void printData() const {
        std::cout << "Sensor ID: " << id << " | Type: ";

        switch (type) {
        case SensorType::TEMPERATURE: {
            TemperatureSensor* data = static_cast<TemperatureSensor*>(sensor_data);
            std::cout << "TEMPERATURE | Temp: " << std::fixed << std::setprecision(2)
                << data->temperature << "C | Humidity: " << data->humidity << "%";
            break;
        }
        case SensorType::PRESSURE: {
            PressureSensor* data = static_cast<PressureSensor*>(sensor_data);
            std::cout << "PRESSURE | Pressure: " << std::fixed << std::setprecision(2)
                << data->pressure << " Pa | Altitude: " << data->altitude << " m";
            break;
        }
        case SensorType::MOTION: {
            MotionSensor* data = static_cast<MotionSensor*>(sensor_data);
            std::cout << "MOTION | Motion: " << (data->motion_detected ? "DETECTED" : "NO MOTION")
                << " | Accel: (" << std::fixed << std::setprecision(2)
                << data->acceleration_x << ", " << data->acceleration_y
                << ", " << data->acceleration_z << ") m/s²";
            break;
        }
        }

        std::time_t time_val = timestamp;
        char time_str[26];
        ctime_s(time_str, sizeof(time_str), &time_val);
        std::cout << " | Time: " << time_str;
    }

private:
    /**
     * Выделение памяти для данных сенсора
     */
    void allocateSensorData() {
        switch (type) {
        case SensorType::TEMPERATURE:
            sensor_data = new TemperatureSensor();
            break;
        case SensorType::PRESSURE:
            sensor_data = new PressureSensor();
            break;
        case SensorType::MOTION:
            sensor_data = new MotionSensor();
            break;
        }
    }

    /**
     * Освобождение памяти данных сенсора
     */
    void deallocateSensorData() {
        if (sensor_data) {
            switch (type) {
            case SensorType::TEMPERATURE:
                delete static_cast<TemperatureSensor*>(sensor_data);
                break;
            case SensorType::PRESSURE:
                delete static_cast<PressureSensor*>(sensor_data);
                break;
            case SensorType::MOTION:
                delete static_cast<MotionSensor*>(sensor_data);
                break;
            }
            sensor_data = nullptr;
        }
    }

    /**
     * Копирование данных сенсора
     *
     * @param other объект для копирования данных
     */
    void copySensorData(const Sensor& other) {
        switch (type) {
        case SensorType::TEMPERATURE: {
            TemperatureSensor* other_data = static_cast<TemperatureSensor*>(other.sensor_data);
            TemperatureSensor* this_data = static_cast<TemperatureSensor*>(sensor_data);
            *this_data = *other_data;
            break;
        }
        case SensorType::PRESSURE: {
            PressureSensor* other_data = static_cast<PressureSensor*>(other.sensor_data);
            PressureSensor* this_data = static_cast<PressureSensor*>(sensor_data);
            *this_data = *other_data;
            break;
        }
        case SensorType::MOTION: {
            MotionSensor* other_data = static_cast<MotionSensor*>(other.sensor_data);
            MotionSensor* this_data = static_cast<MotionSensor*>(sensor_data);
            *this_data = *other_data;
            break;
        }
        }
    }
};

/**
 * Класс-менеджер для управления сетью сенсоров
 */
class SensorNetworkManager {
private:
    std::vector<Sensor*> sensors;  // Массив сенсоров
    int next_id;                   // Следующий доступный ID

public:
    /**
     * Конструктор по умолчанию
     */
    SensorNetworkManager() : next_id(1) {
        std::cout << "SensorNetworkManager default constructor" << std::endl;
    }

    /**
     * Конструктор с параметрами
     *
     * @param initial_sensors начальное количество сенсоров каждого типа
     */
    SensorNetworkManager(int initial_sensors) : next_id(1) {
        std::cout << "SensorNetworkManager parameterized constructor" << std::endl;
        for (int i = 0; i < initial_sensors; ++i) {
            addSensor(SensorType::TEMPERATURE);
            addSensor(SensorType::PRESSURE);
            addSensor(SensorType::MOTION);
        }
    }

    /**
     * Конструктор копирования
     *
     * @param other копируемый объект
     */
    SensorNetworkManager(const SensorNetworkManager& other) : next_id(other.next_id) {
        std::cout << "SensorNetworkManager copy constructor" << std::endl;
        for (const auto& sensor : other.sensors) {
            sensors.push_back(new Sensor(*sensor));
        }
    }

    /**
     * Оператор копирующего присваивания
     *
     * @param other копируемый объект
     * @return ссылка на текущий объект
     */
    SensorNetworkManager& operator=(const SensorNetworkManager& other) {
        std::cout << "SensorNetworkManager copy assignment operator" << std::endl;
        if (this != &other) {
            // Освобождаем существующие ресурсы
            for (auto sensor : sensors) {
                delete sensor;
            }
            sensors.clear();

            // Копируем данные
            next_id = other.next_id;
            for (const auto& sensor : other.sensors) {
                sensors.push_back(new Sensor(*sensor));
            }
        }
        return *this;
    }

    /**
     * Конструктор перемещения
     *
     * @param other перемещаемый объект
     */
    SensorNetworkManager(SensorNetworkManager&& other) noexcept
        : sensors(std::move(other.sensors)), next_id(other.next_id) {
        std::cout << "SensorNetworkManager move constructor" << std::endl;
        other.sensors.clear();
        other.next_id = 1;
    }

    /**
     * Оператор перемещающего присваивания
     *
     * @param other перемещаемый объект
     * @return ссылка на текущий объект
     */
    SensorNetworkManager& operator=(SensorNetworkManager&& other) noexcept {
        std::cout << "SensorNetworkManager move assignment operator" << std::endl;
        if (this != &other) {
            // Освобождаем существующие ресурсы
            for (auto sensor : sensors) {
                delete sensor;
            }

            // Перемещаем данные
            sensors = std::move(other.sensors);
            next_id = other.next_id;

            other.sensors.clear();
            other.next_id = 1;
        }
        return *this;
    }

    /**
     * Деструктор
     */
    ~SensorNetworkManager() {
        std::cout << "SensorNetworkManager destructor" << std::endl;
        for (auto sensor : sensors) {
            delete sensor;
        }
        sensors.clear();
    }

    /**
     * Добавление сенсора в сеть
     *
     * @param type тип добавляемого сенсора
     */
    void addSensor(SensorType type) {
        Sensor* new_sensor = new Sensor(type, next_id++);
        sensors.push_back(new_sensor);
        std::cout << "Added sensor ID: " << new_sensor->getId() << std::endl;
    }

    /**
     * Опрос всех сенсоров в сети
     */
    void pollAllSensors() {
        std::cout << "\n--- Polling all sensors ---" << std::endl;
        for (auto sensor : sensors) {
            sensor->poll();
        }
        std::cout << "All sensors polled successfully!" << std::endl;
    }

    /**
     * Фильтрация сенсоров по типу
     *
     * @param type тип сенсоров для фильтрации
     * @return вектор указателей на отфильтрованные сенсоры
     */
    std::vector<Sensor*> filterByType(SensorType type) const {
        std::vector<Sensor*> result;
        for (auto sensor : sensors) {
            if (sensor->getType() == type) {
                result.push_back(sensor);
            }
        }
        return result;
    }

    /**
     * Калибровка всех сенсоров в сети
     */
    void calibrateAllSensors() {
        std::cout << "\n--- Calibrating all sensors ---" << std::endl;
        for (auto sensor : sensors) {
            sensor->calibrate();
        }
        std::cout << "All sensors calibrated!" << std::endl;
    }

    /**
     * Калибровка сенсоров определенного типа
     *
     * @param type тип сенсоров для калибровки
     */
    void calibrateSensorsByType(SensorType type) {
        std::cout << "\n--- Calibrating sensors of type ---" << std::endl;
        for (auto sensor : sensors) {
            if (sensor->getType() == type) {
                sensor->calibrate();
            }
        }
        std::cout << "Sensors calibrated!" << std::endl;
    }

    /**
     * Сохранение последних значений в журнал (консольный вывод)
     */
    void saveToLog() const {
        std::cout << "\n=== SENSOR NETWORK LOG ===" << std::endl;
        std::cout << "Total sensors: " << sensors.size() << std::endl;
        std::cout << "Timestamp: " << std::time(nullptr) << std::endl;
        std::cout << "---------------------------" << std::endl;

        for (auto sensor : sensors) {
            sensor->printData();
        }
        std::cout << "=== END LOG ===" << std::endl;
    }

    /**
     * Поиск сенсоров по времени создания
     *
     * @param start_time начальное время (Unix timestamp)
     * @param end_time конечное время (Unix timestamp)
     * @return вектор найденных сенсоров
     */
    std::vector<Sensor*> findSensorsByTime(int start_time, int end_time) const {
        std::vector<Sensor*> result;
        std::cout << "\n--- Searching sensors from " << start_time << " to " << end_time << " ---" << std::endl;

        for (auto sensor : sensors) {
            int sensor_time = sensor->getTimestamp();
            if (sensor_time >= start_time && sensor_time <= end_time) {
                result.push_back(sensor);
            }
        }
        return result;
    }

    /**
     * Получение общего количества сенсоров
     *
     * @return количество сенсоров в сети
     */
    int getTotalSensors() const {
        return static_cast<int>(sensors.size());
    }

    /**
     * Получение сенсора по ID
     *
     * @param id идентификатор сенсора
     * @return указатель на сенсор или nullptr если не найден
     */
    Sensor* getSensorById(int id) const {
        for (auto sensor : sensors) {
            if (sensor->getId() == id) {
                return sensor;
            }
        }
        return nullptr;
    }
};

/**
 * Функция для демонстрации работы менеджера
 */
void demonstrateSensorManager() {
    std::cout << "=== DEMONSTRATING SENSOR NETWORK MANAGER ===" << std::endl;

    // Создание менеджера с начальными сенсорами
    SensorNetworkManager manager1(2);
    manager1.saveToLog();

    // Демонстрация копирования
    std::cout << "\n--- Testing copy constructor ---" << std::endl;
    SensorNetworkManager manager2 = manager1;
    manager2.saveToLog();

    // Демонстрация перемещения
    std::cout << "\n--- Testing move constructor ---" << std::endl;
    SensorNetworkManager manager3 = std::move(SensorNetworkManager(1));
    manager3.saveToLog();

    std::cout << "=== DEMONSTRATION COMPLETE ===" << std::endl;
}

/**
 * Функция для вывода меню управления
 */
void printMenu() {
    std::cout << "\n=== SENSOR NETWORK MANAGEMENT ===" << std::endl;
    std::cout << "1. Add temperature sensor" << std::endl;
    std::cout << "2. Add pressure sensor" << std::endl;
    std::cout << "3. Add motion sensor" << std::endl;
    std::cout << "4. Poll all sensors" << std::endl;
    std::cout << "5. Filter sensors by type" << std::endl;
    std::cout << "6. Calibrate all sensors" << std::endl;
    std::cout << "7. Calibrate sensors by type" << std::endl;
    std::cout << "8. Save to log" << std::endl;
    std::cout << "9. Find sensors by time" << std::endl;
    std::cout << "10. Show all sensors" << std::endl;
    std::cout << "11. Demonstrate manager features" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose action: ";
}

/**
 * Основная функция программы
 *
 * @return код завершения программы
 */
int main() {
    SensorNetworkManager network;
    int choice;

    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
        case 1:
            network.addSensor(SensorType::TEMPERATURE);
            break;
        case 2:
            network.addSensor(SensorType::PRESSURE);
            break;
        case 3:
            network.addSensor(SensorType::MOTION);
            break;
        case 4:
            network.pollAllSensors();
            break;
        case 5: {
            std::cout << "Enter type (0-TEMP, 1-PRESSURE, 2-MOTION): ";
            int type;
            std::cin >> type;
            std::vector<Sensor*> filtered = network.filterByType(static_cast<SensorType>(type));
            std::cout << "Found " << filtered.size() << " sensors:" << std::endl;
            for (auto sensor : filtered) {
                sensor->printData();
            }
            break;
        }
        case 6:
            network.calibrateAllSensors();
            break;
        case 7: {
            std::cout << "Enter type to calibrate (0-TEMP, 1-PRESSURE, 2-MOTION): ";
            int type;
            std::cin >> type;
            network.calibrateSensorsByType(static_cast<SensorType>(type));
            break;
        }
        case 8:
            network.saveToLog();
            break;
        case 9: {
            int start_time, end_time;
            std::cout << "Enter start time (Unix timestamp): ";
            std::cin >> start_time;
            std::cout << "Enter end time (Unix timestamp): ";
            std::cin >> end_time;

            std::vector<Sensor*> found = network.findSensorsByTime(start_time, end_time);
            std::cout << "Found " << found.size() << " sensors:" << std::endl;
            for (auto sensor : found) {
                sensor->printData();
            }
            break;
        }
        case 10:
            network.saveToLog();
            break;
        case 11:
            demonstrateSensorManager();
            break;
        case 0:
            std::cout << "Exiting program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    } while (choice != 0);

    return 0;
}