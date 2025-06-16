#include "engine_calculations.h"
#define M_PI
#include <cmath>

// Константы демонстрируют инкапсуляцию "волшебных чисел"
const double R = 8.31; // Универсальная газовая постоянная

// Расчет концентрации демонстрирует работу с константными ссылками
double EngineCalculations::calculateNuConcentration(double concentration, 
                                                  const Substance& H2O2,
                                                  const Substance& H2O_liquid) {
    return concentration / (concentration + (1 - concentration) * H2O2.getMu() / H2O_liquid.getMu());
}

// Расчет температуры демонстрирует итеративный алгоритм
double EngineCalculations::calculateMaxTemperature(double nu_concentration,
                                                 const Substance& H2O2,
                                                 const Substance& H2O_liquid,
                                                 const Substance& H2O_gas,
                                                 const Substance& O2) {
    const double T_initial = 298;
    double Q = (-H2O_liquid.getH(T_initial) + H2O2.getH(T_initial)) * nu_concentration;
    double Q_heat = 0;
    double T = T_initial;

    // Условные ветвления для разных агрегатных состояний
    while (Q_heat < Q) {
        if (T < H2O_liquid.getTBoil()) {
            Q_heat += H2O_liquid.getCp(T) + (0.5 * O2.getCp(T) * nu_concentration);
        }
        else if (T == H2O_liquid.getTBoil()) {
            Q_heat += H2O_liquid.getLBoil();
        }
        else {
            Q_heat += H2O_gas.getCp(T) + (0.5 * O2.getCp(T) * nu_concentration);
        }
        T++;
    }
    return T;
}

// Расчет температуры на выходе
double EngineCalculations::calculateFinalTemperature(double final_P,
                                                   double begin_P,
                                                   const Substance& H2O_gas,
                                                   const Substance& O2,
                                                   double begin_T,
                                                   double nu_conc) {
    double P = begin_P;
    double T = begin_T;

    while (P > final_P) {
        double k_mean = (H2O_gas.getCp(T) + O2.getCp(T) * 0.5 * nu_conc) / 
                       (H2O_gas.getCp(T) + O2.getCp(T) * 0.5 * nu_conc - R * (1 + 0.5 * nu_conc));
        P *= 0.99;
        T *= pow((P / begin_P), (k_mean - 1) / k_mean);
        begin_P = P;
    }
    return T;
}

// Оптимизированный расчет сопла демонстрирует:
// 1. Лямбда-функции для повторяющихся вычислений
// 2. Структурирование кода
EngineCalculations::NozzleResults EngineCalculations::calculateNozzleParameters(
    const Substance& H2O_gas, const Substance& O2,
    double nu_conc, double max_T, double end_T,
    double r_chamber, double P_chamber, 
    double P_atm, double F) 
{
    NozzleResults results;
    
    // Лямбда-функция для расчета средних параметров
    auto calculateMeanParameters = [&](double T) {
        struct {
            double meanH;
            double meanMu;
            double meanCp;
        } params;
        
        params.meanH = (H2O_gas.getH(T) + O2.getH(T) * nu_conc / 2) / (1 + nu_conc / 2);
        params.meanMu = (H2O_gas.getMu() + O2.getMu() * nu_conc / 2) / (1 + nu_conc / 2);
        params.meanCp = (H2O_gas.getCp(T) + O2.getCp(T) * nu_conc / 2) / (1 + nu_conc / 2);
        
        return params;
    };

    // Расчет параметров в камере
    auto chamberParams = calculateMeanParameters(max_T);
    auto exitParams = calculateMeanParameters(end_T);
    
    // Геометрические параметры
    double S_chamber = M_PI * pow(r_chamber, 2);
    double ro_chamber = P_chamber * chamberParams.meanMu / (R * max_T);
    
    // Итерационный расчет скоростей
    results.exitVelocity = sqrt(2 * (chamberParams.meanH - exitParams.meanH) / chamberParams.meanMu);
    results.inletVelocity = 0;
    double prev_velocity = 0;
    
    do {
        prev_velocity = results.inletVelocity;
        double q = F / results.exitVelocity;
        results.inletVelocity = q / (S_chamber * ro_chamber);
        results.exitVelocity = sqrt(2 * (chamberParams.meanH - exitParams.meanH) / chamberParams.meanMu + 
                                  pow(results.inletVelocity, 2));
    } while (fabs(prev_velocity - results.inletVelocity) > 0.01);

    // Расчет критических параметров
    double crit_T = max_T;
    double P_crit = P_chamber;
    double v_sound, v_flow;
    
    do {
        auto critParams = calculateMeanParameters(crit_T);
        double k_mean = critParams.meanCp / (critParams.meanCp - R);
        
        v_sound = sqrt(k_mean * R * crit_T / critParams.meanMu);
        v_flow = sqrt(2 * (chamberParams.meanH - critParams.meanH) / chamberParams.meanMu + 
                     pow(results.inletVelocity, 2));
        
        double prev_T = crit_T;
        crit_T--;
        P_crit *= pow(crit_T/prev_T, k_mean/(k_mean-1));
    } while (fabs(v_sound - v_flow)/v_sound > 0.005);

    // Расчет геометрии сопла
    double ro_exit = P_atm * exitParams.meanMu / (R * end_T);
    results.exitRadius = sqrt(F / (pow(results.exitVelocity, 2) * ro_exit * M_PI));
    
    double ro_crit = P_crit * exitParams.meanMu / (R * crit_T);
    results.criticalRadius = sqrt(F / (ro_crit * results.exitVelocity * v_flow * M_PI));
    
    // Массовый расход
    results.massFlowRate = F / results.exitVelocity;
    
    return results;
}