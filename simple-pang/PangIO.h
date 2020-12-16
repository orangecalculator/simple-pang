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
	bool keyS;

public:
	PangIO(Player& player);

	void setkeySPACE();
	void setkeyLEFT();
	void setkeyUP();
	void setkeyRIGHT();
	void setkeyDOWN();
	void setkeyS();

	void submit();
	void clear();
};

bool IsLastFrame();
void ProceedFrame();
long getFrameCount();

#endif /* _FRAMECOUNT_H_ */