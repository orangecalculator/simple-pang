//#include <vector>

#include <GL/glut.h>
#include "pang_constants.h"
#include "Player.h"

static Player P;

static void Pang_Init() {
	P.setCoord(Init_PlayerPosition_x, Init_PlayerPosition_y);
}

static void Pang_renderScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	glVertex2f(GameFrameLeft, GameFrameUp);
	glVertex2f(GameFrameLeft, GameFrameDown);
	glVertex2f(GameFrameRight, GameFrameDown);
	glVertex2f(GameFrameRight, GameFrameUp);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	P.draw();

	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowInitWidth, WindowInitHeight);
	glutCreateWindow("Simple Pang");

	Pang_Init();

	glutDisplayFunc(Pang_renderScene);

	glutMainLoop();
}