#include "./headers/robot.hpp"

#include <OpenGL/OpenGL.h>

Robot::Robot() {
    objects.reserve(10);
    auto itr = objects.begin();

    head = std::make_shared<Sphere>(0.5, 20, 10);
    head->position.y = 1.2;

    objects.emplace(itr++, head);

    torso = std::make_shared<Box>(1);
    torso->scale = glm::vec3(1, 1.5, 0.8);

    objects.emplace(itr++, torso);

    // He is facing positive Z because I said so.
    upperRightArm = std::make_shared<Box>(1);
    upperRightArm->position = {-1, 0.3, 0};
    upperRightArm->scale = {0.6, 0.3, 0.3};
    upperRightArm->rotation = {0, 0, 0};

    objects.emplace(itr++, upperRightArm);

    lowerRightArm = std::make_shared<Box>(1);
    lowerRightArm->position = {-1.3, -0.3, 0};
    lowerRightArm->scale = {};

    upperLeftArm = std::make_shared<Box>(1);
    upperLeftArm->position = {-1.3, -0.3, 0};
    upperLeftArm->scale = {};
    lowerLeftArm = std::make_shared<Box>(1);

    upperRightLeg = std::make_shared<Box>(1);
    lowerRightLeg = std::make_shared<Box>(1);

    upperLeftLeg = std::make_shared<Box>(1);
    lowerLeftLeg = std::make_shared<Box>(1);

    for (auto& obj : objects) {
        obj->color = color;
    }
}

void Robot::childDraw() {
    for (auto& obj : objects) {
        obj->superDraw();
    }
}

void Robot::toggleWireframe() {
    wireframe = !wireframe;

    for (auto& obj : objects) {
        obj->wireframe = wireframe;
    }
}