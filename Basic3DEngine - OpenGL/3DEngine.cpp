

#include <stdlib.h>             // to define exit(0)

#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>

#include <GLUT/glut.h>
#  include <GLUT/glut.h>

#  include <OpenGL/glext.h>
#else
#  include <gl/glew.h>
#  include <gl/freeglut.h>
// #  include <gl/glext.h>
#  include <gl/glut.h>
#  include <gl/gl.h>

#pragma comment(lib, "glew32.lib") 
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int isSelecting = 0;
static unsigned int buffer[1024]; // Hit buffer.

enum ObjType { CUBE, TEAPOT, CONE, SPHERE, TORUS, OCTO };

GLfloat xRotated, yRotated, zRotated;

typedef struct
{
	ObjType type;

	float x;
	float y;
	float z;

	int selected;
} object;


static const int num_objects = 11;

static object objects[] =
{ { CUBE, 0, 0, -10, 0 },
{ TEAPOT, -4, 0, -10, 0 },
{ CONE, 4, 0, -10, 0 },
{ CONE, -2, 0, -12, 0 },
{ SPHERE, -8, 0, -10, 0 },
{ TORUS, 3, 7, -10, 0 },
{ TORUS, -3, 7, -10, 0 },
{ OCTO, -6, 0, -15, 0 },
{ OCTO, 10, 0, -8, 0 },
{ SPHERE, 6, 0, -10, 0 }
};


static float my_x = 0;
static float my_y = 0;
static float my_z = 0;
static float my_angle = 0;
static float my_other_angle = 0;
unsigned int global_minz = 0xffffffff;
unsigned int save_stack_top;


void drawFloor()
{
	glColor3f(.5, .5, .5);
	glBegin(GL_POLYGON);
	glVertex3f(-100, -1, -100);
	glVertex3f(100, -1, -100);
	glVertex3f(100, -1, 100);
	glVertex3f(-100, -1, 100);
	glEnd();

}



void drawObject(const object& o)
{
	glPushMatrix();
	glTranslatef(o.x, o.y, o.z);
	glLineWidth(1);


	if (o.selected)
	{
		glColor3f(1.0, 0.0, 0.0);

		// changing in transformation matrix
		// rotation about X axis
		glRotatef(xRotated, 1.0, 0.0, 0.0);

		// rotation about Y axis
		glRotatef(yRotated, 0.0, 1.0, 0.0);

		// rotation about Z axis
		glRotatef(zRotated, 0.0, 0.0, 1.0);
	}
	else
	{
		glColor3f(0.0, 1.0, 0.0);
	}

	switch (o.type)
	{
	case CUBE:
		glutSolidCube(1);
		glColor3f(0, 0, 0);
		glutWireCube(1);

		break;

	case TEAPOT:
		glutSolidTeapot(1);
		glColor3f(0, 0, 0);
		glutWireTeapot(1);

		break;


	case CONE:
		glTranslatef(0, -.5, 0);
		glRotatef(90, -1, 0, 0);
		glScalef(0.25, 0.25, 0.25);
		glutSolidCone(5, 10, 3, 3);
		glColor3f(0, 0, 0);
		glutWireCone(5, 10, 3, 3);
		break;

	case SPHERE:
		glTranslatef(0, -.3, 0);
		glRotatef(90, -1, 0, 0);
		glScalef(0.25, 0.25, 0.25);
		glutSolidSphere(5, 10, 3);
		glColor3f(0, 0, 0);
		glutWireSphere(5, 10, 3);
		break;

	case TORUS:
		glTranslatef(0, -.7, 0);
		glRotatef(60, -1, 0, 0);
		glScalef(0.25, 0.25, 0.25);
		glutSolidTorus(5, 10, 3, 4);
		glColor3f(0, 0, 0);
		glutWireTorus(5, 10, 3, 4);
		break;

	case OCTO:
		glTranslatef(0, -.7, 0);
		glRotatef(30, -1, 0, 0);
		glScalef(0.25, 0.25, 0.25);
		glutSolidOctahedron();
		glColor3f(0, 0, 0);
		glutWireOctahedron();

	}

	glPopMatrix();
}


// Drawing routine.

void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// calculate heading
	gluLookAt(my_x, my_y, my_z,
		//		what am I looking at???
		my_x - 5 * sin((M_PI / 180.0) * my_angle),
		my_y - 5 * sin((M_PI / 180) * my_other_angle),
		my_z - 5 * cos((M_PI / 180.0) * my_angle),
		0, 1, 0);



	drawFloor();
	for (int i = 0; i<num_objects; i++)
	{
		glLoadName(i);
		drawObject(objects[i]);
	}

	glutSwapBuffers();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 1);
}


