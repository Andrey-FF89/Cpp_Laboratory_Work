#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <stdexcept>

// Используем псевдонимы для ясности
using MediaId = std::string;
using UserId = std::string;

// --- 1. Интерфейсы ---

/**
 * @interface IMedia
 * Абстрактный базовый класс (интерфейс) для всех медиа-ресурсов.
 */
class IMedia {
public:
    virtual ~IMedia() = default;
    virtual MediaId getId() const = 0;
    virtual std::string getTitle() const = 0;
    virtual std::string getType() const = 0;
    virtual void printDetails() const = 0;
};

/**
 * @interface IMediaRepository
 * Интерфейс Репозитория для управления коллекцией медиа-ресурсов.
 */
class IMediaRepository {
public:
    virtual ~IMediaRepository() = default;
    virtual bool addMedia(std::unique_ptr<IMedia> media) = 0;
    virtual bool removeMedia(const MediaId& id) = 0;
    virtual const IMedia* findMedia(const MediaId& id) const = 0;
    virtual std::vector<const IMedia*> getAllMedia() const = 0;
    virtual bool exists(const MediaId& id) const = 0;
};

// --- 2. Классы Медиа-Ресурсов ---

/**
 * @class Book
 * Конкретная реализация IMedia для Книг. Дополнительно хранит имя автора.
 */
class Book : public IMedia {
private:
    MediaId id_;
    std::string title_;
    std::string author_;

public:
    Book(const MediaId& id, const std::string& title, const std::string& author)
        : id_(id), title_(title), author_(author) {
    }

    MediaId getId() const override { return id_; }
    std::string getTitle() const override { return title_; }
    std::string getType() const override { return "Book"; }

    void printDetails() const override {
        std::cout << "Book (ID: " << id_ << ", Title: " << title_ << ", Author: " << author_ << ")";
    }
};

/**
 * @class Audio
 * Конкретная реализация IMedia для Аудиозаписей. Дополнительно хранит длительность.
 */
class Audio : public IMedia {
private:
    MediaId id_;
    std::string title_;
    int durationSeconds_;

public:
    Audio(const MediaId& id, const std::string& title, int duration)
        : id_(id), title_(title), durationSeconds_(duration) {
    }

    MediaId getId() const override { return id_; }
    std::string getTitle() const override { return title_; }
    std::string getType() const override { return "Audio"; }

    void printDetails() const override {
        std::cout << "Audio (ID: " << id_ << ", Title: " << title_
            << ", Duration: " << durationSeconds_ << "s)";
    }
    int getDuration() const { return durationSeconds_; }
};

// --- 3. Класс Репозитория ---

/**
 * @class InMemoryMediaRepository
 * Реализация IMediaRepository с хранением в памяти (вектор + map-индекс).
 */
class InMemoryMediaRepository : public IMediaRepository {
private:
    std::vector<std::unique_ptr<IMedia>> storage_;
    std::map<MediaId, size_t> index_;

public:
    bool addMedia(std::unique_ptr<IMedia> media) override {
        if (!media) return false;
        const MediaId id = media->getId();

        if (index_.count(id)) {
            return false;
        }

        index_[id] = storage_.size();
        storage_.push_back(std::move(media));
        return true;
    }

    bool removeMedia(const MediaId& id) override {
        if (index_.count(id)) {
            size_t idx = index_.at(id);
            storage_[idx].reset(); // Освобождаем объект
            index_.erase(id);
            return true;
        }
        return false;
    }

    const IMedia* findMedia(const MediaId& id) const override {
        if (index_.count(id)) {
            size_t idx = index_.at(id);
            return storage_[idx].get();
        }
        return nullptr;
    }

    std::vector<const IMedia*> getAllMedia() const override {
        std::vector<const IMedia*> result;
        for (const auto& ptr : storage_) {
            if (ptr) {
                result.push_back(ptr.get());
            }
        }
        return result;
    }

    bool exists(const MediaId& id) const override {
        return index_.count(id) > 0;
    }
};

// --- 4. Усложненный этап: Пользователи и Менеджер ---

/**
 * @class User
 * Класс, представляющий пользователя библиотеки.
 */
class User {
private:
    UserId id_;
    std::string name_;
    std::vector<MediaId> issuedMediaIds_; // Список ID выданных медиа

public:
    User(const UserId& id, const std::string& name) : id_(id), name_(name) {}

    UserId getId() const { return id_; }
    std::string getName() const { return name_; }

    void issueMedia(const MediaId& mediaId) {
        issuedMediaIds_.push_back(mediaId);
    }

    // ДОБАВЛЕНИЕ: Метод для возврата медиа-ресурса
    void returnMedia(const MediaId& mediaId) {
        auto it = std::remove(issuedMediaIds_.begin(), issuedMediaIds_.end(), mediaId);
        issuedMediaIds_.erase(it, issuedMediaIds_.end());
    }

    bool hasMedia(const MediaId& mediaId) const {
        return std::find(issuedMediaIds_.begin(), issuedMediaIds_.end(), mediaId) != issuedMediaIds_.end();
    }

