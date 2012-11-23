#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "treeskeleton.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "data_structures.h"
#include "Debug.h"

// TIME
long long now = TIME_CURRENT_MILLIS;

// FPS
#ifdef FPSLIMIT
long long fpstime = TIME_CURRENT_MILLIS;
int maxfpstime = 1000/MAXFPS; // 1000 msec / fps
#endif

// ROTATE
#ifdef ROTATE
bool viewrotate = ROTATEINITIAL;
#endif

using namespace std;

double eye_radius = 5;
double upangle = 10*M_PI/180;
double sideangle = 0;
triplet eye =
	{
		sqrt(eye_radius*eye_radius - eye_radius*sin(upangle)*eye_radius*sin(upangle))*sin(sideangle),
		eye_radius*sin(upangle),
		sqrt(eye_radius*eye_radius - eye_radius*sin(upangle)*eye_radius*sin(upangle))*cos(sideangle)
	};


double upangle_inc = 10*M_PI/180;
double sideangle_inc = 10*M_PI/180;
double global_time;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

TreeSkeleton tree(4,4);
Wind wind(2,5,2.5);

int TreeWindow, DirectionWindow;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glutSetWindow(TreeWindow);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;

    glutSetWindow(DirectionWindow);
    glutPositionWindow(0, height-150);
    glViewport(0, 0, 150, 150);

}

static void redisplay(int value)
{
	glutPostRedisplay();
}

#ifdef ROTATE
static void rotateview()
{
	if(viewrotate)
	{
		sideangle = fmod(sideangle + sideangle_inc * ROTATE_FACTOR, 2*M_PI);
		double smaller_radius = sqrt(eye_radius*eye_radius - eye[1]*eye[1]);
		eye[0] = smaller_radius*sin(sideangle);
		eye[2] = smaller_radius*cos(sideangle);
	}
}
#endif

static void display(void)
{
	#ifdef ROTATE
	rotateview();
	#endif

	#ifdef PROFILING
	long long diff = TIME_CURRENT_MILLIS;
	cout << "[TIME] [DRAW] : " << diff - now << endl;
	now = TIME_CURRENT_MILLIS;
	#endif
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0],eye[1],eye[2],0,2,0,0,1,0);

    //global_time += 1;
    glPushMatrix();
	long long timenow = TIME_CURRENT_MILLIS;
	tree.paint(timenow);
    glPopMatrix();

	#ifdef PROFILING
	diff = TIME_CURRENT_MILLIS;
	cout << "[TIME] [PRESWAP] : " << diff - now << endl;
	now = TIME_CURRENT_MILLIS;
	#endif
    glutSwapBuffers();

	#ifdef PROFILING
	diff = TIME_CURRENT_MILLIS;
	cout << "[TIME] [POSTSWAP] : " << diff - now << endl;
	now = TIME_CURRENT_MILLIS;
	#endif

	// Next Frame
	tree.nextFrame(global_time++, wind);

	#ifdef FPSLIMIT
		long long fpsnow = TIME_CURRENT_MILLIS;
		if((unsigned int) (fpsnow - fpstime) > maxfpstime)
			glutPostRedisplay();
		else
			glutTimerFunc((unsigned int) (maxfpstime - (fpsnow - fpstime)), redisplay,0);
		fpstime = fpsnow;
	#else
		glutPostRedisplay();
	#endif

	#ifdef PROFILING
	diff = TIME_CURRENT_MILLIS;
	cout << "[TIME] [POSTREDISPLAY] : " << diff - now << endl << endl;
	now = TIME_CURRENT_MILLIS;
	#endif
}

static void xyz()
{
    glColor3f(1,0,0);
    glRasterPos2f(0.8,-0.9);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'X');
    glColor3f(0,1,0);
    glRasterPos2f(0.15,-0.9);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Y');
    glColor3f(0,0,1);
    glRasterPos2f(-0.5,-0.9);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');
}

static void displaydir(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1,1,-1,1,1,3);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,-1,0,0,0,0,1,0);
    xyz();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1.0, 1.0, 3.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0]/2,eye[1]/2,eye[2]/2,0,0,0,0,1,0);

    glPushMatrix();
        glEnable(GL_COLOR_MATERIAL);
        glBegin(GL_LINES);
            glColor3f(1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(1,0,0);
        glEnd();
        glBegin(GL_LINES);
            glColor3f(0,1,0);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
        glEnd();
        glBegin(GL_LINES);
            glColor3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,0,1);
        glEnd();
        glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glutSwapBuffers();
}


static void specialKey(int key, int x, int y)
{
    bool validKey = false;
    switch (key)
    {
        case GLUT_KEY_LEFT :
            sideangle += sideangle_inc;
            validKey = true;
            break;
        case GLUT_KEY_RIGHT :
            sideangle -= sideangle_inc;
            validKey = true;
            break;
        case GLUT_KEY_UP :
            upangle -= upangle_inc;
            validKey = true;
            break;
        case GLUT_KEY_DOWN :
            upangle += upangle_inc;
            validKey = true;
            break;
    }
    if(validKey)
    {
        double eye_height = eye_radius*sin(upangle);
        double smaller_radius = sqrt(eye_radius*eye_radius - eye_height*eye_height);
        eye[0] = smaller_radius*sin(sideangle);
        eye[1] = eye_height;
        eye[2] = smaller_radius*cos(sideangle);
    }

    glutSetWindow(TreeWindow);
    glutPostRedisplay();
    glutSetWindow(DirectionWindow);
    glutPostRedisplay();
}

static void key(unsigned char key, int x, int y)
{
	std::cout << "GOT KEY : " << key << std::endl;
    switch (key)
    {
		#ifdef ROTATE
        case 32 :
			viewrotate=!viewrotate;
			glutSetWindow(TreeWindow);
			glutPostRedisplay();
			glutSetWindow(DirectionWindow);
			glutPostRedisplay();
			break;
		#endif
		case 27:
        case 'q':
            exit(0);
            break;

    }

    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_HEIGHT) - glutGet(GLUT_SCREEN_HEIGHT_MM));
    glutInitWindowPosition(10,10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

    TreeWindow = glutCreateWindow("Tree Animation");

	glEnable( GL_LINE_SMOOTH );
    glEnable( GL_POLYGON_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    //glutIdleFunc(idle);
    glutSpecialFunc(specialKey);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    // glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    // glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glDisable(GL_COLOR_MATERIAL);

    DirectionWindow = glutCreateSubWindow(TreeWindow, 0, glutGet(GLUT_WINDOW_HEIGHT)-150, 150, 150);
    glutDisplayFunc(displaydir);
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);

    glutSetWindow(TreeWindow);

    srand((unsigned) time(NULL));
    tree.makeTree();

    glutMainLoop();

    return EXIT_SUCCESS;
}
