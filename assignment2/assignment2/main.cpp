#include <GL/freeglut.h>

#include <iostream>
#include <random>

#include "headers/camera.hpp"
#include "headers/dog.hpp"
#include "headers/robot.hpp"
#include "headers/time.hpp"

static float WIN_X = 500;
static float WIN_Y = 100;
static float WIN_H = 480.0;
static float WIN_W = 640.0;

// I paid for the whole double I'm gonna use the whole double.
#define M_PI 3.14159265358979323846264338327950288

std::mt19937_64 rng;

double AxisLength = 1.0;

double mouseX = 0, mouseY = 0;
GLint lmb, rmb;

std::vector<Object*> objects;

bool ShowAxes = true;

CameraMode Mode = FLY;

int menuHandle;

bool LightEnabled = false;

Objects LoadedObject = Objects::ROBOT;

const float Light_Red[] = {1, 0, 0, 1};
const float Light_Green[] = {0, 1, 0, 1};
const float Light_Blue[] = {0, 0, 1, 1};
const float Light_Yellow[] = {1, 1, 0, 1};
const float Light_Ourple[] = {1, 0, 1, 1};
const float Light_Teal[] = {0, 1, 1, 1};

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

void GenerateRobots(int num) {
    objects.clear();
    objects.reserve(num);

    std::uniform_int_distribution dist(-10, 10);

    for (int i = 0; i < num; i++) {
        Robot* r = new Robot;

        r->position = {dist(rng), 0.5, dist(rng)};

        objects.emplace_back(r);

        r = nullptr;
    }
}

void GenerateDogs(int num) {
    objects.clear();
    objects.reserve(num);

    std::uniform_int_distribution disg(-10, 10);

    for (int i = 0; i < num; i++) {
        Dog* d = new Dog;

        d->position = {disg(rng), 0.5, disg(rng)};

        objects.emplace_back(d);
    }
}

void DrawPlane() {
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(1, 1, 1);

    glVertex3f(-20, -2, -20);
    glVertex3f(-20, -2, 20);
    glVertex3f(20, -2, 20);
    glVertex3f(20, -2, -20);
    glEnd();

    glPopMatrix();
}

void DisplaySlay() {
    double tmp = Time::elapsedTime;
    Time::elapsedTime = static_cast<double>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;  // GLUT_ELAPSED_TIME is in seconds
    Time::deltaTime = Time::elapsedTime - tmp;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (Mode == ORBIT)
        gluLookAt(OrbitCamera::x, OrbitCamera::y, OrbitCamera::z, 0, 0, 0, 0, 1, 0);
    else
        gluLookAt(FlyCamera::x, 1.0, FlyCamera::z, FlyCamera::getX(), 1, FlyCamera::getZ(), 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto& o : objects)
        o->draw();

    if (ShowAxes)
        Axes();

    DrawPlane();

flush:
    glFlush();
    glutSwapBuffers();
}

void recomputeCamera() {
    OrbitCamera::x = OrbitCamera::radius * sinf(OrbitCamera::theta) * sinf(OrbitCamera::phi);
    OrbitCamera::z = OrbitCamera::radius * cosf(OrbitCamera::theta) * sinf(OrbitCamera::phi);
    OrbitCamera::y = OrbitCamera::radius * cosf(OrbitCamera::phi);
}

void mouseCallback(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        lmb = state;

    if (button == GLUT_RIGHT_BUTTON)
        rmb = state;

    if (state == GLUT_DOWN) {
        if (button == 3) {
            OrbitCamera::radius -= 0.05;

            if (OrbitCamera::radius < 2)
                OrbitCamera::radius = 2;

            recomputeCamera();
        }

        if (button == 4) {
            OrbitCamera::radius += 0.05;

            if (OrbitCamera::radius > 10)
                OrbitCamera::radius = 10;

            recomputeCamera();
        }
    }

    mouseX = x;
    mouseY = y;
}

