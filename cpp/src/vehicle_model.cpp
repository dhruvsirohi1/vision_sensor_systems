//
// Created by Dhruv Sirohi on 12/30/25.
//

#include "vehicle_model.hpp"

#include <algorithm>

#include "cmath"

State VehicleModel::step(const State &x,
                        const Control &u,
                        double dt) const {

    // Next State

    State next_x;

    next_x.x = x.x + x.v * x.v * std::cos(x.theta) * dt;
    next_x.y = x.y + x.v * x.v * std::sin(x.theta) * dt;
    next_x.theta = x.theta + u.omega * dt;

    next_x.v = x.v + u.a * dt;

    // Non-negative speed
    next_x.v = std::max(0.0, next_x.v);

    return next_x;
}
