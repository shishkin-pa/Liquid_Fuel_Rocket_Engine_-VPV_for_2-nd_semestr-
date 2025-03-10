#include <iostream>
#include "shape.h"

int main() {
    // Входные данные
    double gamma = 1.4;         // Показатель адиабаты
    double R = 287.0;           // Удельная газовая постоянная (Дж/(кг·К))
    double P0 = Никита добавит;         // Давление в камере сгорания (Па)
    double T0 = Никита добавит;         // Температура в камере сгорания (К)
    double Pe = 1.0e5;          // Атмосферное давление (Па)
    double m_dot = Никита добавит;         // Массовый расход (кг/с)
    double mu = Никита добавит;         // Динамическая вязкость (Па·с)
    double k = Никита добавит;           // Теплопроводность (Вт/(м·К))
    double D_throat = Решим позже;      // Диаметр горловины сопла (м)
    double k_parabola = (0.5, но это пример, реально решим позже);    // Коэффициент для параболического профиля
    double L = Решим позже;             // Длина сопла (м)

    // Создание объекта сопла Лаваля
    LavalNozzle nozzle(gamma, R, P0, T0, Pe, m_dot, mu, k, D_throat, k_parabola, L);

    // Расчет профиля сопла
    int num_points = 100;
    std::vector<double> profile = nozzle.calculateNozzleProfile(num_points);

    // Численное решение уравнений
    std::vector<double> pressure(num_points);
    std::vector<double> temperature(num_points);
    std::vector<double> velocity(num_points);
    nozzle.solveNavierStokes(pressure, temperature, velocity);

    // Вывод результатов
    std::cout << "Профиль сопла (диаметры в метрах):" << std::endl;
    for (int i = 0; i < num_points; ++i) {
        std::cout << "x = " << static_cast<double>(i) / (num_points - 1) * L
                  << " м, D = " << profile[i] << " м" << std::endl;
    }

    std::cout << "\nДавление (Па):" << std::endl;
    for (int i = 0; i < num_points; ++i) {
        std::cout << "x = " << static_cast<double>(i) / (num_points - 1) * L
                  << " м, P = " << pressure[i] << " Па" << std::endl;
    }

    std::cout << "\nТемпература (К):" << std::endl;
    for (int i = 0; i < num_points; ++i) {
        std::cout << "x = " << static_cast<double>(i) / (num_points - 1) * L
                  << " м, T = " << temperature[i] << " К" << std::endl;
    }

    std::cout << "\nСкорость (м/с):" << std::endl;
    for (int i = 0; i < num_points; ++i) {
        std::cout << "x = " << static_cast<double>(i) / (num_points - 1) * L
                  << " м, V = " << velocity[i] << " м/с" << std::endl;
    }

    return 0;
}
