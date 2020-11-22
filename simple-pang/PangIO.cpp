
#include <time.h>
#include "Pang.h"
#include "PangIO.h"
#include "Player.h"

void PangIO::setkeySPACE() {
	keySPACE = true;
}

void PangIO::setkeyLEFT() {
	keyLEFT = true;
}

void PangIO::setkeyUP() {
	keyUP = true;
}

void PangIO::setkeyRIGHT() {
	keyRIGHT = true;
}

void PangIO::setkeyDOWN() {
	keyDOWN = true;
}

void PangIO::submit() {
	if (keySPACE) {
		keySPACE = false;

		player.launch();
	}
	if (keyLEFT) {
		keyLEFT = false;

		player.move(-PlayerSpeed, 0);
	}
	if (keyUP) {
		keyUP = false;

		player.launch();
	}
	if (keyRIGHT) {
		keyRIGHT = false;

		player.move(PlayerSpeed, 0);
	}
	if (keyDOWN) {
		keyDOWN = false;

		player.launch();
	}
}

static clock_t framecount = 0;

//reference: http://www.cplusplus.com/reference/ctime/clock/
static const clock_t start = clock();

bool IsLastFrame() {

	const clock_t now = clock();
	const clock_t untilnow = ((now - start) * fps / CLOCKS_PER_SEC);
	
	return (untilnow <= framecount);
}

void ProceedFrame() {
	framecount++;
	DEBUG("proceeding frame #% 8d\n", framecount);
}