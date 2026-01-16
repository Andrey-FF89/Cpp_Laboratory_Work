#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <iomanip>

/*
 * Интерфейс для сотрудников, которые могут получать бонусы
 */
class IBonus {
public:
    /*
     * Применяет бонус к заработной плате
     *
     * @param amount размер бонуса
     */
    virtual void applyBonus(double amount) = 0;
    virtual ~IBonus() = default;
};

/*
 * Абстрактный базовый класс сотрудника
 */
class Employee {
protected:
    int id;
    std::string name;
public:
    Employee(int id, const std::string& name) : id(id), name(name) {}
    virtual ~Employee() = default;

    int getId() const { return id; }
    const std::string& getName() const { return name; }

    /*
     * Вычисляет заработную плату сотрудника
     *
     * @return возвращает сумму заработной платы
     */
    virtual double calculateSalary() const = 0;

    /*
     * Выводит информацию о сотруднике
     */
    virtual void printInfo() const = 0;
};

/*
 * Класс сотрудника с полной занятостью
 */
class FullTimeEmployee : public Employee, public IBonus {
    double monthlySalary;
public:
    FullTimeEmployee(int id, const std::string& name, double monthlySalary)
        : Employee(id, name), monthlySalary(monthlySalary) {
    }

    double calculateSalary() const override { return monthlySalary; }

    /*
     * Применяет бонус к месячной зарплате
     *
     * @param amount размер бонуса
     */
    void applyBonus(double amount) override {
        if (amount < 0) return;
        monthlySalary += amount;
    }

    void printInfo() const override {
        std::cout << "[FullTime] ID=" << id
            << ", Name='" << name << "'"
            << ", Salary=" << std::fixed << std::setprecision(2) << calculateSalary()
            << "\n";
    }
};

/*
 * Класс сотрудника с частичной занятостью
 */
class PartTimeEmployee : public Employee {
    double hourlyRate;
    double hoursWorked;
public:
    PartTimeEmployee(int id, const std::string& name, double hourlyRate, double hoursWorked)
        : Employee(id, name), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {
    }

    /*
     * Вычисляет зарплату как произведение часовой ставки на отработанные часы
     *
     * @return возвращает сумму заработной платы
     */
    double calculateSalary() const override { return hourlyRate * hoursWorked; }

    void printInfo() const override {
        std::cout << "[PartTime] ID=" << id
            << ", Name='" << name << "'"
            << ", Hours=" << hoursWorked
            << ", Rate=" << hourlyRate
            << ", Salary=" << std::fixed << std::setprecision(2) << calculateSalary()
            << "\n";
    }
};

/*
 * Класс сотрудника по контракту
 */
class ContractEmployee : public Employee, public IBonus {
    double contractAmount;
public:
    ContractEmployee(int id, const std::string& name, double contractAmount)
        : Employee(id, name), contractAmount(contractAmount) {
    }

    double calculateSalary() const override { return contractAmount; }

    /*
     * Применяет бонус к контрактной сумме
     *
     * @param amount размер бонуса
     */
    void applyBonus(double amount) override {
        if (amount < 0) return;
        contractAmount += amount;
    }

    void printInfo() const override {
        std::cout << "[Contract] ID=" << id
            << ", Name='" << name << "'"
            << ", Payout=" << std::fixed << std::setprecision(2) << calculateSalary()
            << "\n";
    }
};

/*
 * Класс отдела для управления сотрудниками
 */
class Department {
    std::string name;
    std::vector<std::shared_ptr<Employee>> employees;
public:
    /*
     * Создает отдел с заданным названием
     *
     * @param name название отдела
     */
    explicit Department(const std::string& name) : name(name) {}

    const std::string& getName() const { return name; }

    /*
     * Добавляет сотрудника в отдел (предпочтительная перегрузка)
     *
     * @param emp shared_ptr на сотрудника
     */
    void addEmployee(const std::shared_ptr<Employee>& emp) {
        if (emp) employees.push_back(emp);
    }

    /*
     * Добавляет сотрудника в отдел (удобная перегрузка)
     *
     * @param emp указатель на сотрудника
     */
    void addEmployee(Employee* emp) {
        if (emp) employees.emplace_back(std::shared_ptr<Employee>(emp));
    }

    /*
     * Удаляет сотрудника по идентификатору
     *
     * @param targetId идентификатор сотрудника для удаления
     * @return возвращает true если сотрудник был удален, false если не найден
     */
    bool removeEmployee(int targetId) {
        auto before = employees.size();
        employees.erase(
            std::remove_if(employees.begin(), employees.end(), [targetId](const std::shared_ptr<Employee>& e) {
                return e->getId() == targetId;
                }),
            employees.end()
        );
        return employees.size() != before;
    }

    /*
     * Вычисляет общий бюджет на зарплаты в отделе
     *
     * @return возвращает сумму всех зарплат сотрудников отдела
     */
    double totalSalaryBudget() const {
        double sum = 0.0;
        for (const auto& e : employees) sum += e->calculateSalary();
        return sum;
    }

    /*
     * Возвращает список всех сотрудников отдела
     *
     * @return константная ссылка на вектор сотрудников
     */
    const std::vector<std::shared_ptr<Employee>>& list() const { return employees; }
};

/*
 * Выводит информацию о зарплатах сотрудников
 *
 * @param team вектор сотрудников
 * @param title заголовок для вывода
 */
static void printSalaries(const std::vector<std::shared_ptr<Employee>>& team, const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
    for (const auto& e : team) {
        e->printInfo();
    }
}

/*
 * Главная функция - демонстрация работы системы управления сотрудниками
 *
 * @return возвращает 0 при успешном выполнении
 */
int main() {
    // Демонстрация полиморфизма
    std::vector<std::shared_ptr<Employee>> staff;
    staff.push_back(std::make_shared<FullTimeEmployee>(1, "Alice", 120000));
    staff.push_back(std::make_shared<PartTimeEmployee>(2, "Bob", 50, 80));
    staff.push_back(std::make_shared<ContractEmployee>(3, "Charlie", 60000));

    printSalaries(staff, "Initial salaries (polymorphism demo)");

    // Управление отделом и симуляция
    Department rnd("R&D");
    for (const auto& e : staff) rnd.addEmployee(e);

    std::cout << "\nDepartment '" << rnd.getName() << "' total budget: "
        << std::fixed << std::setprecision(2) << rnd.totalSalaryBudget() << "\n";

    // Применение бонусов для поддерживающих сотрудников
    double ftBonus = 5000.0;
    double ctBonus = 2000.0;
    for (const auto& e : rnd.list()) {
        if (auto bonusCapable = dynamic_cast<IBonus*>(e.get())) {
            // Эвристика: больший бонус для полной занятости, меньший для контракта
            // Определение типа через dynamic_cast для выбора суммы
            if (dynamic_cast<FullTimeEmployee*>(e.get())) {
                bonusCapable->applyBonus(ftBonus);
            }
            else {
                bonusCapable->applyBonus(ctBonus);
            }
        }
    }

    printSalaries(rnd.list(), "After bonuses");
    std::cout << "\nUpdated total budget: " << std::fixed << std::setprecision(2) << rnd.totalSalaryBudget() << "\n";

    // Удаление сотрудника и показ обновленного бюджета
    bool removed = rnd.removeEmployee(2); // удаляем Bob (PartTime)
    std::cout << (removed ? "Removed employee with ID=2" : "Employee with ID=2 not found") << "\n";
    printSalaries(rnd.list(), "After removal");
    std::cout << "Final total budget: " << std::fixed << std::setprecision(2) << rnd.totalSalaryBudget() << "\n";

    return 0;
}