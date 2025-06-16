#ifndef SUBSTANCE_H
#define SUBSTANCE_H

#include <vector>
#include <string>
#include "parameters.h"

// Класс демонстрирует несколько принципов ООП:
// 1. Инкапсуляция - скрытие внутренней реализации
// 2. Абстракция - предоставление простого интерфейса
class Substance {
private:
    // Приватные поля - инкапсуляция данных
    double mu;       // Молярная масса
    double T_boil;   // Температура кипения
    double L_boil;   // Теплота парообразования
    std::vector<Parameters> parameters; // Композиция - класс содержит объекты другого класса
    static constexpr double MAX_TEMP_DIFF = 5.0; // Максимально допустимая разница температур [K]

    // Приватный метод - инкапсуляция реализации
    const Parameters& findParameters(double T) const;

public:
    // Конструктор - инициализация объекта
    Substance(const std::string& filename);
    
    // Геттеры - методы доступа к приватным полям (инкапсуляция)
    double getMu() const { return mu; } // const-метод не изменяет состояние объекта
    double getTBoil() const { return T_boil; }
    double getLBoil() const { return L_boil; }
    
    // Публичные методы - интерфейс класса
    double getCp(double T) const;
    double getH(double T) const;
    double getK(double T) const;
};
#endif