void mouseMotion(int x, int y) {
    if (Mode != ORBIT) {
        mouseX = x;
        mouseY = y;
        return;
    }

    if (lmb == GLUT_DOWN) {
        OrbitCamera::theta += (mouseX - x) * 0.005;
        OrbitCamera::phi += (mouseY - y) * 0.005;

        // std::cout << cameraTheta << ", " << cameraPhi << ", " << cameraRadius << std::endl;

        if (OrbitCamera::phi <= 0)
            OrbitCamera::phi = 0 + 0.001;
        if (OrbitCamera::phi >= M_PI)
            OrbitCamera::phi = M_PI - 0.001;

        recomputeCamera();
    } else if (rmb == GLUT_DOWN) {
        double change = (x - mouseX) + (y - mouseY);

        OrbitCamera::radius += change * 0.01;

        if (OrbitCamera::radius <= 2.0)
            OrbitCamera::radius = 2.0;
        if (OrbitCamera::radius >= 10.0)
            OrbitCamera::radius = 10.0;

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
        case '1':
            for (auto& r : objects)
                r->setWireframe(true);
            break;
        case '2':
            for (auto& r : objects)
                r->setWireframe(false);
            break;
        case '3':
            ShowAxes = !ShowAxes;
            break;
        case '4':
            for (auto& o : objects) {
                if (LoadedObject == ROBOT) {
                    auto temp = dynamic_cast<Robot*>(o);

                    temp->lowerLeftArmRotatePoint->rotation.y -= 5;
                    temp->lowerRightArmRotatePoint->rotation.y += 5;
                } else {
                    auto temp = dynamic_cast<Dog*>(o);

                    temp->rfJoint->rotation.x -= 5;
                    temp->lfJoint->rotation.x += 5;
                    temp->rbJoint->rotation.x -= 5;
                    temp->lbJoint->rotation.x += 5;
                }
            }
            break;
        case '5':  // Dog Tail

            for (auto& o : objects) {
                if (LoadedObject == ROBOT) {
                    auto r = dynamic_cast<Robot*>(o);

                    r->upperRightArmRotatePoint->rotation.z += 5;
                    r->upperLeftArmRotatePoint->rotation.z -= 5;
                } else {
                    auto d = dynamic_cast<Dog*>(o);

                    d->tailJoint->rotation.x += 5;
                }
            }
            break;
        case '6':  // Dog Neck
            for (auto& o : objects) {
                if (LoadedObject == ROBOT) {
                    auto r = dynamic_cast<Robot*>(o);

                    r->upperRightLegRotatePoint->rotation.x += 5;
                    r->upperLeftLegRotatePoint->rotation.x -= 5;
                } else {
                    auto d = dynamic_cast<Dog*>(o);

                    d->headJoint->rotation.y += 5;
                }
            }
            break;
        case '7':  // No Dog
            if (LoadedObject == DOG)
                break;
            for (auto& r : objects) {
                auto temp = dynamic_cast<Robot*>(r);

                temp->lowerRightLegRotatePoint->rotation.x += 5;
                temp->lowerLeftLegRotatePoint->rotation.x += 5;
            }
            break;
        case 27:
            std::cout << "さよなら、全てのエバンゲリオン。";
            exit(0);
            break;
        case 't':
            Mode = Mode == ORBIT ? FLY : ORBIT;
            break;
        case 'm':
            if (LoadedObject == ROBOT) {
                LoadedObject = DOG;
                GenerateDogs(10);
            } else {
                LoadedObject = ROBOT;
                GenerateRobots(10);
            }

            std::cout << "Loaded " << (LoadedObject == ROBOT ? "Robot " : "Dog ") << "Model\n";

            break;
        default:
            std::cout << "How did we get here";
            break;
    }
}

