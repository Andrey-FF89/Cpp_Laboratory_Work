#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <locale>

/**
 * Структура, представляющая книгу в библиотечном каталоге
 */
struct Book {
    std::string isbn;        // Уникальный идентификатор формата "XXX-X-XXX-XXXXX-X"
    std::string title;       // Название книги
    std::string author;      // Имя автора
    int year;               // Год публикации
    bool is_available;      // Флаг доступности для выдачи
    int pages;              // Количество страниц
};

/**
 * Конвертация строки в нижний регистр для std::string
 */
std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

/**
 * Генерация случайного ISBN формата "XXX-X-XXX-XXXXX-X"
 */
std::string generate_random_isbn() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> digit(0, 9);

    std::string isbn;

    for (int i = 0; i < 3; i++) isbn += std::to_string(digit(gen));
    isbn += '-';
    isbn += std::to_string(digit(gen));
    isbn += '-';
    for (int i = 0; i < 3; i++) isbn += std::to_string(digit(gen));
    isbn += '-';
    for (int i = 0; i < 5; i++) isbn += std::to_string(digit(gen));
    isbn += '-';
    isbn += std::to_string(digit(gen));

    return isbn;
}

/**
 * Генерация случайного названия книги
 */
std::string generate_random_title() {
    std::vector<std::string> first_words = { "The", "A", "My", "Your", "Our", "Their", "His", "Her" };
    std::vector<std::string> adjectives = { "Great", "Secret", "Last", "Dark", "Bright",
                                          "Forgotten", "Ancient", "Modern", "Lost", "Eternal",
                                          "Mysterious", "Hidden", "Golden", "Silver", "Final" };
    std::vector<std::string> nouns = { "Castle", "Forest", "City", "Island", "Key",
                                     "Sword", "Artifact", "Prophet", "Warrior", "Dragon",
                                     "Journey", "Adventure", "Legend", "Mystery", "Dream" };
    std::vector<std::string> of_phrases = { "of Destiny", "of Time", "of Fire", "of Ice", "of Darkness",
                                         "of Light", "of Magic", "of Technology", "of the Past", "of the Future",
                                         "of Wisdom", "of Courage", "of Hope", "of Power", "of Freedom" };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 7); // для first_words
    std::uniform_int_distribution<> dist2(0, 14); // для остальных

    // Разные форматы названий для разнообразия
    int format = std::uniform_int_distribution<>(0, 2)(gen);

    if (format == 0) {
        return first_words[dist(gen)] + " " + adjectives[dist2(gen)] + " " + nouns[dist2(gen)];
    }
    else if (format == 1) {
        return "The " + nouns[dist2(gen)] + " " + of_phrases[dist2(gen)];
    }
    else {
        return adjectives[dist2(gen)] + " " + nouns[dist2(gen)];
    }
}

/**
 * Генерация случайного имени автора
 */
std::string generate_random_author() {
    std::vector<std::string> first_names = { "James", "John", "Robert", "Michael", "William",
                                           "David", "Richard", "Charles", "Joseph", "Thomas",
                                           "Mary", "Patricia", "Jennifer", "Linda", "Elizabeth",
                                           "Barbara", "Susan", "Jessica", "Sarah", "Karen" };

    std::vector<std::string> last_names = { "Smith", "Johnson", "Williams", "Brown", "Jones",
                                          "Miller", "Davis", "Garcia", "Rodriguez", "Wilson",
                                          "Martinez", "Anderson", "Taylor", "Thomas", "Moore",
                                          "Jackson", "Martin", "Lee", "Thompson", "White" };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> first_dist(0, first_names.size() - 1);
    std::uniform_int_distribution<> last_dist(0, last_names.size() - 1);

    return first_names[first_dist(gen)] + " " + last_names[last_dist(gen)];
}

/**
 * Создание и заполнение массива книг случайными данными
 */
std::vector<Book> create_books_array(int n) {
    std::vector<Book> books;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> year_dist(1900, 2023);
    std::uniform_int_distribution<> pages_dist(100, 800);
    std::uniform_int_distribution<> bool_dist(0, 1);

    for (int i = 0; i < n; i++) {
        Book book;
        book.isbn = generate_random_isbn();
        book.title = generate_random_title();
        book.author = generate_random_author();
        book.year = year_dist(gen);
        book.is_available = bool_dist(gen);
        book.pages = pages_dist(gen);

        books.push_back(book);
    }

    return books;
}

/**
 * Поиск книг по автору и названию (содержит подстроку)
 */
std::vector<Book> search_by_author_and_title(const std::vector<Book>& books, const std::string& search_string) {
    std::vector<Book> result;
    std::string lower_search = to_lower(search_string);

    for (const auto& book : books) {
        std::string lower_author = to_lower(book.author);
        std::string lower_title = to_lower(book.title);

        if (lower_author.find(lower_search) != std::string::npos ||
            lower_title.find(lower_search) != std::string::npos) {
            result.push_back(book);
        }
    }

    return result;
}

/**
 * Фильтрация книг по году публикации (между двумя годами)
 */
std::vector<Book> filter_by_year(const std::vector<Book>& books, int start_year, int end_year) {
    std::vector<Book> result;

    for (const auto& book : books) {
        if (book.year >= start_year && book.year <= end_year) {
            result.push_back(book);
        }
    }

    return result;
}

/**
 * Вывод статистики по книгам
 */
