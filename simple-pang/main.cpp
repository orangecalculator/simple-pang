
#include "Pang.h"
#include "PangIO.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"
#include <GL/glut.h>
#include <iostream>

#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include <Windows.h>
#include <mmsystem.h>
#include <winnt.h>

#include "FreeImage.h"

#pragma comment(lib, "winmm.lib")

#include "Texture.h"
#include "Text.h"

#include "Light.h"

using namespace std;

static Player P;
static PangIO PIO(P);

Light light(1, 1, 1 / 2, GL_LIGHT0);


//void initTexture() {
//
//}

void drawSquareWithTexture() {
	glEnable(GL_TEXTURE_2D);

	static Texture bgImage("background_image.png");
	bgImage.draw();

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

	static Texture pImage("player_image.jpg");
	pImage.draw();

	glBegin(GL_QUADS);

	float x = P.getCoord()[0];
	float y = P.getCoord()[1];
	//cout << x << " " << y << endl;
	float size = PlayerCollideBoxSize;
	glTexCoord2f(0, 0); glVertex3f(x - size, y - size, 0.0);
	glTexCoord2f(0, 1); glVertex3f(x - size, y + size, 0.0);
	glTexCoord2f(1, 1); glVertex3f(x + size, y + size, 0.0);
	glTexCoord2f(1, 0); glVertex3f(x + size, y - size, 0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}



void displayFrameCount() {
	static Text FrameDisplay({ GameFrameLeft, GameFrameUp });

	std::stringstream frametext;

	frametext << "Frame No ";
	frametext << getFrameCount();
	FrameDisplay.draw(frametext.str());
}

void displayLife() {

	static Text LifeDisplay({ GameFrameLeft, GameFrameDown });

	if(P.getLife() > 0)
		LifeDisplay.draw(std::string(P.getLife(), 'L'));
}

static void Pang_Init() {
	P.setCoord(Init_PlayerPosition_x, Init_PlayerPosition_y);

	//initTexture();

	light.setAmbient(0.5, 0.5, 0.5, 1.0);
	light.setDiffuse(0.7, 0.7, 0.7, 1.0);
	light.setSpecular(1.0, 1.0, 1.0, 1.0);

	blocks.push_back(new OuterFrameBlock(GameFrameLeft, GameFrameRight, GameFrameUp, GameFrameDown));
	blocks.push_back(new Block(0.4, 0.5, 0.1, -0.1));
	//blocks.push_back(new Block(0.11, 0.3, 0.2, -0.2));

	balls.push_back(Ball(-BallMaxSize / 2, 0, BallMaxSize / 2, false));
	balls.push_back(Ball(+BallMaxSize / 2, 0, BallMaxSize / 2, true));


	// 게임 소리 나게 하는 코드. 실제 제출 시에는 comment in해야 함.
	//PlaySound(TEXT("bgm.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

static void Pang_Exit() {
	for (Block* PB : blocks)
		delete PB;
	blocks.clear();
}

static void Pang_IdleAction() {
	while (!IsLastFrame()) {
		ProceedFrame();

		PIO.submit();
		P.nextframe();

		for (Ball& B : balls) {
			B.nextframe();
		}

		////harpoon collision
		for (auto it = balls.begin(); it != balls.end(); ++it) {
			if (it->collision(P.getCoord()[0], P.getCoord()[1], P.getCoord()[0], P.getHarpoon())) {
				const double BcoordX = it->getcoordX();
				const double BcoordY = it->getcoordY();
				const double newradius = it->getradius() / 2;

				balls.erase(it);

				// 작살을 3번 맞으면 공이 사라짐. 2 ** 3 = 8
				if (newradius > 0.1 / 8) {
					balls.push_back(Ball(BcoordX - newradius, BcoordY, newradius, false));
					balls.push_back(Ball(BcoordX + newradius, BcoordY, newradius, true));
				}

				

				P.useHarpoon();

				break;
			}
		}

		for (const Ball& B : balls)
			P.checkcollision(B);


		if (P.getLife() <= 0) {
			glutIdleFunc(NULL);
			glutKeyboardFunc(NULL);
			glutSpecialFunc(NULL);
		}
	}

	glutPostRedisplay();
}

static void Pang_renderScene() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawSquareWithTexture();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	for (const Block* PB : blocks)
		PB->draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	P.draw();
	drawPlayerTexture();

	glColor3f(1, 0, 0);

	for (const Ball& B : balls)
		B.draw();

	glEnable(GL_COLOR_MATERIAL);
	

	glDisable(light.getID());
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	displayFrameCount();
	displayLife();

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

static int Pang_main(int * Pargc, char* argv[]) {
	glutInit(Pargc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowInitWidth, WindowInitHeight);
	glutCreateWindow("Simple Pang");

	Pang_Init();

	glutDisplayFunc(Pang_renderScene);
	glutIdleFunc(Pang_IdleAction);
	glutKeyboardFunc(Pang_KeyboardAction);
	glutSpecialFunc(Pang_SpecialKeyboardAction);

	glutMainLoop();

	return 0;
}

int main(int argc, char* argv[]) {
	return Pang_main(&argc, argv);
}