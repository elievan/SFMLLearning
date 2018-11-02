#include "State_Game.h"
#include "../../Shared Code/StateSystem/StateManager.h"
#include "../../Shared Code/WindowSystem/Window.h"
#include "../../Shared Code/GUI/GUI_Manager.h"
#include "../../Shared Code/ECS/Core/System_Manager.h"
#include "../../Shared Code/ECS/Systems/S_Collision.h"
#include "../../Shared Code/ECS/Systems/S_Movement.h"
#include "../../Shared Code/StateSystem/States/State_Loading.h"
#include "../../Shared Code/SoundSystem/SoundManager.h"

State_Game::State_Game(StateManager* l_stateManager)
	: BaseState(l_stateManager) {}

State_Game::~State_Game() {}

void State_Game::OnCreate() {
	auto context = m_stateMgr->GetContext();
	EventManager* evMgr = context->m_eventManager;

	evMgr->AddCallback("Key_Escape", &State_Game::MainMenu, this);
	evMgr->AddCallback("Player_MoveLeft", &State_Game::PlayerMove, this);
	evMgr->AddCallback("Player_MoveRight", &State_Game::PlayerMove, this);
	evMgr->AddCallback("Player_MoveUp", &State_Game::PlayerMove, this);
	evMgr->AddCallback("Player_MoveDown", &State_Game::PlayerMove, this);

	sf::Vector2u size = context->m_wind->GetWindowSize();
	m_view.setSize(static_cast<float>(size.x), static_cast<float>(size.y));
	m_view.setCenter(static_cast<float>(size.x) / 2, static_cast<float>(size.y) / 2);
	m_view.zoom(0.6f);

	m_stateMgr->GetContext()->m_soundManager->PlayMusic("TownTheme", 50.f, true);

	auto loading = m_stateMgr->GetState<State_Loading>(StateType::Loading);
	context->m_gameMap->AddFile(Utils::GetWorkingDirectory() + "Build/media/Maps/map1.map");
	loading->AddLoader(context->m_gameMap);
	loading->SetManualContinue(true);
	context->m_soundManager->PlayMusic("TownTheme", 50.f, true);
}

void State_Game::OnDestroy() {
	auto context = m_stateMgr->GetContext();
	EventManager* evMgr = context->m_eventManager;
	evMgr->RemoveCallback(StateType::Game, "Key_Escape");
	evMgr->RemoveCallback(StateType::Game, "Key_O");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveLeft");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveRight");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveUp");
	evMgr->RemoveCallback(StateType::Game, "Player_MoveDown");
	context->m_gameMap->PurgeMap();
	context->m_gameMap->GetTileSet()->Purge();
}

void State_Game::Update(const sf::Time& l_time) {
	auto context = m_stateMgr->GetContext();
	UpdateCamera();
	context->m_gameMap->Update(l_time.asSeconds());
	context->m_systemManager->Update(l_time.asSeconds());
}

void State_Game::UpdateCamera() {
	if (m_player == -1) { return; }
	SharedContext* context = m_stateMgr->GetContext();
	auto pos = m_stateMgr->GetContext()->m_entityManager->GetComponent<C_Position>(m_player, Component::Position);

	m_view.setCenter(pos->GetPosition());
	context->m_wind->GetRenderWindow()->setView(m_view);

	sf::FloatRect viewSpace = context->m_wind->GetViewSpace();
	if (viewSpace.left <= 0) {
		m_view.setCenter(viewSpace.width / 2, m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	} else if (viewSpace.left + viewSpace.width > (context->m_gameMap->GetMapSize().x) * Sheet::Tile_Size) {
		m_view.setCenter(((context->m_gameMap->GetMapSize().x) * Sheet::Tile_Size) - (viewSpace.width / 2), m_view.getCenter().y);
		context->m_wind->GetRenderWindow()->setView(m_view);
	}

	if (viewSpace.top <= 0) {
		m_view.setCenter(m_view.getCenter().x, viewSpace.height / 2);
		context->m_wind->GetRenderWindow()->setView(m_view);
	} else if (viewSpace.top + viewSpace.height > (context->m_gameMap->GetMapSize().y) * Sheet::Tile_Size) {
		m_view.setCenter(m_view.getCenter().x, ((context->m_gameMap->GetMapSize().y) * Sheet::Tile_Size) - (viewSpace.height / 2));
		context->m_wind->GetRenderWindow()->setView(m_view);
	}
}

void State_Game::Draw() {
	auto context = m_stateMgr->GetContext();
	for (unsigned int i = 0; i < Sheet::Num_Layers; ++i) {
		context->m_gameMap->Draw(i);
		m_stateMgr->GetContext()->m_systemManager->Draw(m_stateMgr->GetContext()->m_wind, i);
	}
}

void State_Game::MainMenu(EventDetails* l_details) {
	m_stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Game::Activate() {
	auto map = m_stateMgr->GetContext()->m_gameMap;

	m_player = map->GetPlayerId();
	map->Redraw();
}

void State_Game::Deactivate() {}

void State_Game::PlayerMove(EventDetails* l_details) {
	Message msg((MessageType)EntityMessage::Move);
	if (l_details->m_name == "Player_MoveLeft") { msg.m_int = static_cast<int>(Direction::Left);
	} else if (l_details->m_name == "Player_MoveRight") { msg.m_int = static_cast<int>(Direction::Right);
	} else if (l_details->m_name == "Player_MoveUp") { msg.m_int = static_cast<int>(Direction::Up);
	} else if (l_details->m_name == "Player_MoveDown") { msg.m_int = static_cast<int>(Direction::Down); }
	msg.m_receiver = m_player;
	m_stateMgr->GetContext()->m_systemManager->GetMessageHandler()->Dispatch(msg);
}