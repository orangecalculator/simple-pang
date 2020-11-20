
#include "Pang.h"
#include "PangIO.h"
#include "Player.h"
#include "Ball.h"
#include <GL/glut.h>
#include <iostream>

#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <Windows.h>
#include <mmsystem.h>
#include <winnt.h>

#include "FreeImage.h"


#pragma comment(lib, "winmm.lib")


using namespace std;

static GLuint bgImageID, pImageID;
GLubyte* bgImageData; 
GLubyte* pImageData;
int bgImageWidth, bgImageHeight, pImageWidth, pImageHeight;

static Player P;
static PangIO PIO(P);

static Ball sampleBall(0, 0, 0.2, true);

#define boundaryX WindowInitWidth/2
#define boundaryY WindowInitHeight/2


FIBITMAP* createBitMap(char const* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1) {
		cout << "Could not find image: " << filename << " - Aborting." << endl;
		exit(-1);
	}

	if (format == FIF_UNKNOWN) {
		cout << "Couldn't determine file format - attempting to get from file extension..." << endl;
		format = FreeImage_GetFIFFromFilename(filename);

		if (!FreeImage_FIFSupportsReading(format)) {
			cout << "Detected image format cannot be read!" << endl;
			exit(-1);
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;
	if (bitsPerPixel == 32) {
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Skipping conversion." << endl;
		bitmap32 = bitmap;
	}
	else {
		cout << "Source image has " << bitsPerPixel << " bits per pixel. Converting to 32-bit colour." << endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	return bitmap32;
}

void generateBGImage() {
	glGenTextures(1, &bgImageID);
	glBindTexture(GL_TEXTURE_2D, bgImageID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bgImageWidth, bgImageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, bgImageData);
}

void generatePIMage() {
	glGenTextures(2, &pImageID);
	glBindTexture(GL_TEXTURE_2D, pImageID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImageWidth, pImageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pImageData);
}

void initTexture() {
	FIBITMAP* bg_image = createBitMap("meteo.jpg");
	bgImageWidth = FreeImage_GetWidth(bg_image);
	bgImageHeight = FreeImage_GetHeight(bg_image);
	bgImageData = FreeImage_GetBits(bg_image);

	FIBITMAP* p_image = createBitMap("yw.jpg");
	pImageWidth = FreeImage_GetWidth(p_image);
	pImageHeight = FreeImage_GetHeight(p_image);
	pImageData = FreeImage_GetBits(p_image);

	generateBGImage();
	generatePIMage();
}

void drawSquareWithTexture() {
	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, bgImageID);

	glBegin(GL_QUADS);
	//glTexCoord2f(0, 0); glVertex3f(-boundaryX, -boundaryY / 1.5, 0.0);
	//glTexCoord2f(0, 1); glVertex3f(-boundaryX, boundaryY / 1.5, 0.0);
	//glTexCoord2f(1, 1); glVertex3f(boundaryX, boundaryY / 1.5, 0.0);
	//glTexCoord2f(1, 0); glVertex3f(boundaryX, -boundaryY / 1.5, 0.0);
	glTexCoord2f(0, 0); glVertex3f(-0.9, -0.9, 0.0);
	glTexCoord2f(0, 1); glVertex3f(-0.9, 0.9, 0.0);
	glTexCoord2f(1, 1); glVertex3f(0.9, 0.9, 0.0);
	glTexCoord2f(1, 0); glVertex3f(0.9, -0.9, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawPlayerTexture() {
	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, pImageID);

	glBegin(GL_QUADS);

	float x = P.getCoord()[0];
	float y = P.getCoord()[1];
	float size = 0.1;
	glTexCoord2f(0, 0); glVertex3f(x - size, y - size, 0.0);
	glTexCoord2f(0, 1); glVertex3f(x - size, y + size, 0.0);
	glTexCoord2f(1, 1); glVertex3f(x + size, y + size, 0.0);
	glTexCoord2f(1, 0); glVertex3f(x + size, y - size, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


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

	drawSquareWithTexture();


	glEnable(GL_DEPTH_TEST);


	//Game Inner Frame
	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	glVertex2f(GameFrameLeft, GameFrameUp);
	glVertex2f(GameFrameLeft, GameFrameDown);
	glVertex2f(GameFrameRight, GameFrameDown);
	glVertex2f(GameFrameRight, GameFrameUp);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Game Inner Frame End

	P.draw();
	drawPlayerTexture();

	if (sampleBall.collision(0, -0.1)) {
		glColor3f(1, 0, 0);
	}
	else if (sampleBall.collision(0.3, 0.1, -0.3, 0.1)) {
		glColor3f(0, 1, 0);
	}
	else
		glColor3f(0, 0, 1);

	sampleBall.draw();
	glColor3f(0, 1, 1);
	glBegin(GL_POINTS);
	glVertex3f(0, -0.1, 0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.3, 0.1, 0);
	glVertex3f(-0.3, 0.1, 0);
	glEnd();

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
	PlaySound(TEXT("bgm.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowInitWidth, WindowInitHeight);
	glutCreateWindow("Simple Pang");

	Pang_Init();
	initTexture();

	glutDisplayFunc(Pang_renderScene);
	glutIdleFunc(Pang_IdleAction);
	glutKeyboardFunc(Pang_KeyboardAction);
	glutSpecialFunc(Pang_SpecialKeyboardAction);

	glutMainLoop();
}