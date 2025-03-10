#include "shape.h"
#include <cmath>
#include <vector>

// Конструктор
LavalNozzle::LavalNozzle(double gamma, double R, double P0, double T0, double Pe, double m_dot, double mu, double k, double D_throat, double k_parabola, double L)
    : gamma(gamma), R(R), P0(P0), T0(T0), Pe(Pe), m_dot(m_dot), mu(mu), k(k), D_throat(D_throat), k_parabola(k_parabola), L(L) {}

// Функция для задания параболического профиля сопла
double LavalNozzle::nozzleProfile(double x) {
    return D_throat + k_parabola * x * x; // Параболический профиль
}

// Функция для расчёта площади сечения
double LavalNozzle::calculateArea(double D) {
    return M_PI * pow(D / 2, 2);
}

// Численное решение уравнений
void LavalNozzle::solveNavierStokes(std::vector<double>& pressure, std::vector<double>& temperature, std::vector<double>& velocity) {
    int num_points = pressure.size();
    double dx = L / (num_points - 1);
    double A_throat = calculateArea(D_throat);

    // Начальные условия
    pressure[0] = P0;
    temperature[0] = T0;
    velocity[0] = (m_dot * R * T0) / (P0 * calculateArea(nozzleProfile(0)));

    for (int i = 1; i < num_points; ++i) {
        double x = i * dx;
        double D = nozzleProfile(x);
        double A = calculateArea(D);

        // Изоэнтропические соотношения (исправлено для температуры)
        pressure[i] = P0 * pow(A_throat / A, (2 * gamma) / (gamma - 1));
        temperature[i] = T0 * pow(A_throat / A, (gamma - 1) / gamma); // Исправлено
        velocity[i] = (m_dot * R * temperature[i]) / (pressure[i] * A);

        // Поправка на вязкость и теплопроводность
        double dp_dx = -mu * velocity[i-1] / (D * D);
        pressure[i] += dp_dx * dx;

        double dT_dx = -k * (temperature[i-1] - T0) / (D * D);
        temperature[i] += dT_dx * dx;
    }

    // Корректировка выходного давления
    pressure[num_points - 1] = Pe;
}
// Расчет профиля сопла
std::vector<double> LavalNozzle::calculateNozzleProfile(int num_points) {
    std::vector<double> profile(num_points);
    double dx = L / (num_points - 1);

    for (int i = 0; i < num_points; ++i) {
        double x = i * dx;
        profile[i] = nozzleProfile(x);
    }

    return profile;
}
