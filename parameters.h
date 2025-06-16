#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>

// Класс демонстрирует инкапсуляцию данных
class Parameters {
public:
    double T;  // Инкапсуляция температуры
    double Cp; // Инкапсуляция теплоемкости
    double H;  // Инкапсуляция энтальпии
    double k;  // Инкапсуляция показателя адиабаты

    // Конструктор
    Parameters(double T, double Cp, double H, double k) 
        : T(T), Cp(Cp), H(H), k(k) {} // Список инициализации
};
#endif