#include "substance.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>

// Конструктор демонстрирует работу с ресурсами (файлами)
Substance::Substance(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл " + filename);
    }

    // Чтение основных параметров (первая строка)
    in >> mu >> T_boil >> L_boil;
    L_boil *= 1000 * mu; // Переводим кДж/моль -> Дж/моль

    // Чтение параметров по температурам (остальные строки)
    double T, Cp, H, k;
    while (in >> T >> Cp >> H >> k) {
        parameters.emplace_back(T, Cp * mu * 1000, H * 1000, k);
    }
    
    // Сортировка по температуре
    std::sort(parameters.begin(), parameters.end(), 
        [](const Parameters& a, const Parameters& b) { return a.T < b.T; });
    if (parameters.empty()) {
        throw std::runtime_error("Файл " + filename + " не содержит данных о параметрах");
    }
}

// Приватный метод демонстрирует:
// 1. Инкапсуляцию сложной логики
// 2. Использование алгоритмов STL
const Parameters& Substance::findParameters(double T) const {
    if (parameters.empty()) {
        throw std::runtime_error("Таблица параметров вещества пуста");
    }

    // Поиск ближайшей температуры
    auto it = std::lower_bound(parameters.begin(), parameters.end(), T,
        [](const Parameters& p, double temp) { return p.T < temp; });

    // Если температура меньше минимальной в таблице
    if (it == parameters.begin() && T < it->T) {
        if (it->T - T > MAX_TEMP_DIFF) {
            throw std::out_of_range("Температура " + std::to_string(T) + 
                                  "K ниже минимальной в таблице (" + 
                                  std::to_string(parameters.front().T) + "K)");
        }
        return *it; // Используем минимальную доступную температуру
    }

    // Если температура больше максимальной в таблице
    if (it == parameters.end()) {
        if (T - parameters.back().T > MAX_TEMP_DIFF) {
            throw std::out_of_range("Температура " + std::to_string(T) + 
                                  "K превышает максимальную в таблице (" + 
                                  std::to_string(parameters.back().T) + "K)");
        }
        return parameters.back(); // Используем максимальную доступную температуру
    }

    // Если точное значение не найдено, выбираем ближайшее
    if (it->T != T) {
        // Проверяем предыдущий элемент (если существует)
        if (it != parameters.begin()) {
            auto prev_it = std::prev(it);
            if (std::abs(prev_it->T - T) < std::abs(it->T - T)) {
                it = prev_it;
            }
        }

        // Проверяем допустимость отклонения
        if (std::abs(it->T - T) > MAX_TEMP_DIFF) {
            throw std::out_of_range("Ближайшая доступная температура " + 
                                  std::to_string(it->T) + 
                                  "K слишком далеко от запрошенной " + 
                                  std::to_string(T) + "K");
        }
    }

    return *it;
}

// Методы доступа демонстрируют единый интерфейс
double Substance::getCp(double T) const { return findParameters(T).Cp; }
double Substance::getH(double T) const { return findParameters(T).H; }
double Substance::getK(double T) const { return findParameters(T).k; }