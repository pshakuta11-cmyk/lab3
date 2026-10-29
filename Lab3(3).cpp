#include "Lab3(3).h"
#include <sstream>

int inputInt(string prompt) {
    int value;
    while (true) {
        cout << prompt; cin >> value;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Ошибка!\n"; continue; }
        cin.ignore(1000, '\n'); return value;
    }
}

double inputDouble(string prompt) {
    double value;
    while (true) {
        cout << prompt; cin >> value;
        if (cin.fail()) { cin.clear(); cin.ignore(1000, '\n'); cout << "Ошибка!\n"; continue; }
        cin.ignore(1000, '\n'); return value;
    }
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

string padRight(string str, int width) {
    int visible_length = 0;
    for (char c : str) if ((c & 0xC0) != 0x80) visible_length++;
    if (visible_length < width) str.append(width - visible_length, ' ');
    return str;
}

string formatPrice(double price) {
    stringstream ss;
    if (price == (int)price) ss << (int)price;
    else ss << fixed << setprecision(2) << price;
    ss << "р";
    return ss.str();
}

void Service::input() {
    id = inputInt("ID: ");
    cout << "Название процедуры: "; getline(cin, name);
    price = inputDouble("Цена (руб): ");
    duration = inputInt("Длительность (мин): ");
    cout << "Тип: "; getline(cin, type);
}

void Service::printHeader() const {
    cout << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип" << endl;
    cout << string(95, '-') << endl;
}

void Service::printRow() const {
    cout << padRight(to_string(id), 5) << padRight(name, 30) << padRight(formatPrice(price), 12) << padRight(to_string(duration) + " мин", 12) << type << endl;
}

void CosmeticService::input() {
    Service::input();
    sessions = inputInt("Количество сеансов: ");
}

void CosmeticService::printRow() const {
    cout << padRight(to_string(id), 5) << padRight(name, 30) << padRight(formatPrice(price), 12) << padRight(to_string(duration) + " мин", 12) << padRight(type, 20) << sessions << endl;
}

void editService(Service& s) {
    cout << "1. ID\n2. Название\n3. Цена\n4. Длительность\n5. Тип\n6. ВСЕ\n";
    int ch = inputInt("Выбор: ");
    switch (ch) {
    case 1: s.setId(inputInt("Новый ID: ")); break;
    case 2: { string str; cout << "Новое: "; getline(cin, str); s.setName(str); break; }
    case 3: s.setPrice(inputDouble("Новая цена: ")); break;
    case 4: s.setDuration(inputInt("Новая длительность: ")); break;
    case 5: { string str; cout << "Новый тип: "; getline(cin, str); s.setType(str); break; }
    case 6: s.input(); break;
    }
}

void ServiceVector::add() { Service s; s.input(); data.push_back(s); }
void ServiceVector::show() {
    if (data.empty()) { cout << "Список пуст\n"; return; }
    data[0].printHeader(); for (const auto& s : data) s.printRow();
}
void ServiceVector::edit() {
    int id = inputInt("ID: ");
    for (auto& s : data) if (s.getId() == id) { editService(s); return; }
    cout << "Не найдено\n";
}
void ServiceVector::remove() {
    int id = inputInt("ID: ");
    auto it = remove_if(data.begin(), data.end(), [id](const Service& s) { return s.getId() == id; });
    if (it != data.end()) { data.erase(it, data.end()); cout << "Удалено\n"; }
    else cout << "Не найдено\n";
}
void ServiceVector::search() {
    string t; cout << "Тип: "; getline(cin, t); string target = toLower(t);
    bool f = false;
    for (const auto& s : data) {
        if (toLower(s.getType()) == target) { if (!f) s.printHeader(); s.printRow(); f = true; }
    }
    if (!f) cout << "Не найдено\n";
}
void ServiceVector::sortData() {
    sort(data.begin(), data.end(), [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
    cout << "Отсортировано по цене\n";
}
void ServiceVector::saveToFile() {
    if (data.empty()) { cout << "Список пуст\n"; return; }
    ofstream f("vector.txt");
    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(95, '-') << "\n";
    for (const auto& s : data) f << padRight(to_string(s.getId()), 5) << padRight(s.getName(), 30) << padRight(formatPrice(s.getPrice()), 12) << padRight(to_string(s.getDuration()) + " мин", 12) << s.getType() << "\n";
    cout << "Данные успешно сохранены в файл vector.txt\n";
}

void ServiceList::add() { CosmeticService s; s.input(); data.push_back(s); }
void ServiceList::show() {
    if (data.empty()) { cout << "Список пуст\n"; return; }
    cout << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << padRight("Тип", 20) << "Сеансы\n" << string(110, '-') << endl;
    for (const auto& s : data) s.printRow();
}
void ServiceList::edit() {
    int id = inputInt("ID: ");
    for (auto& s : data) if (s.getId() == id) { editService(s); return; }
    cout << "Не найдено\n";
}
void ServiceList::remove() {
    int id = inputInt("ID: ");
    auto it = data.begin();
    while (it != data.end()) { if (it->getId() == id) { it = data.erase(it); cout << "Удалено\n"; return; } else ++it; }
    cout << "Не найдено\n";
}
void ServiceList::search() {
    string t; cout << "Тип: "; getline(cin, t); string target = toLower(t);
    bool f = false;
    for (const auto& s : data) {
        if (toLower(s.getType()) == target) { if (!f) { cout << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << padRight("Тип", 20) << "Сеансы\n" << string(110, '-') << endl; } s.printRow(); f = true; }
    }
    if (!f) cout << "Не найдено\n";
}
void ServiceList::sortData() {
    data.sort([](const CosmeticService& a, const CosmeticService& b) { return a.getPrice() > b.getPrice(); });
    cout << "Отсортировано по цене\n";
}
void ServiceList::saveToFile() {
    if (data.empty()) { cout << "Список пуст\n"; return; }
    ofstream f("list.txt");
    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << padRight("Тип", 20) << "Сеансы\n" << string(110, '-') << "\n";
    for (const auto& s : data) f << padRight(to_string(s.getId()), 5) << padRight(s.getName(), 30) << padRight(formatPrice(s.getPrice()), 12) << padRight(to_string(s.getDuration()) + " мин", 12) << padRight(s.getType(), 20) << s.getSessions() << "\n";
    cout << "Данные успешно сохранены в файл list.txt\n";
}

void ServiceArray::add() { if (count >= 100) { cout << "Массив заполнен\n"; return; } data[count].input(); count++; }
void ServiceArray::show() {
    if (count == 0) { cout << "Список пуст\n"; return; }
    data[0].printHeader(); for (int i = 0; i < count; i++) data[i].printRow();
}
void ServiceArray::edit() {
    int id = inputInt("ID: ");
    for (int i = 0; i < count; i++) if (data[i].getId() == id) { editService(data[i]); return; }
    cout << "Не найдено\n";
}
void ServiceArray::remove() {
    int id = inputInt("ID: ");
    for (int i = 0; i < count; i++) {
        if (data[i].getId() == id) {
            for (int j = i; j < count - 1; j++) data[j] = data[j + 1];
            count--; cout << "Удалено\n"; return;
        }
    }
    cout << "Не найдено\n";
}
void ServiceArray::search() {
    string t; cout << "Тип: "; getline(cin, t); string target = toLower(t);
    bool f = false;
    for (int i = 0; i < count; i++) {
        if (toLower(data[i].getType()) == target) { if (!f) data[i].printHeader(); data[i].printRow(); f = true; }
    }
    if (!f) cout << "Не найдено\n";
}
void ServiceArray::sortData() {
    sort(data.begin(), data.begin() + count, [](const Service& a, const Service& b) { return a.getPrice() > b.getPrice(); });
    cout << "Отсортировано по цене\n";
}
void ServiceArray::saveToFile() {
    if (count == 0) { cout << "Список пуст\n"; return; }
    ofstream f("array.txt");
    f << padRight("ID", 5) << padRight("Название процедуры", 30) << padRight("Цена", 12) << padRight("Длит.", 12) << "Тип\n" << string(95, '-') << "\n";
    for (int i = 0; i < count; i++) f << padRight(to_string(data[i].getId()), 5) << padRight(data[i].getName(), 30) << padRight(formatPrice(data[i].getPrice()), 12) << padRight(to_string(data[i].getDuration()) + " мин", 12) << data[i].getType() << "\n";
    cout << "Данные успешно сохранены в файл array.txt\n";
}

void Salon::run() {
    int choice;
    do {
        cout << "\n--- ЛР3 (Вариант 13: Косметический салон) ---\n1. Вектор\n2. Список\n3. Массив\n0. Выход\n";
        choice = inputInt("Выбор: ");
        if (choice == 0) break;
        int act;
        do {
            cout << "\n1. Добавить\n2. Показать\n3. Удалить\n4. Изменить\n5. Поиск\n6. Сортировать\n7. В файл\n0. Назад\n";
            act = inputInt("Выбор: ");
            if (act == 0) break;
            if (choice == 1) {
                if (act == 1) v.add(); else if (act == 2) v.show();
                else if (act == 3) v.remove(); else if (act == 4) v.edit();
                else if (act == 5) v.search(); else if (act == 6) v.sortData();
                else if (act == 7) v.saveToFile();
            }
            else if (choice == 2) {
                if (act == 1) l.add(); else if (act == 2) l.show();
                else if (act == 3) l.remove(); else if (act == 4) l.edit();
                else if (act == 5) l.search(); else if (act == 6) l.sortData();
                else if (act == 7) l.saveToFile();
            }
            else if (choice == 3) {
                if (act == 1) a.add(); else if (act == 2) a.show();
                else if (act == 3) a.remove(); else if (act == 4) a.edit();
                else if (act == 5) a.search(); else if (act == 6) a.sortData();
                else if (act == 7) a.saveToFile();
            }
        } while (act != 0);
    } while (choice != 0);
}
void runLab3() { Salon s; s.run(); }