    void printDetails() const {
        std::cout << "User (ID: " << id_ << ", Name: " << name_
            << ", Issued: " << issuedMediaIds_.size() << " items)";
    }

    const std::vector<MediaId>& getIssuedMediaIds() const {
        return issuedMediaIds_;
    }
};

/**
 * @class InMemoryUserManager
 * Менеджер, управляющий коллекцией пользователей.
 */
class InMemoryUserManager {
private:
    std::map<UserId, std::unique_ptr<User>> users_;

public:
    bool addUser(std::unique_ptr<User> user) {
        if (!user || users_.count(user->getId())) {
            return false;
        }
        users_[user->getId()] = std::move(user);
        return true;
    }

    bool exists(const UserId& id) const {
        return users_.count(id) > 0;
    }

    User* getUser(const UserId& id) {
        if (users_.count(id)) {
            return users_[id].get();
        }
        return nullptr;
    }

    bool isMediaIssued(const MediaId& mediaId) const {
        for (const auto& pair : users_) {
            if (pair.second->hasMedia(mediaId)) {
                return true;
            }
        }
        return false;
    }

    std::vector<const User*> getAllUsers() const {
        std::vector<const User*> all;
        for (const auto& pair : users_) {
            all.push_back(pair.second.get());
        }
        return all;
    }
};

// --- 5. Класс Library (Агрегатор) ---

/**
 * @class Library
 * Агрегирующий компонент (фасад).
 */
class Library {
private:
    IMediaRepository& mediaRepo_;
    InMemoryUserManager& userManager_;

public:
    Library(IMediaRepository& repo, InMemoryUserManager& manager)
        : mediaRepo_(repo), userManager_(manager) {
    }

    bool issueMediaToUser(const MediaId& mediaId, const UserId& userId) {
        User* user = userManager_.getUser(userId);
        if (!user) {
            std::cerr << "Error: User ID " << userId << " not found." << std::endl;
            return false;
        }

        const IMedia* media = mediaRepo_.findMedia(mediaId);
        if (!media) {
            std::cerr << "Error: Media ID " << mediaId << " not found in repository." << std::endl;
            return false;
        }

        if (userManager_.isMediaIssued(mediaId)) {
            std::cerr << "Error: Media ID " << mediaId << " is already issued to another user." << std::endl;
            return false;
        }

        user->issueMedia(mediaId);
        std::cout << "Success: Media '" << media->getTitle()
            << "' issued to user " << user->getName() << "." << std::endl;
        return true;
    }

    // НОВАЯ ОПЕРАЦИЯ: Возврат медиа
    bool returnMediaFromUser(const MediaId& mediaId, const UserId& userId) {
        User* user = userManager_.getUser(userId);
        if (!user) {
            std::cerr << "Error: User ID " << userId << " not found." << std::endl;
            return false;
        }

        if (!user->hasMedia(mediaId)) {
            std::cerr << "Error: Media ID " << mediaId << " was not issued to user " << user->getName() << "." << std::endl;
            return false;
        }

        user->returnMedia(mediaId);
        std::cout << "Success: Media ID " << mediaId << " returned by user " << user->getName() << "." << std::endl;
        return true;
    }

    // Вспомогательный метод для отображения всего статуса
    void printAllStatus() const {
        std::cout << "\n--- Current Library Status ---" << std::endl;

        // 1. Медиа
        auto allMedia = mediaRepo_.getAllMedia();
        std::cout << "Available Media (" << allMedia.size() << " total):" << std::endl;
        if (allMedia.empty()) { std::cout << "  (Empty)" << std::endl; }
        for (const auto* media : allMedia) {
            bool issued = userManager_.isMediaIssued(media->getId());
            std::cout << "  - ID: " << media->getId() << ", Title: " << media->getTitle()
                << " (" << media->getType() << ") [" << (issued ? "**ISSUED**" : "Available") << "]" << std::endl;
        }

        // 2. Пользователи
        auto allUsers = userManager_.getAllUsers();
        std::cout << "\nUsers (" << allUsers.size() << " total):" << std::endl;
        if (allUsers.empty()) { std::cout << "  (Empty)" << std::endl; }
        for (const auto* user : allUsers) {
            std::cout << "  - " << user->getName() << " (ID: " << user->getId() << ")";
            const auto& issued = user->getIssuedMediaIds();
            if (!issued.empty()) {
                std::cout << " Issued Media: [";
                for (size_t i = 0; i < issued.size(); ++i) {
                    std::cout << issued[i] << (i < issued.size() - 1 ? ", " : "");
                }
                std::cout << "]";
            }
            std::cout << std::endl;
        }
        std::cout << "------------------------------" << std::endl;
    }
};


// --- 6. Интерактивное меню ---

// Функция для очистки ввода
void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/**
 * @brief Инициализирует репозиторий тестовыми данными.
 */
