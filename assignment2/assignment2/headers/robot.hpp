#pragma once

#ifndef ROBOT_H
#define ROBOT_H

#include <GL/freeglut.h>

#include <memory>
#include <vector>

#include "../include/glm/glm.hpp"
#include "object.hpp"

using vec3 = glm::vec3;

struct Robot : public Object {
    Robot(float h = 0.5f,
          vec3 t = {1, 1.5, 0.8},
          vec3 uas = {0.6, 0.3, 0.3},
          vec3 las = {0.6, 0.3, 0.3},
          vec3 uls = {0.3, 0.6, 0.3},
          vec3 lls = {0.3, 0.6, 0.3});

    std::shared_ptr<Sphere> head;

    std::shared_ptr<Box> torso,
        upperRightArm, lowerRightArm,
        upperLeftArm, lowerLeftArm,
        upperLeftLeg, lowerLeftLeg,
        upperRightLeg, lowerRightLeg;

    std::shared_ptr<Object> upperRightArmRotatePoint, lowerRightArmRotatePoint;
    std::shared_ptr<Object> upperLeftArmRotatePoint, lowerLeftArmRotatePoint;
    std::shared_ptr<Object> upperRightLegRotatePoint, lowerRightLegRotatePoint;
    std::shared_ptr<Object> upperLeftLegRotatePoint, lowerLeftLegRotatePoint;

    void render() override;

    vec3 color = {0.2, 0.4, 0.78};

    float headSize;
    vec3 torsoSize, upperArmSize, lowerArmSize, upperLegSize, lowerLegSize;
};

#endif
