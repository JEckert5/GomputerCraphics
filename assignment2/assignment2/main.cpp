#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>

#include <iostream>
#include <random>

#include "headers/robot.hpp"
#include "headers/time.hpp"

static float WIN_X = 700;
static float WIN_Y = 100;
static float WIN_H = 480.0;
static float WIN_W = 640.0;

#define PI 3.141592

std::mt19937_64 rng;

double AxisLength = 1.0;

double cameraTheta = 0.001, cameraPhi = PI / 2, cameraRadius = 5;
double cameraX, cameraY, cameraZ;
double mouseX = 0, mouseY = 0;

GLint lmb, rmb;

Robot robot;

void Axes() {
    glPushMatrix();

    glTranslatef(0, 0, 0);

    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(AxisLength, 0, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, AxisLength, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, AxisLength);
    glEnd();

    glPopMatrix();
}

void Draw() {
    double tmp = Time::elapsedTime;
    Time::elapsedTime = static_cast<double>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;  // GLUT_ELAPSED_TIME is in seconds
    Time::deltaTime = Time::elapsedTime - tmp;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0.0f, 1.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    robot.superDraw();

    Axes();

flush:
    glFlush();
    glutSwapBuffers();
}

void recomputeCamera() {
    cameraX = cameraRadius * sinf(cameraTheta) * sinf(cameraPhi);
    cameraZ = cameraRadius * cosf(cameraTheta) * sinf(cameraPhi);
    cameraY = cameraRadius * cosf(cameraPhi);
}

void mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        lmb = state;
    }

    if (button == GLUT_RIGHT_BUTTON)
        rmb = state;

    mouseX = x;
    mouseY = y;
}

void mouseMotion(int x, int y) {
    if (lmb == GLUT_DOWN) {
        cameraTheta += (mouseX - x) * 0.005;
        cameraPhi += (mouseY - y) * 0.005;

        // std::cout << cameraTheta << ", " << cameraPhi << ", " << cameraRadius << std::endl;

        if (cameraPhi <= 0)
            cameraPhi = 0 + 0.001;
        if (cameraPhi >= PI)
            cameraPhi = PI - 0.001;

        recomputeCamera();
    } else if (rmb == GLUT_DOWN) {
        double change = (x - mouseX) + (y - mouseY);

        cameraRadius += change * 0.01;

        if (cameraRadius <= 2.0)
            cameraRadius = 2.0;
        if (cameraRadius >= 10.0)
            cameraRadius = 10.0;

        recomputeCamera();
    }

    mouseX = x;
    mouseY = y;
}

void resizeWindow(int w, int h) {
    auto asp = w / static_cast<float>(h);
    WIN_W = w;
    WIN_H = h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, asp, 0.1, 1000);
}

void keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            robot.toggleWireframe();
            break;
        case 27:
            exit(0);
            break;
        default:
            std::cout << "How did we get here";
            break;
    }
}

void init(int a, char** v) {
    glutInit(&a, v);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  // RGBA Double Buffer Depth mode
    glutInitWindowSize(WIN_W, WIN_H);                           // window size
    glutInitWindowPosition(WIN_X, WIN_Y);

    glutCreateWindow("Jens Eckert jeckert5");

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0, 0.0, 0.0, 1.0);  // clear the window screen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, WIN_W / WIN_H, 0.1, 1000);

    glutDisplayFunc(Draw);
    glutIdleFunc(Draw);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resizeWindow);
    glutKeyboardFunc(keyboardCallback);

    recomputeCamera();

    robot.toggleWireframe();
}

int main(int argc, char** argv) {
    std::random_device rd;
    rng.seed(rd());

    init(argc, argv);

    glutMainLoop();

    return 0;
}
