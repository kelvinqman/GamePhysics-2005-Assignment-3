#include "Brick.h"
#include "TextureManager.h"
Brick::Brick()
{
	TextureManager::Instance()->load("../Assets/textures/brick.png", "brick");
	const auto size = TextureManager::Instance()->getTextureSize("brick");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(200.0f, 200.0f);
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setType(BRICK);
}

Brick::~Brick()
= default;

void Brick::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("brick", x, y, 0, 255, true);
}

void Brick::update()
{
}

void Brick::clean()
{
}