// set up variables to process mouse
static int window_width;
static int window_height;
static int last_mouse_x;
static int last_mouse_y;



// OpenGL window reshape routine.
void resize(int w, int h)
{
	window_width = w;
	window_height = h;

	// initialize to center of window, so that things don't go crazy
	last_mouse_x = w / 2;
	last_mouse_y = h / 2;

	float aspect = w*1.0 / h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50, aspect, 0.1, 250);
	//   glFrustum(-5,5,-5,5,5,200);
	//   glOrtho(-5,5,-5,5,0,200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// The mouse callback routine
void pickFunction(int button, int state, int x, int y)
{
	int viewport[4]; // Viewport data

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return; // Don't react unless the left button is pressed

	glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data

	glSelectBuffer(1024, buffer); //Specify buffer to write hit records in selection mouse mode

	(void)glRenderMode(GL_SELECT); // Enter selection mode

	glMatrixMode(GL_PROJECTION);
	// Save the viewing volume defined in the resize routine

	glPushMatrix();

	// Define a viewing volume corresponding to selecting in 3 x 3 region around the cursor.
	glLoadIdentity();
	gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);
	// glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); // Copied from the reshape routine.
	gluPerspective(50, 1, 0.1, 250);

	glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
	glLoadIdentity();

	glInitNames(); // Initializes the name stack to empty.
	glPushName(0); // Puts name 0 on top of stack.

	isSelecting = 1;
	drawScene();
	isSelecting = 0;
	glPopName();

	int hits = glRenderMode(GL_RENDER);

	printf("clicked on %d thing[s]\n", hits);

	if (hits > 0)
	{
		unsigned int* ptr = buffer;

		global_minz = 0xffffffff;

		for (int i = 0; i<hits; i++)
		{
			ptr++;

			unsigned int minZ = *ptr;
			ptr += 2;
			unsigned int stack_top = *ptr;
			ptr++;

			printf("item %u at %f\n", stack_top, minZ*1.0 / 0xffffffff);

			if (minZ < global_minz)
			{
				global_minz = minZ;
				save_stack_top = stack_top;
			}

			printf("save_stack_top is %u at \n", save_stack_top);
		}
		objects[save_stack_top].selected = !objects[save_stack_top].selected;

	}


	// Restore viewing volume of the resize routine and return to modelview mode.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();

}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:                  // escape
		exit(0);
		break;
	default:
		break;
	}
}

void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: my_angle += 5;
		break;
	case GLUT_KEY_RIGHT: my_angle -= 5;
		break;
	case GLUT_KEY_UP:
		my_x -= 1 * sin((M_PI / 180.0)*my_angle);
		my_z -= 1 * cos((M_PI / 180.0)*my_angle);
		break;
	case GLUT_KEY_DOWN:
		my_x += 1 * sin((M_PI / 180.0)*my_angle);
		my_z += 1 * cos((M_PI / 180.0)*my_angle);
		break;
	}

	if (my_angle > 360.0) my_angle -= 360;
	if (my_angle < 0) my_angle += 360;

	//    printf("my angle: %f\n", my_angle);

	glutPostRedisplay();
}

// register this with glutPassiveMotionFunc
void trackMouse(int x, int y)
{
	last_mouse_x = x;
	last_mouse_y = y;
}

// register this with glutIdleFunc
void mousePositionIdleUpdate()
{
	// if the last recorded mouse position is near the horizontal window
	// boundaries, update the angle and redraw the scene
	if (last_mouse_x <= .3*window_width)
	{
		my_angle += 0.25;
		glutPostRedisplay();
	}
	else if (last_mouse_x >= .7*window_width)
	{
		my_angle -= 0.25;
		glutPostRedisplay();
	}

	if (last_mouse_y <= .3*window_height)
	{
		my_other_angle -= 0.25;
		glutPostRedisplay();
	}
	else if (last_mouse_y >= .7*window_height)
	{
		my_other_angle += 0.25;
		glutPostRedisplay();
	}

	// weird place to put this!! put it in it's own subroutine!
	// rotation around x axis
	xRotated += .1;
	glutPostRedisplay();

	// yRotated += 0.01;
	// zRotated += 0.01;
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("3D Engine");
	//	xRotated = yRotated = zRotated = 0.0;
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);
	glutMouseFunc(pickFunction);
	glutPassiveMotionFunc(trackMouse);
	glutIdleFunc(mousePositionIdleUpdate);


	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}


// Local Variables:
// compile-command: "g++ -framework OpenGL -framework GLUT -Wall -Wno-deprecated colored_objects.cpp -lglew"
// End: