#include <GL/glut.h>
#include "pang_constants.h"

static void Pang_renderScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.1, 50, 50);

	glDisable(GL_DEPTH_TEST);

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowInitWidth, WindowInitHeight);
	glutCreateWindow("Simple Pang");

	glutDisplayFunc(Pang_renderScene);

	glutMainLoop();
}