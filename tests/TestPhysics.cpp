#include "Physics.hpp"

#include <gtest/gtest.h>

TEST(Physics, FallsUnderGravity) {
    physics::Body body{.acceleration = {0, -9.81, 0}};

    physics::step(body, 1.0);
    EXPECT_NEAR(body.velocity.y, -9.81, 0.01);
}

TEST(Physics, InfiniteMassStaysPut) {
    physics::Body body{.velocity = {1, 0, 0}, .inverseMass = 0.0};

    physics::step(body, 1.0);
    EXPECT_DOUBLE_EQ(body.position.x, 0.0);
}
