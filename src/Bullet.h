#pragma once
#ifndef __BULLET__
#define __BULLET__
#include "Sprite.h"
class Bullet:public Sprite
{
public:
	Bullet();
	~Bullet();
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	void Reset();
	bool active = false;
private:
};

#endif