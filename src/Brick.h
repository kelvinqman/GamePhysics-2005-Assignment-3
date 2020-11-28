#pragma once
#ifndef __BRICK__
#define __BRICK__
#include "Sprite.h"
class Brick final : public Sprite
{
public:
	Brick();
	~Brick();
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
private:
};

#endif