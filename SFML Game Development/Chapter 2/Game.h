#pragma once
#include "../Shared Code/WindowSystem/Window.h"
#include "../Shared Code/EventSystem/EventManager.h"
#include "../Shared Code/StateSystem/StateManager.h"
#include "../Shared Code/Resources/TextureManager.h"
#include "../Shared Code/Utilities/RandomGenerator.h"
#include "../Shared Code/Resources/FontManager.h"
#include "../Shared Code/Resources/AudioManager.h"
#include "../Shared Code/SoundSystem/SoundManager.h"
#include "../Shared Code/MapSystem/Map.h"
#include "../Shared Code/ECS/Core/System_Manager.h"
#include "../Shared Code/ECS/Core/Entity_Manager.h"
#include "../Shared Code/GUI/GUI_Manager.h"
#include "../Shared Code/StateSystem/StateManager.h"

class Game{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
private:
	void SetUpClasses();
	void SetUpECS();
	void SetUpStates();
	void RestartClock();
	sf::Clock m_clock;
	sf::Time m_elapsed;
	SharedContext m_context;
	RandomGenerator m_rand;
	Window m_window;
	TextureManager m_textureManager;
	FontManager m_fontManager;
	AudioManager m_audioManager;
	SoundManager m_soundManager;
	GUI_Manager m_guiManager;
	SystemManager m_systemManager;
	EntityManager m_entityManager;
	Map m_gameMap;
	std::unique_ptr<StateManager> m_stateManager;
};