#pragma once

#ifndef CAT_H
#define CAT_H

#include "object.hpp"

struct Dog : public Object {
    Dog(vec3 h = {0.45, 0.45, 0.4},
        vec3 b = {0.5, 0.5, 1},
        vec3 n = {0.2, 0.2, 0.3},
        vec3 e = {0.15, 0.15, 0.05},
        vec3 t = {0.15, 0.15, 0.8},
        vec3 l = {0.15, 0.45, 0.15});

    std::shared_ptr<Box> head, nose, body, tail, rfLeg, lfLeg, rbLeg, lbLeg;
    std::shared_ptr<Object> tailJoint, headJoint, rfJoint, lfJoint, rbJoint, lbJoint;

    void render() override;

    vec3 color = {0.2, 0.4, 0.78};
};

#endif