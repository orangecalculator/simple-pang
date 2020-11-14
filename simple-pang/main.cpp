//#include <vector>

#include <GL/glut.h>
#include "Pang.h"
#include "PangIO.h"
#include "Player.h"
#include "Ball.h"

static Player P;
static PangIO PIO(P);

static Ball sampleBall(0, 0, 0.05, true);

static void Pang_Init() {
	P.setCoord(Init_PlayerPosition_x, Init_PlayerPosition_y);
}

static void Pang_IdleAction() {
	while (!IsLastFrame()) {
		ProceedFrame();

		PIO.submit();
		P.nextframe();
		sampleBall.nextframe();
	}

	glutPostRedisplay();
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
	sampleBall.draw();

	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

static void Pang_KeyboardAction(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		PIO.setkeySPACE();
		break;
	}

	glutPostRedisplay();
}
static void Pang_SpecialKeyboardAction(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		PIO.setkeyLEFT();
		break;
	case GLUT_KEY_UP:
		PIO.setkeyUP();
		break;
	case GLUT_KEY_RIGHT:
		PIO.setkeyRIGHT();
		break;
	case GLUT_KEY_DOWN:
		PIO.setkeyDOWN();
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowInitWidth, WindowInitHeight);
	glutCreateWindow("Simple Pang");

	Pang_Init();

	glutDisplayFunc(Pang_renderScene);
	glutIdleFunc(Pang_IdleAction);
	glutKeyboardFunc(Pang_KeyboardAction);
	glutSpecialFunc(Pang_SpecialKeyboardAction);

	glutMainLoop();
}