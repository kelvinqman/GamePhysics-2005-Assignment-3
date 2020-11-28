#pragma once
#ifndef __SCENE2__
#define __SCENE2__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "BouncingBall.h"
#include "Brick.h"
class Scene2 : public Scene
{
public:
	Scene2();
	~Scene2();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;

	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	//Player* m_pPlayer;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;

	BouncingBall* m_pBall;
	Brick* m_pBrick;
};

#endif /* defined (__PLAY_SCENE__) */