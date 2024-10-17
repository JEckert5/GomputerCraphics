#include "headers/dog.hpp"

Dog::Dog(vec3 h,
         vec3 b,
         vec3 n,
         vec3 e,
         vec3 t,
         vec3 l) {
    // Good god this is horrific
    children.reserve(1);

    body = std::make_shared<Box>();
    body->scale = b;

    addChild(body);

    headJoint = std::make_shared<Object>();
    rfJoint = std::make_shared<Object>();
    lfJoint = std::make_shared<Object>();
    rbJoint = std::make_shared<Object>();
    lbJoint = std::make_shared<Object>();
    tailJoint = std::make_shared<Object>();

    headJoint->position = {0, b.y / 3, b.z / 1.85};

    rfJoint->position = {-b.x / 2.5, -b.y / 2, b.z / 2.5};
    rbJoint->position = {-b.x / 2.5, -b.y / 2, -b.z / 2.5};

    lfJoint->position = {b.x / 2.5, -b.y / 2, b.z / 2.5};
    lbJoint->position = {b.x / 2.5, -b.y / 2, -b.z / 2.5};

    tailJoint->position = {0, 0, -b.z / 2};

    body->addChild(rfJoint);
    body->addChild(rbJoint);
    body->addChild(lfJoint);
    body->addChild(lbJoint);
    body->addChild(headJoint);
    body->addChild(tailJoint);

    head = std::make_shared<Box>();
    head->scale = h;
    head->position = {0, 0, 0};

    headJoint->addChild(head);

    nose = std::make_shared<Box>();
    nose->scale = n;
    nose->position = {0, 0, h.z / 2};

    head->addChild(nose);

    rfLeg = std::make_shared<Box>();
    rbLeg = std::make_shared<Box>();
    lfLeg = std::make_shared<Box>();
    lbLeg = std::make_shared<Box>();
    rfLeg->scale = l;
    rfLeg->position = {0, -l.y / 2, 0};
    rfJoint->addChild(rfLeg);

    rbLeg->scale = l;
    rbLeg->position = {0, -l.y / 2, 0};
    rbJoint->addChild(rbLeg);

    lfLeg->scale = l;
    lfLeg->position = {0, -l.y / 2, 0};
    lfJoint->addChild(lfLeg);

    lbLeg->scale = l;
    lbLeg->position = {0, -l.y / 2, 0};
    lbJoint->addChild(lbLeg);

    tail = std::make_shared<Box>();
    tail->scale = t;
    tail->position = {0, 0, -t.z / 2};

    tailJoint->addChild(tail);

    setColor(color);
}

void Dog::render() {
}