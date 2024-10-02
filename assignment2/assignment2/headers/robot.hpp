#pragma once

#ifndef ROBOT_H
#define ROBOT_H

#include <GL/freeglut.h>

#include <memory>
#include <vector>

#include "../include/glm/glm.hpp"
#include "object.hpp"

class Robot : public Object {
public:
    Robot();

    std::vector<std::shared_ptr<Object>> objects;
    std::shared_ptr<Sphere> head;

    std::shared_ptr<Box> torso,
        upperRightArm, lowerRightArm,
        upperLeftArm, lowerLeftArm,
        upperLeftLeg, lowerLeftLeg,
        upperRightLeg, lowerRightLeg;

    void childDraw() override;
    void toggleWireframe();

    glm::vec3 color = {0.2, 0.4, 0.78};
};

#endif
