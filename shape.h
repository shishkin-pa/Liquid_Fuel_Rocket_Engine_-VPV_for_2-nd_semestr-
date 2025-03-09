#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

class LavalNozzle {
private:
    double gamma;       // Показатель адиабаты для продуктов сгорания
    double R;           // Удельная газовая постоянная для продуктов сгорания (Дж/(кг·К))
    double P0;          // Давление в камере сгорания (Па)
    double T0;          // Температура в камере сгорания (К)
    double Pe;          // Атмосферное давление (Па)
    double m_dot;       // Массовый расход (кг/с)
    double Q;           // Теплота сгорания топлива (Дж/кг)
    double OF_ratio;    // Соотношение окислителя к топливу (O/F)
    double M;           // Молярная масса продуктов сгорания (кг/моль)

public:
    // Конструктор
    LavalNozzle(double gamma, double R, double P0, double Pe, double m_dot, double Q, double OF_ratio, double M);

    // Расчет критического давления
    double calculateCriticalPressure() const;

    // Расчет критической температуры
    double calculateCriticalTemperature() const;

    // Расчет площади критического сечения
    double calculateCriticalArea() const;

    // Расчет площади выходного сечения
    double calculateExitArea(double A_star) const;

    // Построение профиля сопла
    std::vector<double> generateNozzleProfile(double A_star, double A_e, double L, int num_points) const;

private:
    // Расчет температуры в камере сгорания
    double calculateCombustionTemperature() const;
};

#endif // SHAPE_H
