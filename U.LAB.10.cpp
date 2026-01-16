#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <iomanip>

// --- Иерархия исключений ---
class TaskManagerException : public std::runtime_error {
public:
    TaskManagerException(const std::string& msg) : std::runtime_error(msg) {}
};

class DuplicateTaskError : public TaskManagerException {
public:
    DuplicateTaskError(const std::string& description)
        : TaskManagerException("Error: Task '" + description + "' already exists.") {
    }
};

class TaskNotFoundError : public TaskManagerException {
public:
    TaskNotFoundError(int index)
        : TaskManagerException("Error: Task with index " + std::to_string(index) + " not found or index is invalid.") {
    }

    TaskNotFoundError(const std::string& description)
        : TaskManagerException("Error: Task '" + description + "' not found.") {
    }
};

class NoTasksError : public TaskManagerException {
public:
    NoTasksError() : TaskManagerException("Error: Task manager is empty. No tasks to retrieve.") {}
};


// --- Структура задачи ---
struct Task {
    std::string description;
    int priority; // Выше число - выше приоритет
    bool isCompleted = false;

    Task(std::string desc, int prio) : description(std::move(desc)), priority(prio) {}

    bool operator<(const Task& other) const {
        return priority < other.priority;
    }
};


// --- Класс менеджера задач ---
class TaskManager {
private:
    std::vector<Task> tasks_;

    /**
     * @brief Находит задачу по описанию.
     * @param description Описание задачи.
     * @return Итератор на найденную задачу или tasks_.end().
     */
    std::vector<Task>::iterator findTask(const std::string& description) {
        return std::find_if(tasks_.begin(), tasks_.end(),
            [&description](const Task& t) {
                return t.description == description;
            });
    }

    /**
     * @brief Сортирует вектор по приоритету (наивысший в начале).
     */
    void sortTasks() {
        std::sort(tasks_.begin(), tasks_.end(), [](const Task& t1, const Task& t2) {
            return t1.priority > t2.priority;
            });
    }

public:
    /**
     * @brief Добавляет новую задачу.
     * @param description Описание задачи.
     * @param priority Приоритет (целое число).
     * @throws DuplicateTaskError Если задача с таким описанием уже существует.
     */
    void addTask(const std::string& description, int priority) {
        if (findTask(description) != tasks_.end()) {
            throw DuplicateTaskError(description);
        }
        tasks_.emplace_back(description, priority);
        sortTasks();
    }

    /**
     * @brief Отмечает задачу как выполненную по ее индексу в отсортированном списке.
     * @param index Индекс задачи (начиная с 0).
     * @throws TaskNotFoundError Если индекс неверный.
     * @throws TaskNotFoundError Если задача уже выполнена.
     */
    void completeTask(size_t index) {
        if (index >= tasks_.size()) {
            throw TaskNotFoundError(static_cast<int>(index));
        }

        Task& task = tasks_[index];
        if (task.isCompleted) {
            throw TaskNotFoundError("Task '" + task.description + "' is already marked completed.");
        }

        task.isCompleted = true;

        tasks_.erase(tasks_.begin() + index);
    }

    /**
     * @brief Получает следующую задачу с наивысшим приоритетом (первую в списке).
     * @return Константная ссылка на следующую задачу.
     * @throws NoTasksError Если список задач пуст.
     */
    const Task& getNextTask() const {
        if (tasks_.empty()) {
            throw NoTasksError();
        }
        return tasks_.front();
    }

    /**
     * @brief Удаляет и возвращает следующую задачу с наивысшим приоритетом.
     * @return Следующая задача.
     * @throws NoTasksError Если список задач пуст.
     */
    Task popNextTask() {
        if (tasks_.empty()) {
            throw NoTasksError();
        }
        Task nextTask = tasks_.front();
        tasks_.erase(tasks_.begin());
        return nextTask;
    }

    /**
     * @brief Выводит текущий список задач.
     */
    void displayTasks() const {
        std::cout << "\n--- Current Task List (" << tasks_.size() << " items) ---" << std::endl;
        if (tasks_.empty()) {
            std::cout << "The task list is empty." << std::endl;
        }
        for (size_t i = 0; i < tasks_.size(); ++i) {
            std::cout << "[" << i << "] Prio: " << std::setw(2) << tasks_[i].priority
                << " | Desc: " << tasks_[i].description
                << (tasks_[i].isCompleted ? " (COMPLETED)" : "") << std::endl;
        }
        std::cout << "-----------------------------------" << std::endl;
    }
};

// --- Функция-исполнитель ---

/**
 * @brief Имитирует цикл выполнения задач до тех пор, пока менеджер не опустеет.
 * @param manager Ссылка на TaskManager.
 */
static void taskExecutor(TaskManager& manager) {
    std::cout << "\n--- TASK EXECUTOR STARTED ---" << std::endl;
    try {
        while (true) {
            Task currentTask = manager.popNextTask();

            std::cout << "Executing: [Prio " << currentTask.priority << "] - '"
                << currentTask.description << "'..." << std::endl;
        }
    }
    catch (const NoTasksError& e) {
        std::cout << "\n[GRACEFUL SHUTDOWN] " << e.what() << std::endl;
    }
    catch (const TaskManagerException& e) {
        std::cerr << "[ERROR] Unexpected TaskManager Exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[FATAL] Unexpected standard exception: " << e.what() << std::endl;
    }
    std::cout << "--- TASK EXECUTOR STOPPED ---" << std::endl;
}

int main() {
    TaskManager manager;

    std::cout << "--- BASE DEMO: Adding Tasks ---" << std::endl;
    manager.addTask("Review Code", 80);
    manager.addTask("Fix Critical Bug", 100);
    manager.addTask("Write Documentation", 20);
    manager.addTask("Refactor Legacy Module", 50);

    manager.displayTasks();

    try {
        std::cout << "\nAttempting to add duplicate task..." << std::endl;
        manager.addTask("Review Code", 80); // Попытка добавить существующую задачу
    }
    catch (const DuplicateTaskError& e) {
        std::cerr << "[CAUGHT] " << e.what() << std::endl;
    }

    try {
        const Task& next = manager.getNextTask();
        std::cout << "\nNext highest priority task (peek): [Prio " << next.priority << "] - '"
            << next.description << "'" << std::endl;
    }
    catch (const TaskManagerException& e) {
        std::cerr << "[CAUGHT] " << e.what() << std::endl;
    }

    try {
        std::cout << "\nAttempting to complete task at index 1 (Review Code)..." << std::endl;
        manager.completeTask(1);
    }
    catch (const TaskNotFoundError& e) {
        std::cerr << "[CAUGHT] " << e.what() << std::endl;
    }

    manager.displayTasks();

    // Обработка TaskNotFoundError
    try {
        std::cout << "\nAttempting to complete task at invalid index 10..." << std::endl;
        manager.completeTask(10); // Неверный индекс
    }
    catch (const TaskNotFoundError& e) {
        std::cerr << "[CAUGHT] " << e.what() << std::endl;
    }

    // Запускаем цикл выполнения
    taskExecutor(manager);

    manager.displayTasks();

    // Обработка NoTasksError
    try {
        manager.getNextTask();
    }
    catch (const NoTasksError& e) {
        std::cout << "\n[FINAL CHECK] Successfully caught: " << e.what() << std::endl;
    }

    return 0;
}