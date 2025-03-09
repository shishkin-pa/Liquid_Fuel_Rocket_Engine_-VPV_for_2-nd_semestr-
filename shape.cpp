#include "shape.h"
#include <cmath>
#include <vector>

// Конструктор
LavalNozzle::LavalNozzle(double gamma, double R, double P0, double T0, double Pe, double m_dot)
    : gamma(gamma), R(R), P0(P0), T0(T0), Pe(Pe), m_dot(m_dot) {}

// Расчет критического давления
double LavalNozzle::calculateCriticalPressure() const {
    return P0 * pow(2.0 / (gamma + 1.0), gamma / (gamma - 1.0));
}

// Расчет критической температуры
double LavalNozzle::calculateCriticalTemperature() const {
    return T0 * (2.0 / (gamma + 1.0));
}

// Расчет площади критического сечения
double LavalNozzle::calculateCriticalArea() const {
    double term = pow((gamma + 1.0) / 2.0, (gamma + 1.0) / (2.0 * (gamma - 1.0)));
    return (m_dot * sqrt(T0)) / (P0 * sqrt(gamma / R) * term);
}

// Расчет площади выходного сечения
double LavalNozzle::calculateExitArea(double A_star) const {
    double Me = sqrt((2.0 / (gamma - 1.0)) * (pow(P0 / Pe, (gamma - 1.0) / gamma) - 1.0));
    double term = pow((1.0 + (gamma - 1.0) / 2.0 * Me * Me), (gamma + 1.0) / (2.0 * (gamma - 1.0)));
    return A_star / Me * term;
}

// Построение профиля сопла
std::vector<double> LavalNozzle::generateNozzleProfile(double A_star, double A_e, double L, int num_points) const {
    std::vector<double> profile(num_points);
    double D_star = sqrt(4.0 * A_star / M_PI);
    double D_e = sqrt(4.0 * A_e / M_PI);
    double slope = (D_e - D_star) / (2.0 * L);

    for (int i = 0; i < num_points; ++i) {
        double x = static_cast<double>(i) / (num_points - 1) * L;
        profile[i] = D_star + 2.0 * slope * x;
    }

    return profile;
}