void setupInitialData(InMemoryMediaRepository& mediaRepo, InMemoryUserManager& userManager) {
    mediaRepo.addMedia(std::make_unique<Book>("B101", "The Martian", "Andy Weir"));
    mediaRepo.addMedia(std::make_unique<Audio>("A201", "Dune Audiobook", 30000));
    mediaRepo.addMedia(std::make_unique<Book>("B102", "Neuromancer", "William Gibson"));

    userManager.addUser(std::make_unique<User>("U001", "Alice"));
    userManager.addUser(std::make_unique<User>("U002", "Bob"));

    std::cout << "Library initialized with 3 media items and 2 users." << std::endl;
}

/**
 * @brief Отображает главное меню.
 */
void showMenu() {
    std::cout << "\n==============================================" << std::endl;
    std::cout << "               Library Manager" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "1. Add New Media (Book/Audio)" << std::endl;
    std::cout << "2. Remove Media (by ID)" << std::endl;
    std::cout << "3. Add New User" << std::endl;
    std::cout << "4. Issue Media to User" << std::endl;
    std::cout << "5. Return Media from User" << std::endl;
    std::cout << "6. View Full Status" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "Enter choice: ";
}

/**
 * @brief Добавляет новый медиа-ресурс, запрашивая данные у пользователя.
 */
void handleAddMedia(IMediaRepository& mediaRepo) {
    std::string type, id, title;

    std::cout << "Enter Media Type (Book/Audio): ";
    std::cin >> type;
    std::transform(type.begin(), type.end(), type.begin(), ::tolower); // Переводим в нижний регистр

    std::cout << "Enter ID (e.g., B103, A202): ";
    std::cin >> id;
    std::cout << "Enter Title: ";
    clearInput(); // Очистка буфера перед getline
    std::getline(std::cin, title);

    if (type == "book") {
        std::string author;
        std::cout << "Enter Author Name: ";
        std::getline(std::cin, author);
        if (mediaRepo.addMedia(std::make_unique<Book>(id, title, author))) {
            std::cout << "SUCCESS: Book '" << title << "' added." << std::endl;
        }
        else {
            std::cerr << "ERROR: Media ID " << id << " already exists." << std::endl;
        }
    }
    else if (type == "audio") {
        int duration;
        std::cout << "Enter Duration in seconds: ";
        std::cin >> duration;
        if (mediaRepo.addMedia(std::make_unique<Audio>(id, title, duration))) {
            std::cout << "SUCCESS: Audio '" << title << "' added." << std::endl;
        }
        else {
            std::cerr << "ERROR: Media ID " << id << " already exists." << std::endl;
        }
    }
    else {
        std::cerr << "ERROR: Unknown media type." << std::endl;
    }
}

/**
 * @brief Добавляет нового пользователя.
 */
void handleAddUser(InMemoryUserManager& userManager) {
    std::string id, name;
    std::cout << "Enter User ID (e.g., U003): ";
    std::cin >> id;
    std::cout << "Enter User Name: ";
    clearInput();
    std::getline(std::cin, name);

    if (userManager.addUser(std::make_unique<User>(id, name))) {
        std::cout << "SUCCESS: User '" << name << "' added." << std::endl;
    }
    else {
        std::cerr << "ERROR: User ID " << id << " already exists." << std::endl;
    }
}

// --- 7. Главная функция ---

int main() {
    // Создаем компоненты
    InMemoryMediaRepository mediaRepo;
    InMemoryUserManager userManager;
    Library library(mediaRepo, userManager); // Агрегация

    // Инициализация тестовыми данными
    setupInitialData(mediaRepo, userManager);

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(std::cin >> choice)) {
            // Если ввод не удался (например, введена буква)
            std::cin.clear();
            clearInput();
            std::cerr << "Invalid input. Please enter a number." << std::endl;
            continue;
        }

        std::string mediaId, userId;

        switch (choice) {
        case 1: // Add New Media
            handleAddMedia(mediaRepo);
            break;
        case 2: // Remove Media
            std::cout << "Enter Media ID to remove: ";
            std::cin >> mediaId;
            if (library.returnMediaFromUser(mediaId, "")) {} // Проверка и очистка
            if (mediaRepo.removeMedia(mediaId)) {
                std::cout << "SUCCESS: Media " << mediaId << " removed." << std::endl;
            }
            else {
                std::cerr << "ERROR: Media ID " << mediaId << " not found." << std::endl;
            }
            break;
        case 3: // Add New User
            handleAddUser(userManager);
            break;
        case 4: // Issue Media
            std::cout << "Enter Media ID to issue: ";
            std::cin >> mediaId;
            std::cout << "Enter User ID: ";
            std::cin >> userId;
            library.issueMediaToUser(mediaId, userId);
            break;
        case 5: // Return Media
            std::cout << "Enter Media ID to return: ";
            std::cin >> mediaId;
            std::cout << "Enter User ID who returns it: ";
            std::cin >> userId;
            library.returnMediaFromUser(mediaId, userId);
            break;
        case 6: // View Full Status
            library.printAllStatus();
            break;
        case 0:
            std::cout << "Exiting Library Manager. Goodbye!" << std::endl;
            break;
        default:
            std::cerr << "Invalid choice. Please select a number from the menu." << std::endl;
            break;
        }
    }

    return 0;
}