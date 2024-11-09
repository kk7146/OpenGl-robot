#include "Robot.h"
#include <GL/glut.h>
#include "glk.h"
static Robot robot;
static bool bRobotRun = false;

#define RIGHT_UP_QUADRANT 0
#define LEFT_UP_QUADRANT 1
#define LEFT_DOWN_QUADRANT 2
#define RIGHT_DOWN_QUADRANT 3

#define LEFT_UP_VIEW(width, height) 0, height/2.0 , width/2.0, height/2.0
#define LEFT_DOWN_VIEW(width, height) 0, 0, width/2.0, height/2.0
#define RIGHT_UP_VIEW(width, height) width/2.0, height/2.0, width/2.0, height/2.0
#define RIGHT_DOWN_VIEW(width, height) width/2.0, 0, width/2.0, height/2.0

int previousQuadrant = RIGHT_UP_QUADRANT;

static double PrevX, PrevY;
static double InstDeltaX, InstDeltaY;
static double DeltaX, DeltaY;

double ModelX = 0;
double ModelY = 0;
double RotX = 0;
double RotY = 0;

double leftUpZoom = 1.0;
double leftDownZoom = 1.0;
double rightUpZoom = 1.0;
double rightDownZoom = 1.0;

int getQuadrant(double x, double y) {
    double location = atan2(y, x);
    location = (location < 0 ? location + 2 * M_PI : location) / M_PI_2;
    return (int)location % 4;
}

int getMouseQuadrant(double x, double y) {
    return (3 - getQuadrant(x, y)) % 4;
}

void handleViewLogic() {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    int mouseQuadrant = getMouseQuadrant(PrevX - width / 2.0, PrevY - height / 2.0);
    if (previousQuadrant != mouseQuadrant) {
        DeltaX = DeltaY = 0;
        previousQuadrant = mouseQuadrant;
    }

    
    switch (mouseQuadrant) {
        case LEFT_UP_QUADRANT:
            ModelX = DeltaX / 100.0;
            ModelX = 1 < ModelX ? 1 : ModelX;
            ModelX = -1 > ModelX ? -1 : ModelX;
            ModelY = DeltaY / 100.0;
            ModelY = 1 < ModelY ? 1 : ModelY;
            ModelY = -1 > ModelY ? -1 : ModelY;
            break;
        case LEFT_DOWN_QUADRANT:
            RotX = DeltaX;
            RotY = DeltaY;
            break;
        case RIGHT_UP_QUADRANT:
            if (InstDeltaX < 0) {
                rightUpZoom += 8 >= rightUpZoom ? 0.025 : 0;
            } else if (InstDeltaX > 0) {
                rightUpZoom += 0.25 <= rightUpZoom ? -0.025 : 0;
            }
            break;
        case RIGHT_DOWN_QUADRANT:
            if (InstDeltaX > 0) {
                robot.scale += 250 >= robot.scale ? 0.75 : 0;
            } else if (InstDeltaX < 0) {
                robot.scale += 25 <= robot.scale ? -0.75 : 0;
            }
            break;
    }
}

void renderView(int x, int y, int width, int height,
                double upx, double upy, double upz,
                double eyex, double eyey, double eyez, double zoom_where, bool useOrtho = true) {
    glViewport(x, y, width, height);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (useOrtho) {
        glOrtho(
                -1, 1, -1, 1,
                0, 10
        );
    } else {
        glFrustum(
                -1, 1, -1, 1,
                0, 10
        );
    }

    glMatrixMode(GL_MODELVIEW);
    gluLookAt(
            eyex, eyey, eyez,
            0, 0, 0,
            upx, upy, upz
    );

    handleViewLogic();
    glTranslated(ModelX, 0, ModelY);
    glRotated(RotX, 0, -1, 0);
    glRotated(RotY, 1, 0, 0);
    glScalef(zoom_where, zoom_where, zoom_where);

    robot.draw();
    glPopMatrix();
}

void mouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		PrevX = x;
		PrevY= y;
	}
}

void mouseMotion (int x, int y) {
	glMatrixMode(GL_MODELVIEW) ;
	glRotated(x - PrevX, 0, 1, 0);
	glRotated(y - PrevY, 1, 0, 0);
	PrevX = x;
	PrevY = y;
	glutPostRedisplay();
}
void display() {
	glClearColor (0.8, 0.9, 0.8, 1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    renderView(LEFT_UP_VIEW(width, height), 0, 0, 1, 0, 1, 0, leftUpZoom);
    renderView(LEFT_DOWN_VIEW(width, height), 0, 1, 0, 0, 0, -1, leftDownZoom);
    renderView(RIGHT_UP_VIEW(width, height), 0, 1, 0, 0.2, -0.2, 0.2, rightUpZoom, false);
    renderView(RIGHT_DOWN_VIEW(width, height), 0, 1, 0, 1, 0, 0, rightDownZoom);

	glutSwapBuffers();
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 'i') {
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	else if (key == 'z') {
        if (rightUpZoom < 8.0) {
            rightUpZoom += 0.1;
        }
    } else if (key == 'x') { 
        if (rightUpZoom > 0.5) {
            rightUpZoom -= 0.1;
        }
    }
	glutPostRedisplay();
}

void initRendering()
{
	GLfloat light_specular[] = { 0.8, 0.5, 0.8, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_position[] = {0.f, -2.5f, 2.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	GLfloat mat_specular[] = { 0.5, 1.0, 0.5, 1.0};
	GLfloat mat_shininess[] = { 70.0 };
	GLfloat mat_color[] = { 0.5, 0.5, 0.5, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
}

int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow ("RobotWorld");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	initRendering();
	glutMainLoop();
	return 0;
}