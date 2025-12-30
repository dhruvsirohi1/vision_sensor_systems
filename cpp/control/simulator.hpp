//
// Created by Dhruv Sirohi on 12/30/25.
//

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "vehicle_model.hpp"
#include <vector>

class Simulator {
public:
    Simulator(double dt, VehicleModel model);

    void reset(const State& initial_state);

    void step(const Control& u);

    const State& currentState() const;
    const std::vector<State>& trajectory() const;

private:
    double dt_;
    VehicleModel model_;

    State state_;
    std::vector<State> trajectory_;
};

#endif //SIMULATOR_HPP
