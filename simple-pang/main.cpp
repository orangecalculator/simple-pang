
#include "Pang.h"
#include "PangIO.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"
#include <GL/glut.h>
#include <iostream>

#include <set>
#include <functional>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <random>
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
static OuterFrameBlock* OuterFrame;
static multiset<int, std::greater<int>> ScoreBoard;

int stage = 1;
int score;

int slowItemNumber = 4;

Light light(0.5, 0.5, 2.0, GL_LIGHT0);

void drawSquareWithTexture() {
	glEnable(GL_TEXTURE_2D);

	static Texture bgImage("background_image.png");
	bgImage.draw();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-0.9, -0.8, 1.0);
	glTexCoord2f(0, 1); glVertex3f(-0.9, 0.9, 1.0);
	glTexCoord2f(1, 1); glVertex3f(0.9, 0.9, 1.0);
	glTexCoord2f(1, 0); glVertex3f(0.9, -0.8, 1.0);
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

void displayScore() {
	static Text ScoreDisplay({ GameFrameRight / 2.0, GameFrameUp });

	std::stringstream scoretext;

	scoretext << "Score: ";
	scoretext << score;
	ScoreDisplay.draw(scoretext.str());
}

void displayScoreBoard() {
	Text ScoreBoardDisplay({ GameFrameRight / 2.0, GameFrameUp / 2.0 });

	ScoreBoardDisplay.draw("Scores");

	double displayY = ScoreBoardDisplay.getPosition()[1];
	std::stringstream scoretext;
	for (int score : ScoreBoard) {
		displayY -= DisplayInterval;
		ScoreBoardDisplay.setPosition(ScoreBoardDisplay.getPosition()[0], displayY);
		scoretext.clear();
		scoretext.str("");
		scoretext << score;
		ScoreBoardDisplay.draw(scoretext.str());
	}
}

void displaySlowItem() {

	static Text LifeDisplay({ GameFrameLeft, GameFrameDown-0.03 });

	if (slowItemNumber > 0)
		LifeDisplay.draw(std::string(slowItemNumber, 'S'));
}

void displayStageNumber() {
	static Text StageNumberDisplay({ -0.1, GameFrameUp + 0.02 });

	std::stringstream stagetext;
	stagetext << "Stage: ";
	stagetext << stage;

	StageNumberDisplay.draw(stagetext.str());
}

void displayGameInfo1() {
	static Text GameInfoDisplay({ -0.9, -0.8 - 0.07 });

	std::stringstream gameinfotext;
	gameinfotext << "If you push spacebar, harpoon will be shooted. ";


	GameInfoDisplay.draw(gameinfotext.str());
}

void displayGameInfo2() {
	static Text GameInfoDisplay({ -0.9, -0.8 - 0.1 });

	std::stringstream gameinfotext;
	gameinfotext << "If you push keyboard button 's', balls will move slowly for 3 seconds.";


	GameInfoDisplay.draw(gameinfotext.str());
}


void displayGameInfo3() {
	static Text GameInfoDisplay({ -0.9, -0.8 - 0.13 });

	std::stringstream gameinfotext;
	gameinfotext << "Get rid of all the balloons and go to the next stage. The balloons will be much faster.";

	GameInfoDisplay.draw(gameinfotext.str());
}

int frameCounter;
bool isSlow = false;
static double framedelta_stage(int stageno, bool isSlow = false) {
	const double framedelta = 1.0 + 0.5 * (double)(stageno - 1);
	if (isSlow)
		return framedelta / 2.0;
	else
		return framedelta;
}

void makeBallSlow() {
	if (isSlow == false) {
		frameCounter = getFrameCount();
		isSlow = true;
		slowItemNumber -= 1;
	}
}

