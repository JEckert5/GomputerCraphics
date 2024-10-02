#include <GL/freeglut.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>

#include <iostream>

#define WIN_W 600.0
#define WIN_H 600.0

double AspectRatio = WIN_W / WIN_H;

float Angle = 0;
bool ShowAxes = true;
bool DrawModel = true;
bool Wireframe = false;
bool Clear = false;
bool Rotate = false;
int MainMenu, RotationMenu, XMenu, YMenu, ZMenu, ProjectionMenu, BodyColorMenu;
float XRot = 0, YRot = 0, ZRot = 0;
float Red = 0.4, Green = 0.3, Blue = 0.7;
int Window;
double OrbitRadius = 5;
double OrbitAngle = 270;
double PrevX = 0, PrevY = 0;
float zoom = 1;

bool BW = false;

bool Animate = true;
bool LeftForward = false, RightForward = true;
double RightLegAngle = -45, LeftLegAngle = 45;

struct CGTime {
    static inline double deltaTime = 0.0;
    static inline double elapsedTime = 0.0;
};

float clampf(float v, float min, float max) {
    return std::max(min, std::min(v, max));
}

double clampd(double v, double min, double max) {
    return std::max<double>(min, (std::min<double>(v, max)));
}

void Head() {
    glPushMatrix();

    glTranslatef(0, 1, 0);

    if (Wireframe) {
        glutWireSphere(0.5, 20, 10);
    } else
        glutSolidSphere(0.5, 20, 10);

    glPopMatrix();
}

void Body() {
    glPushMatrix();

    glTranslatef(0, -0.2f, 0);
    glScalef(1, 1.5f, 1);

    if (Wireframe) {
        glutWireCube(1);
    } else
        glutSolidCube(1);

    glPopMatrix();
}

void LeftArm() {
    glPushMatrix();

    glTranslatef(-1.3f, -0.3f, 0);
    glRotatef(45, 0, 0, 1);
    glScalef(2, 0.5f, 0.8f);

    if (Wireframe) {
        glutWireCube(1);
    } else
        glutSolidCube(1);

    glPopMatrix();
}

void RightArm() {
    glPushMatrix();

    glTranslatef(1.3f, -0.3f, 0);
    glRotatef(-45, 0, 0, 1);
    glScalef(2, 0.5f, 0.8f);

    if (Wireframe) {
        glutWireCube(1);
    } else
        glutSolidCube(1);

    glPopMatrix();
}

void LeftLeg() {
    glPushMatrix();

    glTranslatef(-0.3f, -1, 0);
    glRotatef(LeftLegAngle, 1, 0, 0);
    glTranslatef(0, -1, 0);
    glScalef(0.5f, 2, 0.8f);

    if (Wireframe) {
        glutWireCube(1);
    } else
        glutSolidCube(1);

    glPopMatrix();
}

void RightLeg() {
    glPushMatrix();

    glTranslatef(0.3f, -1, 0);
    glRotatef(RightLegAngle, 1, 0, 0);
    glTranslatef(0, -1, 0);
    glScalef(0.5f, 2, 0.8f);

    if (Wireframe) {
        glutWireCube(1);
    } else {
        glutSolidCube(1);
    }

    glPopMatrix();
}

void DrawRobot() {
    glPushMatrix();

    if (Wireframe || BW) {
        glColor3f(1, 1, 1);
    } else {
        glColor3f(Red, Green, Blue);
    }

    glRotatef(XRot, 1, 0, 0);
    glRotatef(YRot, 0, 1, 0);
    glRotatef(ZRot, 0, 0, 1);

    Head();
    Body();
    LeftArm();
    RightArm();
    LeftLeg();
    RightLeg();

    glPopMatrix();
}

void drawAxes() {
    glPushMatrix();

    glBegin(GL_LINES);
    BW ? glColor3f(1, 1, 1) : glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glEnd();

    glBegin(GL_LINES);
    BW ? glColor3f(1, 1, 1) : glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);
    glEnd();

    glBegin(GL_LINES);
    BW ? glColor3f(1, 1, 1) : glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();

    glPopMatrix();
}

void display() {
    double temp = CGTime::elapsedTime;
    CGTime::elapsedTime = double(glutGet(GLUT_ELAPSED_TIME)) / 1000.0;
    CGTime::deltaTime = CGTime::elapsedTime - temp;

    OrbitAngle = Rotate ? OrbitAngle + 1 * CGTime::deltaTime : OrbitAngle;

    if (Animate) {
        if (LeftForward) {
            LeftLegAngle += 20.0 * CGTime::deltaTime;
            LeftForward = LeftLegAngle <= 45;
        } else {
            LeftLegAngle -= 20.0 * CGTime::deltaTime;
            LeftForward = LeftLegAngle <= -45;
        }

        if (RightForward) {
            RightLegAngle += 20.0 * CGTime::deltaTime;
            RightForward = RightLegAngle <= 45;
        } else {
            RightLegAngle -= 20.0 * CGTime::deltaTime;
            RightForward = RightLegAngle <= -45;
        }
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    OrbitAngle = clampf(OrbitAngle, 0, 360);

    double x = OrbitRadius * cos(OrbitAngle);
    double z = OrbitRadius * sin(OrbitAngle);

    gluLookAt(x, 0, z, 0, 0, 0, 0, 1, 0);

    if (Clear)
        goto flush;

    glPushMatrix();

    // glScalef(zoom, zoom, zoom);

    if (DrawModel) {
        DrawRobot();
    }

    if (ShowAxes) {
        drawAxes();
    }

    glPopMatrix();

flush:

    glFlush();
    glutSwapBuffers();
}

void keyboardCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':  // Wireframe
            Wireframe = true;
            break;
        case 's':  // Solid
            Wireframe = false;
            break;
        case 'c':  // Clear
            Clear = !Clear;
            break;
        case 'm':  // Show model
            DrawModel = !DrawModel;
            break;
        case 'a':  // Axis
            ShowAxes = !ShowAxes;
            break;
        case 27:  // esc
            exit(69);
            break;
        default:
            std::cout << "get rotated idiot." << std::endl;
            break;
    }
}

void mouseCallback(int button, int state, int x, int y) {
    if (state == 0) return;

    switch (button) {
        case 0:
            BW = !BW;
            break;
        case 3:
            OrbitRadius += 0.1;
            break;
        case 4:
            OrbitRadius -= OrbitRadius < 0.1 ? 0 : 0.1;
            break;
    }
}

void motionCallback(int x, int y) {
    if (Rotate) return;

    OrbitAngle += (float(x) - PrevX) / 50.f;

    PrevX = x;
    PrevY = y;
}

void idle() {
    glutPostRedisplay();
}

void timer(int arg) {
    glutTimerFunc(1, timer, arg);
}

void mainMenuCallback(int arg) {
    switch (arg) {
        case 0:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(65, 1, 0.1f, 1000);
            break;
        case 1:
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(-5, 5, -5, 5, -10, 10);
            break;
        case 2:
            zoom = 1;
            break;
        case 3:
            Rotate = !Rotate;
            OrbitAngle = 270;
            break;
        case 4:
            Animate = !Animate;
            break;
        default:
            std::cout << "Get Rotated you fool\n";
            break;
    }
}

void rotMenuCallback(int r) {
    switch (r) {
        case 0:  // X30
            XRot = 30;
            break;
        case 1:
            XRot = 60;
            break;
        case 2:
            XRot = 90;
            break;
        case 3:
            XRot = 120;
            break;
        case 4:
            XRot = 180;
            break;
        case 5:
            YRot = 30;
            break;
        case 6:
            YRot = 60;
            break;
        case 7:
            YRot = 90;
            break;
        case 8:
            YRot = 120;
            break;
        case 9:
            YRot = 180;
            break;
        case 10:
            ZRot = 30;
            break;
        case 11:
            ZRot = 60;
            break;
        case 12:
            ZRot = 90;
            break;
        case 13:
            ZRot = 120;
            break;
        case 14:
            ZRot = 180;
            break;
        case 15:
            XRot = 0;
            YRot = 0;
            ZRot = 0;
            break;
        default:
            std::cout << "oop" << std::endl;
            break;
    }
}

void bodyColorCallback(int c) {
    switch (c) {
        case 0:
            Red = 1;
            Green = 0;
            Blue = 0;
            break;
        case 1:
            Red = 0;
            Green = 1;
            Blue = 0;
            break;
        case 2:
            Red = 0;
            Green = 0;
            Blue = 1;
            break;
        default:
            std::cout << "What\n";
            break;
    }
}

void menuInit() {
    XMenu = glutCreateMenu(rotMenuCallback);
    glutAddMenuEntry("30", 0);
    glutAddMenuEntry("60", 1);
    glutAddMenuEntry("90", 2);
    glutAddMenuEntry("120", 3);
    glutAddMenuEntry("180", 4);

    YMenu = glutCreateMenu(rotMenuCallback);
    glutAddMenuEntry("30", 5);
    glutAddMenuEntry("60", 6);
    glutAddMenuEntry("90", 7);
    glutAddMenuEntry("120", 8);
    glutAddMenuEntry("180", 9);

    ZMenu = glutCreateMenu(rotMenuCallback);
    glutAddMenuEntry("30", 10);
    glutAddMenuEntry("60", 11);
    glutAddMenuEntry("90", 12);
    glutAddMenuEntry("120", 13);
    glutAddMenuEntry("180", 14);

    RotationMenu = glutCreateMenu(rotMenuCallback);
    glutAddSubMenu("X", XMenu);
    glutAddSubMenu("Y", YMenu);
    glutAddSubMenu("Z", ZMenu);
    glutAddMenuEntry("Reset", 15);

    ProjectionMenu = glutCreateMenu(mainMenuCallback);
    glutAddMenuEntry("Perspective", 0);
    glutAddMenuEntry("Orthographic", 1);

    BodyColorMenu = glutCreateMenu(bodyColorCallback);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);

    MainMenu = glutCreateMenu(mainMenuCallback);
    glutAddSubMenu("Rotation", RotationMenu);
    glutAddSubMenu("Projection", ProjectionMenu);
    glutAddSubMenu("Body Color", BodyColorMenu);
    glutAddMenuEntry("Reset Zoom", 2);
    glutAddMenuEntry("Toggle Camera Rotate", 3);
    glutAddMenuEntry("Toggle Animation", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void init(int* argc, char** argv) {
    glutInit(argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);

    Window = glutCreateWindow("XQuartz Sux - Jens Eckert | jeckert5");

    glClearColor(0, 0, 0, 1);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardCallback);
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    float pos[] = {10, 10, 10, 1};
    float ambient[] = {0.3f, 0.2f, 0.8f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

    menuInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // gluPerspective(70, 1, 0.1f, 1000);
    glOrtho(-5, 5, -5, 5, -10, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

    glutTimerFunc(1, timer, 0);
}

int main(int argc, char** argv) {
    init(&argc, argv);

    glutMainLoop();

    glutDestroyWindow(Window);

    return 0;
}
