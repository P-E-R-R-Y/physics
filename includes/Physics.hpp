/**
 * @file Physics.hpp
 * @brief Integration of a rigid body over one step.
 *
 * @addtogroup physics
 * @{
 */

#pragma once

#include "Vector3.hpp"

namespace physics {

    /** @brief A body reduced to what integration needs. */
    struct Body {
        Vector3<double> position;
        Vector3<double> velocity;
        Vector3<double> acceleration;
        double inverseMass = 1.0;   ///< 0 pins the body in place
        double damping = 0.999;
    };

    /** @brief Advances the body by dt seconds. */
    void step(Body &body, double dt);
}

/** @} */
