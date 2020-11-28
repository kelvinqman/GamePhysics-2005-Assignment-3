#include "BouncingBall.h"
#include "TextureManager.h"
#include "Util.h"

BouncingBall::BouncingBall()
{
	TextureManager::Instance()->load("../Assets/textures/tile.png", "title");
	const auto size = TextureManager::Instance()->getTextureSize("title");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(100.0f, 100.0f);
	getRigidBody()->velocity = glm::vec2(-100, 50);
	getRigidBody()->isColliding = false;

	setType(BOUNCINGBALL);

	bouncedX = false;
	bouncedY = false;
}

BouncingBall::~BouncingBall()
= default;

void BouncingBall::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->draw("title", x, y, 0, 255, true);
}

void BouncingBall::update()
{
	move();
	checkBoundary();
	lostEnergy();
	slowDown();
}

void BouncingBall::clean()
{
}

void BouncingBall::move()
{
	float deltaTime = 1.0f / 60.0f;
	getRigidBody()->velocity = getRigidBody()->velocity + (getRigidBody()->acceleration * deltaTime);
	getTransform()->position = getTransform()->position + (getRigidBody()->velocity * deltaTime);
}

void BouncingBall::checkBoundary()
{
	if (getTransform()->position.x >= 800 - getWidth() / 2 || getTransform()->position.x <= 0 + getWidth() / 2) {
		getRigidBody()->velocity.x *= -1;
		getRigidBody()->acceleration.x *= -1;
		bouncedX = true;
	}
	if (getTransform()->position.y >= 600 - getHeight() / 2 || getTransform()->position.y <= 0 + getHeight() / 2) {
		getRigidBody()->velocity.y *= -1;
		getRigidBody()->acceleration.y *= -1;
		bouncedY = true;
	}
}

void BouncingBall::lostEnergy()
{
	if (bouncedX) {
		if (getRigidBody()->velocity.x < 0 && getTransform()->position.x < 800 - getWidth() / 2 ||
			getRigidBody()->velocity.x>0 && getTransform()->position.x > 0 + getWidth() / 2) {
			getRigidBody()->velocity.x *= 0.8;
			bouncedX = false;
		}
	}
	if (bouncedY) {
		if (getRigidBody()->velocity.y < 0 && getTransform()->position.y < 600 - getHeight() / 2 ||
			getRigidBody()->velocity.y>0 && getTransform()->position.y > 0 + getHeight() / 2) {
			getRigidBody()->velocity.y *= 0.8;
			bouncedY = false;
		}
	}
}

void BouncingBall::slowDown()
{
	float x = getRigidBody()->velocity.x;
	float y = getRigidBody()->velocity.y;
	glm::vec2 m_direction = glm::vec2(x, y);
	float dirMagnitude = Util::magnitude(m_direction);
	float ACCELERATION = 2;
	/*if (dirMagnitude > 0) {
		getRigidBody()->acceleration = Util::normalize(m_direction) * -ACCELERATION;
	}
	else */if (Util::magnitude(getRigidBody()->velocity) > ACCELERATION) {
		getRigidBody()->acceleration = Util::normalize(getRigidBody()->velocity) * -ACCELERATION;
	}
	else
		getRigidBody()->acceleration = -getRigidBody()->velocity;
}
