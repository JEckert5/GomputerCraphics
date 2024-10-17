#include "headers/robot.hpp"

#include <OpenGL/OpenGL.h>

Robot::Robot(float h,
             vec3 t,
             vec3 uas,
             vec3 las,
             vec3 uls,
             vec3 lls) : headSize(h),
                         torsoSize(t),
                         upperArmSize(uas),
                         lowerArmSize(las),
                         upperLegSize(uls),
                         lowerLegSize(lls) {
    children.reserve(1);

    head = std::make_shared<Sphere>(headSize, 20, 10);
    head->position.y = 1.2;
    head->color = color;

    torso = std::make_shared<Box>(1);
    torso->children.reserve(4);
    torso->scale = torsoSize;
    // torso->rotation = {0, 0, 45};
    torso->addChild(head);

    addChild(torso);

    // Create and position upper rotate points.
    upperRightArmRotatePoint = std::make_shared<Object>();
    upperRightArmRotatePoint->position = {-torsoSize.x / 2, torsoSize.y / 3, 0};

    upperLeftArmRotatePoint = std::make_shared<Object>();
    upperLeftArmRotatePoint->position = {torsoSize.x / 2, torsoSize.y / 3, 0};

    upperRightLegRotatePoint = std::make_shared<Object>();
    upperRightLegRotatePoint->position = {-torsoSize.x / 4, -torsoSize.y / 2, 0};

    upperLeftLegRotatePoint = std::make_shared<Object>();
    upperLeftLegRotatePoint->position = {torsoSize.x / 4, -torsoSize.y / 2, 0};

    torso->addChild(upperRightArmRotatePoint);
    torso->addChild(upperLeftArmRotatePoint);
    torso->addChild(upperRightLegRotatePoint);
    torso->addChild(upperLeftLegRotatePoint);

    ///////////// Right Arm
    upperRightArm = std::make_shared<Box>(1);
    upperRightArm->position = {-upperArmSize.x / 2, 0, 0};
    upperRightArm->scale = upperArmSize;

    upperRightArmRotatePoint->addChild(upperRightArm);

    lowerRightArmRotatePoint = std::make_shared<Object>();
    lowerRightArmRotatePoint->position = {-upperArmSize.x / 2, 0, 0};

    upperRightArm->addChild(lowerRightArmRotatePoint);

    lowerRightArm = std::make_shared<Box>(1);
    lowerRightArm->scale = lowerArmSize;
    lowerRightArm->position = {-lowerArmSize.x / 2, 0, 0};

    lowerRightArmRotatePoint->addChild(lowerRightArm);

    ////////////// Left Arm

    upperLeftArm = std::make_shared<Box>(1);
    upperLeftArm->position = {upperArmSize.x / 2, 0, 0};
    upperLeftArm->scale = upperArmSize;

    upperLeftArmRotatePoint->addChild(upperLeftArm);

    lowerLeftArmRotatePoint = std::make_shared<Object>();
    lowerLeftArmRotatePoint->position = {upperArmSize.x / 2, 0, 0};

    upperLeftArm->addChild(lowerLeftArmRotatePoint);

    lowerLeftArm = std::make_shared<Box>(1);
    lowerLeftArm->position = {lowerArmSize.x / 2, 0, 0};
    lowerLeftArm->scale = lowerArmSize;

    lowerLeftArmRotatePoint->addChild(lowerLeftArm);

    ////////////// Right Leg

    upperRightLeg = std::make_shared<Box>(1);
    upperRightLeg->position = {0, -upperLegSize.y / 2, 0};
    upperRightLeg->scale = upperLegSize;

    upperRightLegRotatePoint->addChild(upperRightLeg);

    lowerRightLegRotatePoint = std::make_shared<Object>();
    lowerRightLegRotatePoint->position = {0, -upperLegSize.y / 2, 0};

    upperRightLeg->addChild(lowerRightLegRotatePoint);

    lowerRightLeg = std::make_shared<Box>(1);
    lowerRightLeg->scale = lowerLegSize;
    lowerRightLeg->position = {0, -lowerLegSize.y / 2, 0};

    lowerRightLegRotatePoint->addChild(lowerRightLeg);

    ////////////// Left Leg

    upperLeftLeg = std::make_shared<Box>(1);
    upperLeftLeg->position = {0, -upperLegSize.y / 2, 0};
    upperLeftLeg->scale = upperLegSize;

    upperLeftLegRotatePoint->addChild(upperLeftLeg);

    lowerLeftLegRotatePoint = std::make_shared<Object>();
    lowerLeftLegRotatePoint->position = {0, -upperLegSize.y / 2, 0};

    upperLeftLeg->addChild(lowerLeftLegRotatePoint);

    lowerLeftLeg = std::make_shared<Box>(1);
    lowerLeftLeg->scale = lowerLegSize;
    lowerLeftLeg->position = {0, -lowerLegSize.y / 2, 0};

    lowerLeftLegRotatePoint->addChild(lowerLeftLeg);

    setColor(color);
}

void Robot::render() {
}