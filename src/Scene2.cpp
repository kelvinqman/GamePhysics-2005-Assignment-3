#include "Scene2.h"
//#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

Scene2::Scene2()
{
	Scene2::start();
}

Scene2::~Scene2()
= default;

void Scene2::draw()
{
	if (EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

}

void Scene2::update()
{
	updateDisplayList();
	dragDrop();
	checkBrick();
	reset();
}

void Scene2::clean()
{
	removeAllChildren();
}

void Scene2::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}

	m_mousePosition= EventManager::Instance().getMousePosition();
	mouseButtonDown = EventManager::Instance().getMouseButton(0);
	if (mouseButtonDown && hoverOver()&&!draged) {
		timeStart = SDL_GetTicks();
		tempPos = m_pBrick->getTransform()->position;
		std::cout << timeStart<<"  " << std::endl;
		draged = true;
	}
}

void Scene2::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
		{
			m_pBackButton->setActive(false);
			TheGame::Instance()->changeSceneState(START_SCENE);
		});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pBackButton->setAlpha(128);
		});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pBackButton->setAlpha(255);
		});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
		{
			m_pNextButton->setActive(false);
			TheGame::Instance()->changeSceneState(END_SCENE);
		});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pNextButton->setAlpha(128);
		});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pNextButton->setAlpha(255);
		});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("2Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);

	m_pBall = new BouncingBall();
	addChild(m_pBall);

	m_pBrick = new Brick();
	addChild(m_pBrick);

	hoverOvered = false;

	bouncedX = false;
	bouncedY = false;

	hit = false;

	//timeStart = 0;
	tempPos = glm::vec2(0, 0);
	endPos= glm::vec2(0, 0);
}

void Scene2::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if (ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if (ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}

	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

bool Scene2::hoverOver()
{
	return m_mousePosition.x < (m_pBrick->getTransform()->position.x + m_pBrick->getWidth() / 2) && m_mousePosition.x >(m_pBrick->getTransform()->position.x - m_pBrick->getWidth() / 2) &&
		m_mousePosition.y < (m_pBrick->getTransform()->position.y + m_pBrick->getHeight() / 2) && m_mousePosition.y >(m_pBrick->getTransform()->position.y - m_pBrick->getHeight() / 2);
}

void Scene2::dragDrop()
{
	if (hoverOver())
		hoverOvered = true;
	if (hoverOvered && mouseButtonDown && !hit) {
		m_pBrick->getTransform()->position = glm::vec2(m_mousePosition.x, m_mousePosition.y);
		if (SDL_GetTicks() - timeStart >= timerDuration) {
			draged = false;
			std::cout << "updated" << std::endl;
		}
	}
	if (!mouseButtonDown && hoverOvered)
		hoverOvered = false;
	if (!mouseButtonDown && hit)
		hit = false;
}

void Scene2::checkBrick()
{
	SDL_Rect brickRect = { m_pBrick->getTransform()->position.x,m_pBrick->getTransform()->position.y,m_pBrick->getWidth(),m_pBrick->getHeight() };
	SDL_Rect ballRect = { m_pBall->getTransform()->position.x,m_pBall->getTransform()->position.y,m_pBall->getWidth(),m_pBall->getHeight() };

	if (!m_pBall->getRigidBody()->isColliding&& SDL_HasIntersection(&brickRect, &ballRect)) {
		std::cout << "hit" << std::endl;
		timeEnd = SDL_GetTicks();
		endPos = m_pBrick->getTransform()->position;
		if (mouseButtonDown/* && hoverOvered*/) {
			m_pBrick->getRigidBody()->velocity.x = (endPos.x - tempPos.x) / (timeEnd - timeStart) * 1000;
			m_pBrick->getRigidBody()->velocity.y = (endPos.y - tempPos.y) / (timeEnd - timeStart) * 1000;
		}
		else m_pBrick->getRigidBody()->velocity = glm::vec2(0, 0);
		std::cout << timeEnd - timeStart << "  " << m_pBrick->getRigidBody()->velocity.x << "  " << m_pBrick->getRigidBody()->velocity.y << std::endl;
		hit = true;
		m_pBall->getRigidBody()->isColliding = true;
		bool top = m_pBall->getTransform()->position.y < m_pBrick->getTransform()->position.y;
		bool bottom = m_pBall->getTransform()->position.y > m_pBrick->getTransform()->position.y;
		bool left = m_pBall->getTransform()->position.x < m_pBrick->getTransform()->position.x;
		bool right = m_pBall->getTransform()->position.x > m_pBrick->getTransform()->position.x;
		float x = abs(m_pBall->getTransform()->position.x - m_pBrick->getTransform()->position.x);
		float y = abs(m_pBall->getTransform()->position.y - m_pBrick->getTransform()->position.y);
		bool samedirectionX = m_pBrick->getRigidBody()->velocity.x / m_pBall->getRigidBody()->velocity.x>0;
		bool samedirectionY = m_pBrick->getRigidBody()->velocity.y / m_pBall->getRigidBody()->velocity.y > 0;
		if (!samedirectionX&&!samedirectionY) {
			if (top) {
				if (left) {
					if (x < y) m_pBall->bounceY();
					else m_pBall->bounceX();
				}
				if (right) {
					if (x < y) m_pBall->bounceY();
					else m_pBall->bounceX();
				}
			}
			if (bottom) {
				if (left) {
					if (x < y) m_pBall->bounceY();
					else m_pBall->bounceX();
				}
				if (right) {
					if (x < y) m_pBall->bounceY();
					else m_pBall->bounceX();
				}
			}
			if (m_pBall->getBouncedX()) {
				m_pBall->getRigidBody()->velocity.x += m_pBrick->getRigidBody()->velocity.x;
			}
			if (m_pBall->getBouncedY()) {
				m_pBall->getRigidBody()->velocity.y += m_pBrick->getRigidBody()->velocity.y;
			}
		}
	}
}

void Scene2::reset()
{
	SDL_Rect brickRect = { m_pBrick->getTransform()->position.x,m_pBrick->getTransform()->position.y,m_pBrick->getWidth(),m_pBrick->getHeight() };
	SDL_Rect ballRect = { m_pBall->getTransform()->position.x,m_pBall->getTransform()->position.y,m_pBall->getWidth(),m_pBall->getHeight() };

	if (m_pBall->getRigidBody()->isColliding && !SDL_HasIntersection(&brickRect, &ballRect))
		m_pBall->getRigidBody()->isColliding = false;
}

