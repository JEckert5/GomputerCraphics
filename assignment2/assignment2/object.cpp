#include "headers/object.hpp"

void Object::addChild(std::shared_ptr<Object> c) {
    children.emplace_back(c);
}

void Object::setColor(glm::vec3 c) {
    color = c;

    for (auto& ch : children) {
        ch->setColor(color);
    }
}

void Object::setWireframe(bool w) {
    wireframe = w;

    for (auto& c : children) {
        c->setWireframe(wireframe);
    }
}

void Object::toggleWireframe() {
    setWireframe(!wireframe);
}

void Object::render() {
    // glutWireSphere(0.1, 10, 5);
}

void Object::draw() {
    glPushMatrix();

    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glRotatef(rotation.z, 0, 0, 1);

    for (auto& c : children) {
        c->draw();
    }

    glScaled(scale.x, scale.y, scale.z);

    glColor3f(color.r, color.g, color.b);

    // Actual render call
    render();

    glPopMatrix();
}