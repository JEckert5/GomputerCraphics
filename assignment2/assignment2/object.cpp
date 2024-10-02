#include "headers/object.hpp"

#include "object.hpp"

void Object::superDraw() {
    glPushMatrix();

    glTranslated(position.x, position.y, position.z);
    glRotated(rotation.x, 1, 0, 0);
    glRotated(rotation.y, 0, 1, 0);
    glRotated(rotation.z, 0, 0, 1);
    glScaled(scale.x, scale.y, scale.z);

    glColor3f(color.r, color.g, color.b);

    this->childDraw();

    glPopMatrix();
}

void Object::parentTo(std::shared_ptr<Object> p) {
    parent = p;
}