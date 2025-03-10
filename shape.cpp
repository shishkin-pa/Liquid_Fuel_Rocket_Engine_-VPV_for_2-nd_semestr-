#include "shape.h"
#include <cmath>
#include <vector>

// Конструктор: инициализирует все параметры
LavalNozzle::LavalNozzle(double gamma, double R, double P0, double T0, double Pe, double m_dot, double mu, double k, double D_throat, double k_parabola, double L)
    : gamma(gamma), R(R), P0(P0), T0(T0), Pe(Pe), m_dot(m_dot), mu(mu), k(k), D_throat(D_throat), k_parabola(k_parabola), L(L) {}

// Функция для задания параболического профиля сопла
double LavalNozzle::nozzleProfile(double x) {
    // Параболический профиль: D(x) = D_throat + k_parabola * x^2
    return D_throat + k_parabola * x * x;
}

// Функция для расчёта площади сечения сопла
double LavalNozzle::calculateArea(double D) {
    // Площадь круга: A = π * (D/2)^2
    return M_PI * pow(D / 2, 2);
}

// Численное решение уравнений Навье-Стокса
void LavalNozzle::solveNavierStokes(std::vector<double>& pressure, std::vector<double>& temperature, std::vector<double>& velocity) {
    int num_points = pressure.size(); // Количество точек вдоль сопла
    double dx = L / (num_points - 1); // Шаг по длине сопла
    double A_throat = calculateArea(D_throat); // Площадь горловины сопла
    double A_max = 10 * A_throat; // Максимальная площадь сечения (ограничение)
    double P_min = 0.1 * Pe; // Минимальное допустимое давление (ограничение)

    // Начальные условия на входе сопла (x = 0)
    pressure[0] = P0; // Давление на входе
    temperature[0] = T0; // Температура на входе
    velocity[0] = (m_dot * R * T0) / (P0 * calculateArea(nozzleProfile(0))); // Скорость на входе

    // Расчёт параметров газа в каждой точке сопла
    for (int i = 1; i < num_points; ++i) {
        double x = i * dx; // Текущая координата вдоль сопла
        double D = nozzleProfile(x); // Диаметр сопла в точке x
        double A = calculateArea(D); // Площадь сечения в точке x

        // Ограничение площади сечения, чтобы избежать слишком больших значений
        if (A > A_max) A = A_max;

        // Изоэнтропические соотношения для давления и температуры
        pressure[i] = P0 * pow(A_throat / A, (2 * gamma) / (gamma - 1)); // Давление
        temperature[i] = T0 * pow(A_throat / A, (gamma - 1) / gamma); // Температура

        // Скорость газа через уравнение состояния идеального газа
        velocity[i] = (m_dot * R * temperature[i]) / (pressure[i] * A);

        // Поправка на вязкость: градиент давления
        double dp_dx = -mu * velocity[i-1] / (D * D); // Градиент давления
        pressure[i] += dp_dx * dx; // Обновление давления
        if (pressure[i] < P_min) pressure[i] = P_min; // Ограничение давления

        // Поправка на теплопроводность: градиент температуры
        double dT_dx = -k * (temperature[i-1] - T0) / (D * D); // Градиент температуры
        temperature[i] += dT_dx * dx; // Обновление температуры
    }

    // Корректировка давления на выходе сопла
    pressure[num_points - 1] = Pe; // Давление на выходе
    if (pressure[num_points - 1] < P_min) pressure[num_points - 1] = P_min; // Ограничение давления
}

// Расчёт профиля сопла: возвращает диаметр сопла в каждой точке
std::vector<double> LavalNozzle::calculateNozzleProfile(int num_points) {
    std::vector<double> profile(num_points); // Вектор для хранения диаметров
    double dx = L / (num_points - 1); // Шаг по длине сопла

    // Расчёт диаметра сопла в каждой точке
    for (int i = 0; i < num_points; ++i) {
        double x = i * dx; // Текущая координата вдоль сопла
        profile[i] = nozzleProfile(x); // Диаметр сопла в точке x
    }

    return profile; // Возвращаем профиль сопла
}
