#ifndef DFA12_H
#define DFA12_H

#include "Charm.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
using namespace std;


class Dfa12
{
public:
	PairingGroup group;
	Dfa12() { group.setCurve(AES_SECURITY); };
	~Dfa12() {};
	DFA dfaUtil;

	void setup(CharmListStr & alphabet, CharmList & mpk, G1 & msk);
	void keygen(CharmList & mpk, G1 & msk, CharmListInt & Q, CharmMetaListInt & T, CharmListInt & F, CharmList & sk);
	void encrypt(CharmList & mpk, CharmListStr & w, GT & M, CharmList & ct);
	void decrypt(CharmList & sk, CharmList & ct, GT & M);
};


#endif