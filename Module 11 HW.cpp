#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

// Класс Трактат
class Treatise {
public:
    std::string title;
    std::string author;
    std::string ISBN;
    bool isAvailable;

    Treatise(const std::string& title, const std::string& author, const std::string& ISBN, bool isAvailable = true)
        : title(title), author(author), ISBN(ISBN), isAvailable(isAvailable) {}
};

// Класс Ученик
class Acolyte {
public:
    std::string name;
    std::vector<Treatise*> borrowedTreatises;
    static const int maxTreatises = 3; // Ограничение на количество трактатов

    explicit Acolyte(const std::string& name) : name(name) {}

    bool borrowTreatise(Treatise& treatise) {
        if (borrowedTreatises.size() >= maxTreatises) {
            std::cout << "Ученик " << name << " достиг лимита на аренду трактатов!\n";
            return false;
        }
        if (!treatise.isAvailable) {
            std::cout << "Трактат \"" << treatise.title << "\" недоступен для аренды.\n";
            return false;
        }
        treatise.isAvailable = false;
        borrowedTreatises.push_back(&treatise);
        std::cout << "Ученик " << name << " арендовал трактат \"" << treatise.title << "\".\n";
        return true;
    }

    void returnTreatise(Treatise& treatise) {
        auto it = std::find(borrowedTreatises.begin(), borrowedTreatises.end(), &treatise);
        if (it != borrowedTreatises.end()) {
            treatise.isAvailable = true;
            borrowedTreatises.erase(it);
            std::cout << "Ученик " << name << " вернул трактат \"" << treatise.title << "\".\n";
        } else {
            std::cout << "Трактат \"" << treatise.title << "\" не был арендован учеником " << name << ".\n";
        }
    }
};

// Класс Архивариус
class Archivist {
public:
    std::string name;

    explicit Archivist(const std::string& name) : name(name) {}

    void addTreatise(std::vector<Treatise>& treatises, const Treatise& treatise) {
        treatises.push_back(treatise);
        std::cout << "Архивариус " << name << " добавил трактат \"" << treatise.title << "\" в хранилище.\n";
    }

    void addTreatise(class Repository& repository, const Treatise& treatise);
};

// Класс Хранилище
class Repository {
private:
    std::vector<Treatise> treatises;

public:
    void addTreatise(const Treatise& treatise) {
        treatises.push_back(treatise);
    }

    void displayTreatises(bool includeRented = false) const {
        std::cout << "Список трактатов в хранилище:\n";
        for (const auto& treatise : treatises) {
            if (includeRented || treatise.isAvailable) {
                std::cout << "- \"" << treatise.title << "\" от " << treatise.author
                          << " (ISBN: " << treatise.ISBN << ")"
                          << (treatise.isAvailable ? " [В наличии]\n" : " [Арендован]\n");
            }
        }
    }

    Treatise* findTreatiseByTitle(const std::string& title) {
        for (auto& treatise : treatises) {
            if (treatise.title == title) {
                return &treatise;
            }
        }
        std::cout << "Трактат \"" << title << "\" не найден.\n";
        return nullptr;
    }

    std::vector<Treatise>& getTreatises() { return treatises; }
};

// Определение метода addTreatise для работы с Repository
void Archivist::addTreatise(Repository& repository, const Treatise& treatise) {
    repository.addTreatise(treatise);
    std::cout << "Архивариус " << name << " добавил трактат \"" << treatise.title << "\" в хранилище.\n";
}

// Точка входа
int main() {
    Repository repository;

    // Создание архивариуса
    Archivist archivist("Брат Тарк");

    // Добавление трактатов в хранилище
    archivist.addTreatise(repository, {"Codex Astartes", "Робуте Гиллиман", "WH40K-0001"});
    archivist.addTreatise(repository, {"Lectitio Divinitatus", "Неизвестный автор", "WH40K-0002"});
    archivist.addTreatise(repository, {"Damnation of Pythos", "Дэвид Эннандейл", "WH40K-0003"});

    // Вывод всех трактатов
    repository.displayTreatises();

    // Ученик берет трактат
    Acolyte acolyte("Неофит Каин");
    Treatise* treatise = repository.findTreatiseByTitle("Codex Astartes");
    if (treatise) acolyte.borrowTreatise(*treatise);

    // Вывод трактатов после аренды
    repository.displayTreatises();

    // Ученик возвращает трактат
    if (treatise) acolyte.returnTreatise(*treatise);

    // Вывод трактатов после возврата
    repository.displayTreatises();

    return 0;
}