static void Pang_Init() {

	light.setAmbient(0.5, 0.5, 0.5, 1.0);
	light.setDiffuse(0.7, 0.7, 0.7, 1.0);
	light.setSpecular(1.0, 1.0, 1.0, 1.0);

	OuterFrame = new OuterFrameBlock(GameFrameLeft, GameFrameRight, GameFrameUp, GameFrameDown);
	blocks.push_back(OuterFrame);
	blocks.push_back(new Block(0.4, 0.5, 0.1, -0.1));

	// 게임 소리 나게 하는 코드. 
	PlaySound(TEXT("bgm.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
}

void Pang_Mode_Standby();

static void Pang_InitStage() {
	static std::random_device fd;
	static std::mt19937 randgenerator(fd());
	static std::uniform_real_distribution<double> ballposnoise(-Init_BallPosition_Noise, Init_BallPosition_Noise);

	balls.clear();
	for(int k=0;k<stage+1;++k)
		balls.push_back(Ball(ballposnoise(randgenerator), ballposnoise(randgenerator), BallMaxSize, (bool)(k % 2)));

	P.setCoord(Init_PlayerPosition_x, Init_PlayerPosition_y);
	P.setHarpoon(-1.0);
	P.setLife(5);
	slowItemNumber = 4;

	Pang_Mode_Standby();
}

static void Pang_InitGame() {
	stage = 1;
	score = 0;
	Pang_InitStage();
}

static void Pang_Exit() {
	for (Block* PB : blocks)
		delete PB;
	blocks.clear();
}

static void Pang_IdleAction() {

	// 속도를 원상복귀시킴.
	if (isSlow) {
		if (frameCounter + 180 < getFrameCount()) {
			isSlow = false;
		}
	}

	while (!IsLastFrame()) {
		ProceedFrame();

		PIO.submit();
		P.nextframe(framedelta_stage(stage));

		for (Ball& B : balls) {
			B.nextframe(framedelta_stage(stage, isSlow));
		}

		//harpoon collision
		for (auto it = balls.begin(); it != balls.end(); ++it) {
			if (P.checkHarpooncollision(*it)) {
				score += (int)(1.0 / it->getradius());

				const double BcoordX = it->getcoordX();
				const double BcoordY = it->getcoordY();
				const double newradius = it->getradius() / 2;

				balls.erase(it);

				// 작살을 3번 맞으면 공이 사라짐. 2 ** 3 = 8
				if (newradius >= BallMinSize) {
					balls.push_back(Ball(BcoordX - newradius, BcoordY, newradius, false));
					balls.push_back(Ball(BcoordX + newradius, BcoordY, newradius, true));
				}

				P.useHarpoon();


				break;
			}
		}

		for (const Ball& B : balls)
			P.checkcollision(B);

		for (const Block* B : blocks) {
			if (P.checkHarpooncollision(*B)) {
				P.useHarpoon();
			}
		}

		if (balls.size() == 0) {
			stage += 1;
			Pang_InitStage();
			break;
		}

		if (P.getLife() <= 0) {
			ScoreBoard.insert(score);
			Pang_InitGame();
			break;
		}
	}

	glutPostRedisplay();
}

static void Pang_renderScene() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawSquareWithTexture();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	for (const Block* PB : blocks)
		PB->draw();

	P.draw();

	for (const Ball& B : balls)
		B.draw();

	glDisable(light.getID());
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	displayFrameCount();
	displayLife();
	displaySlowItem();
	displayStageNumber();
	displayGameInfo1();
	displayGameInfo2();
	displayGameInfo3();
	displayScore();

	glutSwapBuffers();

}

static void Pang_KeyboardAction(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		PIO.setkeySPACE();
		break;
	case 's':
		makeBallSlow();
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

static void Pang_Mode_Game() {
	glutDisplayFunc(Pang_renderScene);
	glutIdleFunc(Pang_IdleAction);
	glutKeyboardFunc(Pang_KeyboardAction);
	glutSpecialFunc(Pang_SpecialKeyboardAction);
}

static void PangStandby_renderScene() {

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawSquareWithTexture();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	OuterFrame->draw();
	P.draw();

	glDisable(light.getID());
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	displayFrameCount();
	displayLife();
	displaySlowItem();
	displayStageNumber();
	displayGameInfo1();
	displayGameInfo2();
	displayGameInfo3();
	displayScore();
	displayScoreBoard();

	static Text StandbyMessage({ -0.8, 0.0 });
	StandbyMessage.draw("Press 'P' to Continue");

	glutSwapBuffers();
}

static void PangStandby_IdleAction() {
	while (!IsLastFrame()) {
		ProceedFrame();
	}

	glutPostRedisplay();
}

static void PangStandby_KeyboardInput(unsigned char key, int x, int y) {
	DEBUG("key %c at (%d, %d)\n", key, x, y);
	switch (key) {
	case 'P':
	case 'p':
		Pang_Mode_Game();
		break;
	default:
		return;
	}

	glutPostRedisplay();
}

static void Pang_Mode_Standby() {
	glutDisplayFunc(PangStandby_renderScene);
	glutIdleFunc(PangStandby_IdleAction);
	glutKeyboardFunc(PangStandby_KeyboardInput);
	glutSpecialFunc(nullptr);
}

static int Pang_main(int * Pargc, char* argv[]) {
	glutInit(Pargc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowInitWidth, WindowInitHeight);
	glutCreateWindow("Simple Pang");

	Pang_Init();
	Pang_InitGame();

	Pang_Mode_Standby();

	glutMainLoop();

	return 0;
}

int main(int argc, char* argv[]) {
	return Pang_main(&argc, argv);
}