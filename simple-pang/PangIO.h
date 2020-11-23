#ifndef _FRAMECOUNT_H_
#define _FRAMECOUNT_H_

#include "Player.h"

class PangIO {
private:
	Player& player;

	bool keySPACE;
	bool keyLEFT;
	bool keyUP;
	bool keyRIGHT;
	bool keyDOWN;

public:
	PangIO(Player& player) : player(player), keySPACE(false), keyLEFT(false), keyUP(false), keyRIGHT(false), keyDOWN(false) { }

	void setkeySPACE();
	void setkeyLEFT();
	void setkeyUP();
	void setkeyRIGHT();
	void setkeyDOWN();

	void submit();
};

bool IsLastFrame();
void ProceedFrame();
long getFrameCount();

#endif /* _FRAMECOUNT_H_ */