void specialKeyCallback(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F4:
            for (auto& o : objects) {
                if (LoadedObject == ROBOT) {
                    auto temp = dynamic_cast<Robot*>(o);

                    temp->lowerLeftArmRotatePoint->rotation.y += 5;
                    temp->lowerRightArmRotatePoint->rotation.y -= 5;
                } else {
                    auto temp = dynamic_cast<Dog*>(o);

                    temp->rfJoint->rotation.x += 5;
                    temp->lfJoint->rotation.x -= 5;
                    temp->rbJoint->rotation.x += 5;
                    temp->lbJoint->rotation.x -= 5;
                }
            }
            break;
        case GLUT_KEY_F5:
            for (auto& o : objects) {
                if (LoadedObject == ROBOT) {
                    auto r = dynamic_cast<Robot*>(o);

                    r->upperRightArmRotatePoint->rotation.z -= 5;
                    r->upperLeftArmRotatePoint->rotation.z += 5;
                } else {
                    auto d = dynamic_cast<Dog*>(o);

                    d->tailJoint->rotation.x -= 5;
                }
            }
            break;
        case GLUT_KEY_F6:
            for (auto& o : objects) {
                if (LoadedObject == ROBOT) {
                    auto r = dynamic_cast<Robot*>(o);

                    r->upperRightLegRotatePoint->rotation.x -= 5;
                    r->upperLeftLegRotatePoint->rotation.x += 5;
                } else {
                    auto d = dynamic_cast<Dog*>(o);

                    d->headJoint->rotation.y -= 5;
                }
            }
            break;
        case GLUT_KEY_F7:
            if (LoadedObject == DOG)
                break;

            for (auto& r : objects) {
                auto t = dynamic_cast<Robot*>(r);
                t->lowerRightLegRotatePoint->rotation.x -= 5;
                t->lowerLeftLegRotatePoint->rotation.x -= 5;
            }
            break;
        case GLUT_KEY_UP:
            if (Mode == ORBIT)
                break;

            FlyCamera::x += FlyCamera::lx * 0.1;
            FlyCamera::z += FlyCamera::lz * 0.1;
            break;
        case GLUT_KEY_DOWN:
            if (Mode == ORBIT)
                break;

            FlyCamera::x -= FlyCamera::lx * 0.1;
            FlyCamera::z -= FlyCamera::lz * 0.1;
            break;
        case GLUT_KEY_RIGHT:
            if (Mode == ORBIT)
                break;

            FlyCamera::angle += 0.05;
            FlyCamera::lx = sin(FlyCamera::angle);
            FlyCamera::lz = -cos(FlyCamera::angle);
            break;
        case GLUT_KEY_LEFT:
            if (Mode == ORBIT)
                break;

            FlyCamera::angle -= 0.05;
            FlyCamera::lx = sin(FlyCamera::angle);
            FlyCamera::lz = -cos(FlyCamera::angle);
            break;
        default:
            std::cout << "We suffer, pain, dying.\n";
            break;
    }
}

void printInstructions() {
    std::cout << "T: Toggle Orbit/Flycam\n"
                 "Right Mouse: Open Menus\n"
                 "Middle Mouse: Zoom when in Orbit Mode\n"
                 "M: Toggle Dog Model/Robot Model\n"
                 "1: display a wireframe (mesh only) model\n"
                 "2 : display a solid model\n"
                 "3 : toggle to on/off the three axes\n"
                 "4 : increase the elbow angle by 5 degrees for both arms (Y Axis) (the left and right arms)\n"
                 "F4 : decrease the elbow angle by 5 degrees for both arms (Y Axis) (the left and right arms)\n"
                 "5 : increase the shoulder angle by 5 degrees for both arms (Z Axis) (the left and right arms)\n"
                 "F5 : decrease the shoulder angle by 5 degrees for both arms (Z Axis) (left and right arms)\n"
                 "6: increase the angles of the upper leg joints by 5 degrees (X Axis) for both legs\n"
                 "F6: decrease the angles of the upper leg joints by 5 degrees (X Axis) for both legs\n"
                 "7: increase the angles of the lower leg joints by 5 degrees (X Axis) for both legs\n"
                 "F7: decrease the angles of the lower leg joints by 5 degrees (X Axis) for both legs\n"
                 "ESC: terminate the program\n"
                 "7 and F7 Does nothing on the Dog Model.\n"
              << std::endl;
}

/*********** Menu Callbacks ********** */
void MainMenuCallback(int v) {
    switch (v) {
        default:
            printf("I cannot find the swag you are looking for...");
    }
}

void LightingCallback(int v) {
    switch (v) {
        case 0:
            LightEnabled ? glDisable(GL_LIGHTING) : glEnable(GL_LIGHTING);
            LightEnabled = !LightEnabled;
            printf("Toggle Light\n");
            break;
        default:
            printf("Unable to fart.\n");
    }
}

