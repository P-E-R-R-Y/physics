#include "Physics.hpp"

#include <cmath>

namespace physics {

    void step(Body &body, double dt) {
        if (body.inverseMass <= 0.0)
            return;
        body.position += body.velocity * dt;
        body.velocity += body.acceleration * dt;
        body.velocity *= std::pow(body.damping, dt);
    }
}
