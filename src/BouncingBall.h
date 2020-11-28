#pragma once
#ifndef __BOUNCINGBALL__
#define __BOUNCINGBALL__
#include "Sprite.h"
class BouncingBall final:public Sprite
{
public:
	BouncingBall();
	~BouncingBall();
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void move();
private:
	void checkBoundary();
	bool bouncedX,bouncedY;
	void lostEnergy();
	void slowDown();
};

#endif