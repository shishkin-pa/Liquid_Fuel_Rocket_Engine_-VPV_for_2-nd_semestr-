#ifndef ENGINE_CALCULATIONS_H
#define ENGINE_CALCULATIONS_H

#include "substance.h"

// Класс демонстрирует:
// 1. Статический полиморфизм (все методы статические)
// 2. Инкапсуляцию расчетных алгоритмов
class EngineCalculations {
public:
    // Статические методы - могут вызываться без создания экземпляра класса
    static double calculateNuConcentration(double concentration, 
                                         const Substance& H2O2, 
                                         const Substance& H2O_liquid);
    
    static double calculateMaxTemperature(double nu_concentration, 
                                        const Substance& H2O2,
                                        const Substance& H2O_liquid,
                                        const Substance& H2O_gas,
                                        const Substance& O2);
    
    static double calculateFinalTemperature(double final_P, 
                                         double begin_P,
                                         const Substance& H2O_gas,
                                         const Substance& O2,
                                         double begin_T,
                                         double nu_conc);
    
    // Структура для группировки результатов - инкапсуляция связанных данных
    struct NozzleResults {
        double exitVelocity;
        double inletVelocity;
        double exitRadius;
        double criticalRadius;
        double massFlowRate;
    };
    
    static NozzleResults calculateNozzleParameters(const Substance& H2O_gas,
                                                  const Substance& O2,
                                                  double nu_conc,
                                                  double max_T,
                                                  double end_T,
                                                  double r_chamber,
                                                  double P_chamber,
                                                  double P_atm,
                                                  double F);
};
#endif