void LightingAmbientCallback(int v) {
    switch (v) {
        case 0:  // Red
            glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Red);
            break;
        case 1:  // Green
            glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Green);
            break;
        case 2:  // Blue
            glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Blue);
            break;
        case 3:  // Yellow
            glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Yellow);
            break;
        case 4:  // Ourple
            glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ourple);
            break;
        case 5:  // Teal
            glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Teal);
            break;
        default:
            std::cout << "OH my";
            break;
    }
}

void LightingSpecularCallback(int v) {
    switch (v) {
        case 0:  // Red
            glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Red);
            break;
        case 1:  // Green
            glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Green);
            break;
        case 2:  // Blue
            glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Blue);
            break;
        case 3:  // Yellow
            glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Yellow);
            break;
        case 4:  // Ourple
            glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Ourple);
            break;
        case 5:  // Teal
            glLightfv(GL_LIGHT0, GL_SPECULAR, Light_Teal);
            break;
        default:
            std::cout << "OH my";
            break;
    }
}

void ModelColorCallback(int v) {
    glm::vec3 color;

    switch (v) {
        case 0:
            color = {1, 0, 0};
            break;
        case 1:
            color = {0, 1, 0};
            break;
        case 2:
            color = {0, 0, 1};
            break;
        case 3:
            color = {1, 1, 0};
            break;
        case 4:
            color = {1, 0, 1};
            break;
        case 5:
            color = {0, 1, 1};
            break;
    }

    for (auto& o : objects)
        o->setColor(color);
}

void EmptyCallback(int v) {}

void initMenus() {
    auto lightingAmbientMenu = glutCreateMenu(LightingAmbientCallback);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);
    glutAddMenuEntry("Yellow", 3);
    glutAddMenuEntry("Purple", 4);
    glutAddMenuEntry("Teal", 5);

    auto lightingSpecularMenu = glutCreateMenu(LightingSpecularCallback);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);
    glutAddMenuEntry("Yellow", 3);
    glutAddMenuEntry("Purple", 4);
    glutAddMenuEntry("Teal", 5);

    auto lightingColorMenu = glutCreateMenu(EmptyCallback);
    glutAddSubMenu("Ambient", lightingAmbientMenu);
    glutAddSubMenu("Specular", lightingSpecularMenu);

    auto lightingMenu = glutCreateMenu(LightingCallback);
    glutAddMenuEntry("Toggle On/Off", 0);
    glutAddSubMenu("Change Color", lightingColorMenu);

    auto modelColorMenu = glutCreateMenu(ModelColorCallback);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);
    glutAddMenuEntry("Yellow", 3);
    glutAddMenuEntry("Purple", 4);
    glutAddMenuEntry("Teal", 5);

    // auto modelMenu;

    menuHandle = glutCreateMenu(MainMenuCallback);
    glutAddSubMenu("Lighting", lightingMenu);
    glutAddSubMenu("Model Color", modelColorMenu);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(int a, char** v) {
    glutInit(&a, v);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);  // RGBA Double Buffer Depth mode
    glutInitWindowSize(WIN_W, WIN_H);                           // window size
    glutInitWindowPosition(WIN_X, WIN_Y);

    glutCreateWindow("Jens Eckert jeckert5");

    glEnable(GL_DEPTH_TEST);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    float lightCol[4] = {1, 1, 1, 1};
    float ambientCol[4] = {0.3, 0.3, 0.3, 1.0};
    float lPosition[4] = {10, 10, 10, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightCol);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientCol);
    // glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 1.0);  // clear the window screen
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, WIN_W / WIN_H, 0.1, 1000);

    glutDisplayFunc(DisplaySlay);
    glutIdleFunc(DisplaySlay);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(mouseMotion);
    glutReshapeFunc(resizeWindow);
    glutKeyboardFunc(keyboardCallback);
    glutSpecialFunc(specialKeyCallback);

    initMenus();

    recomputeCamera();

    printInstructions();

    GenerateRobots(10);
    // GenerateDogs(10);
}

int main(int argc, char** argv) {
    std::random_device rd;
    rng.seed(rd());

    init(argc, argv);

    glutMainLoop();

    return 0;
}
