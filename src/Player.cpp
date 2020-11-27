#include "Player.h"
#include "TextureManager.h"
#include "SoundManager.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE_RIGHT)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/spritesheet.txt",
		"../Assets/sprites/spritesheet.png", 
		"littleman");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("littleman"));
	
	// set frame width
	setWidth(30);

	// set frame height
	setHeight(41);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_buildAnimations();

	SoundManager::Instance().load("../Assets/audio/Explode.wav", "explode", SoundType::SOUND_SFX);
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("littleman", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true);
		break;
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("littleman", getAnimation("idle"),
			x, y, 0.12f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	case PLAYER_RUN_RIGHT:
		TextureManager::Instance()->playAnimation("littleman", getAnimation("run"),
			x, y, 0.25f, 0, 255, true);
		break;
	case PLAYER_RUN_LEFT:
		TextureManager::Instance()->playAnimation("littleman", getAnimation("run"),
			x, y, 0.25f, 0, 255, true, SDL_FLIP_HORIZONTAL);
		break;
	default:
		break;
	}
	
}

void Player::update()
{
	
}

void Player::clean()
{
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::moveLeft()
{
	getTransform()->position.x -= 5;
}

void Player::moveRight()
{
	getTransform()->position.x += 5;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();

	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("idle0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("idle1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("idle2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("idle3"));

	setAnimation(idleAnimation);

	Animation runAnimation = Animation();

	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("run0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("run1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("run2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("run3"));

	setAnimation(runAnimation);
}
