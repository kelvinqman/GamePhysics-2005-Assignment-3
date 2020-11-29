#pragma once
#ifndef __BOUNCINGBALL__
#define __BOUNCINGBALL__
#include "Sprite.h"
#include "Brick.h"
class BouncingBall final:public Sprite
{
public:
	BouncingBall();
	~BouncingBall();
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	bool boundary();
	void setBouncedX(bool x) { bouncedX = x; }
	void setBouncedY(bool y) { bouncedY = y; }
	bool getBouncedX() { return bouncedX; }
	bool getBouncedY() { return bouncedY; }
	void bounceX();
	void bounceY();

private:
	void move();
	void checkBoundary();
	bool bouncedX,bouncedY;
	void lostEnergy();
	void slowDown();
};

#endif