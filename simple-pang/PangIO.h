#ifndef _FRAMECOUNT_H_
#define _FRAMECOUNT_H_

class PangIO {
private:
	bool keySPACE;
	bool keyLEFT;
	bool keyUP;
	bool keyRIGHT;
	bool keyDOWN;

public:
	PangIO() : keySPACE(false), keyLEFT(false), keyUP(false), keyRIGHT(false), keyDOWN(false) {}

	void setkeySPACE();
	void setkeyLEFT();
	void setkeyUP();
	void setkeyRIGHT();
	void setkeyDOWN();

	void submit();
};

bool IsLastFrame();
void ProceedFrame();

#endif /* _FRAMECOUNT_H_ */