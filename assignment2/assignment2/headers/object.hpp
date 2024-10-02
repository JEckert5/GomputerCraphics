#pragma once

#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>

#include <glm/glm.hpp>
#include <iostream>
#include <memory>

// Base Class for rendered objects in the world
class Object {
public:
    Object() = default;

    void superDraw();
    virtual void childDraw() = 0;

    void parentTo(std::shared_ptr<Object> p);

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale = {1, 1, 1};
    glm::vec3 color;

    bool wireframe = false;

    std::shared_ptr<Object> parent;
};

/////////////////////////////////////////////////////////
//
/// Rendered box
//
/////////////////////////////////////////////////////////
class Box : public Object {
public:
    Box(double scalar) : s(scalar) {
    }

    void childDraw() override {
        wireframe ? glutWireCube(s) : glutSolidCube(s);
    }

private:
    double s;
};

/////////////////////////////////////////////////////////
//
/// Rendered Sphere
//
/////////////////////////////////////////////////////////
class Sphere : public Object {
public:
    Sphere(double radius, int slices, int stacks) : m_radius(radius), m_slices(slices), m_stacks(stacks) {
    }

    void childDraw() override {
        wireframe ? glutWireSphere(m_radius, m_slices, m_stacks) : glutSolidSphere(m_radius, m_slices, m_stacks);
    }

private:
    double m_radius;
    int m_slices, m_stacks;
};
