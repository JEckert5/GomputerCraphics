#pragma once

#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>

#include <glm/glm.hpp>
#include <iostream>
#include <memory>

using vec3 = glm::vec3;

// Base Class for rendered objects in the world
struct Object {
    Object() = default;

    void draw();
    virtual void render();

    void addChild(std::shared_ptr<Object> c);
    void addChildren(std::vector<std::shared_ptr<Object>> c);

    // Propagates down child tree
    void setColor(glm::vec3 c);
    void setWireframe(bool w);
    void toggleWireframe();

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale = {1, 1, 1};
    glm::vec3 color;

    bool wireframe = false;

    std::shared_ptr<Object> parent;
    std::vector<std::shared_ptr<Object>> children;
};

enum Objects {
    ROBOT,
    DOG
};

/////////////////////////////////////////////////////////
//
/// Rendered Box
//
/////////////////////////////////////////////////////////
struct Box : public Object {
    Box(double scalar = 1) : size(scalar) {
    }

    void render() override {
        wireframe ? glutWireCube(size) : glutSolidCube(size);
    }

    double size;
};

/////////////////////////////////////////////////////////
//
/// Rendered Sphere
//
/////////////////////////////////////////////////////////
struct Sphere : public Object {
    Sphere(double radius = 0.5, int slices = 10, int stacks = 20) : radius(radius), slices(slices), stacks(stacks) {
    }

    void render() override {
        wireframe ? glutWireSphere(radius, slices, stacks) : glutSolidSphere(radius, slices, stacks);
    }

    double radius;
    int slices, stacks;
};
