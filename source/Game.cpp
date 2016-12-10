#include "Game.hpp"

ld::Game::Game() :
	xy::App(),
	m_stateStack({getRenderWindow(), *this})
{
	this->getRenderWindow().setTitle("Mother's Room");
	this->getRenderWindow().setSize(sf::Vector2u(1280, 720));
}

void ld::Game::handleEvent(const sf::Event & ev)
{
	m_stateStack.handleEvent(ev);
}

void ld::Game::handleMessage(const xy::Message & msg)
{
	switch (msg.id)
	{
	case xy::Message::Type::UIMessage:
	{
		auto& msgData = msg.getData<xy::Message::UIEvent>();
		switch (msgData.type)
		{
		case xy::Message::UIEvent::ResizedWindow:
			m_stateStack.updateView();
			break;
		default: break;
		}
		break;
	}
	default: break;
	}

	m_stateStack.handleMessage(msg);
}

void ld::Game::registerStates()
{
	m_stateStack.registerState<PlayState>(States::ID::Play);
	m_stateStack.pushState(States::ID::Play);

	// to-do
}

void ld::Game::updateApp(float dt)
{
	m_stateStack.update(dt);
}

void ld::Game::draw()
{
	m_stateStack.draw();
}

void ld::Game::initialise()
{
	registerStates();
	// register main menu here

	//getRenderWindow().setKeyRepeatEnabled(false);

	if (!xy::Input::load())
		xy::Input::save();
}

void ld::Game::finalise()
{
	m_stateStack.clearStates();
	m_stateStack.applyPendingChanges();
}
