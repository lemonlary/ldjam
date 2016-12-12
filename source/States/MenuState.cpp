#include "MenuState.hpp"
#include <xygine/App.hpp>
#include <xygine/ui/Button.hpp>
#include <xygine/util/Random.hpp>
#include <SFML/Window/Mouse.hpp>

ld::MenuState::MenuState(xy::StateStack& stack, Context context)
	: State(stack, context),
	m_messageBus(context.appInstance.getMessageBus()),
	m_uiContainer(m_messageBus)
{
	m_cursorSprite.setTexture(m_textureResource.get("data/textures/ui/cursor.png"));
	m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));

	m_logoSprite.setTexture(m_textureResource.get("data/textures/ui/logo.png"));
	m_logoSprite.setPosition(context.renderWindow.getSize().y / 2.0f - m_logoSprite.getTexture()->getSize().x / 2.0f, 250.0f);

	buildMenu();

	auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
	msg->type = xy::Message::UIEvent::MenuOpened;
	msg->value = 0.f;
	msg->stateID = States::ID::Menu;

	xy::App::setMouseCursorVisible(false);
}

//public
bool ld::MenuState::update(float dt)
{
	m_uiContainer.update(dt);
	return false;
}

void ld::MenuState::draw()
{
	auto& rw = getContext().renderWindow;
	rw.setView(getContext().defaultView);

	rw.draw(m_uiContainer);
	rw.draw(m_logoSprite);
	rw.draw(m_cursorSprite);
}

bool ld::MenuState::handleEvent(const sf::Event& evt)
{
	const auto& rw = getContext().renderWindow;
	auto mousePos = rw.mapPixelToCoords(sf::Mouse::getPosition(rw));

	m_uiContainer.handleEvent(evt, mousePos);
	m_cursorSprite.setPosition(mousePos);

	return false;
}

void ld::MenuState::handleMessage(const xy::Message& msg)
{

}

void ld::MenuState::buildMenu()
{
	auto button = std::make_shared<xy::UI::Button>(m_font, m_textureResource.get("data/textures/ui/btnPlay.png"));
	button->setPosition(960.f, 175.f);
	button->addCallback([this]()
	{
		close();
		requestStackPush(States::ID::Play);
	});
	m_uiContainer.addControl(button);

	button = std::make_shared<xy::UI::Button>(m_font, m_textureResource.get("data/textures/ui/btnOpt.png"));
	button->setPosition(960.f, 375.f);
	button->addCallback([this]()
	{
		close();
		//requestStackPush(States::ID::Play);
	});
	m_uiContainer.addControl(button);

	button = std::make_shared<xy::UI::Button>(m_font, m_textureResource.get("data/textures/ui/btnCredits.png"));
	button->setPosition(960.f, 575.f);
	button->addCallback([this]()
	{
		close();
		//requestStackPush(States::ID::Play);
	});
	m_uiContainer.addControl(button);

	button = std::make_shared<xy::UI::Button>(m_font, m_textureResource.get("data/textures/ui/btnLeave.png"));
	button->setPosition(960.f, 775.f);
	button->addCallback([this]()
	{
		close();
		requestStackPush(States::ID::EndGame);
	});
	m_uiContainer.addControl(button);
}

void ld::MenuState::close()
{
	requestStackClear();

	auto msg = m_messageBus.post<xy::Message::UIEvent>(xy::Message::UIMessage);
	msg->type = xy::Message::UIEvent::MenuClosed;
	msg->value = 0.f;
	msg->stateID = States::ID::Menu;
}