void print_statistics(const std::vector<Book>& books) {
    if (books.empty()) {
        std::cout << "No books for analysis." << std::endl;
        return;
    }

    int total_books = books.size();
    int total_pages = 0;
    int available_books = 0;
    int unavailable_books = 0;

    for (const auto& book : books) {
        total_pages += book.pages;
        if (book.is_available) {
            available_books++;
        }
        else {
            unavailable_books++;
        }
    }

    double average_pages = static_cast<double>(total_pages) / total_books;

    std::cout << "\n=== CATALOG STATISTICS ===" << std::endl;
    std::cout << "Total books: " << total_books << std::endl;
    std::cout << "Average pages: " << std::fixed << std::setprecision(1) << average_pages << std::endl;
    std::cout << "Available books: " << available_books << std::endl;
    std::cout << "Unavailable books: " << unavailable_books << std::endl;
}

/**
 * Сравнение книг для сортировки (по году и автору)
 */
bool compare_books(const Book& a, const Book& b) {
    if (a.year != b.year) {
        return a.year > b.year; // Новые сначала
    }
    return a.author < b.author; // При одинаковом годе - по автору
}

/**
 * Сортировка массива книг по году (новые сначала) и автору
 */
void sort_books(std::vector<Book>& books) {
    std::sort(books.begin(), books.end(), compare_books);
}

/**
 * Выдача книги по ISBN
 */
bool borrow_book(std::vector<Book>& books, const std::string& isbn) {
    for (auto& book : books) {
        if (book.isbn == isbn) {
            if (book.is_available) {
                book.is_available = false;
                std::cout << "Book with ISBN " << isbn << " successfully borrowed." << std::endl;
                return true;
            }
            else {
                std::cout << "Error: book with ISBN " << isbn << " is already borrowed." << std::endl;
                return false;
            }
        }
    }

    std::cout << "Error: book with ISBN " << isbn << " not found." << std::endl;
    return false;
}

/**
 * Возврат книги по ISBN
 */
bool return_book(std::vector<Book>& books, const std::string& isbn) {
    for (auto& book : books) {
        if (book.isbn == isbn) {
            if (!book.is_available) {
                book.is_available = true;
                std::cout << "Book with ISBN " << isbn << " successfully returned." << std::endl;
                return true;
            }
            else {
                std::cout << "Error: book with ISBN " << isbn << " is already available." << std::endl;
                return false;
            }
        }
    }

    std::cout << "Error: book with ISBN " << isbn << " not found." << std::endl;
    return false;
}

/**
 * Вывод информации о книге в консоль
 */
void print_book(const Book& book) {
    std::cout << "ISBN: " << book.isbn << std::endl;
    std::cout << "Title: " << book.title << std::endl;
    std::cout << "Author: " << book.author << std::endl;
    std::cout << "Year: " << book.year << std::endl;
    std::cout << "Pages: " << book.pages << std::endl;
    std::cout << "Status: " << (book.is_available ? "Available" : "Borrowed") << std::endl;
    std::cout << "------------------------" << std::endl;
}

/**
 * Вывод массива книг в консоль
 */
void print_books(const std::vector<Book>& books, const std::string& title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
    if (books.empty()) {
        std::cout << "No books found." << std::endl;
        return;
    }

    for (const auto& book : books) {
        print_book(book);
    }
}

/**
 * Основная функция программы
 */
int main() {
    // Установка локали для корректного отображения символов
    std::locale::global(std::locale(""));

    int n;
    std::cout << "Enter number of books to generate: ";
    std::cin >> n;
    std::cin.ignore();

    if (n <= 0) {
        std::cout << "Error: number of books must be positive." << std::endl;
        return 1;
    }

    // Создание массива книг
    std::vector<Book> books = create_books_array(n);
    std::cout << "Generated " << n << " books." << std::endl;

    // Главное меню
    int choice;
    do {
        std::cout << "\n=== LIBRARY CATALOG ===" << std::endl;
        std::cout << "1. Search by author and title" << std::endl;
        std::cout << "2. Filter by year" << std::endl;
        std::cout << "3. Statistics" << std::endl;
        std::cout << "4. Sort by year and author" << std::endl;
        std::cout << "5. Borrow book" << std::endl;
        std::cout << "6. Return book" << std::endl;
        std::cout << "7. Show all books" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose action: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            std::string search_string;
            std::cout << "Enter search string: ";
            std::getline(std::cin, search_string);
            std::vector<Book> found_books = search_by_author_and_title(books, search_string);
            print_books(found_books, "SEARCH RESULTS: " + search_string);
            break;
        }

        case 2: {
            int start_year, end_year;
            std::cout << "Enter start year: ";
            std::cin >> start_year;
            std::cout << "Enter end year: ";
            std::cin >> end_year;
            std::vector<Book> filtered_books = filter_by_year(books, start_year, end_year);
            print_books(filtered_books, "BOOKS FROM " + std::to_string(start_year) + " TO " + std::to_string(end_year));
            break;
        }

        case 3:
            print_statistics(books);
            break;

        case 4:
            sort_books(books);
            std::cout << "Books sorted by year (newest first) and author." << std::endl;
            print_books(books, "SORTED CATALOG");
            break;

        case 5: {
            std::string isbn;
            std::cout << "Enter ISBN to borrow: ";
            std::getline(std::cin, isbn);
            borrow_book(books, isbn);
            break;
        }

        case 6: {
            std::string isbn;
            std::cout << "Enter ISBN to return: ";
            std::getline(std::cin, isbn);
            return_book(books, isbn);
            break;
        }

        case 7:
            print_books(books, "ALL BOOKS");
            break;

        case 0:
            std::cout << "Exiting program." << std::endl;
            break;

        default:
            std::cout << "Invalid choice. Try again." << std::endl;
        }

    } while (choice != 0);

    return 0;
}