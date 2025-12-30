//
// Created by Dhruv Sirohi on 12/30/25.
//

#ifndef VEHICLE_MODEL_HPP
#define VEHICLE_MODEL_HPP

struct State {
    double x;
    double y;
    double theta;
    double v;
};

struct Control {
    double a; // acceleration
    double omega; // yaw rate
};

class VehicleModel {
public:
    State step(const State& x,
               const Control& u,
               double dt) const;
};
#endif //VEHICLE_MODEL_HPP
