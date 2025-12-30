//
// Created by Dhruv Sirohi on 12/30/25.
//

#include "../control/simulator.hpp"

Simulator::Simulator(double dt, VehicleModel model) :
dt_(dt), model_(model) {}

void Simulator::reset(const State &initial_state) {
    state_ = initial_state;
    trajectory_.clear();
    trajectory_.push_back(state_);
}

void Simulator::step(const Control &u) {
    state_ = model_.step(state_, u, dt_);
    trajectory_.push_back(state_);
}

const State &Simulator::currentState() const {
    return state_;
}

const std::vector<State> &Simulator::trajectory() const {
    return trajectory_;
}



