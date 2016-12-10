#include "PlayState.hpp"

ld::PlayState::PlayState(xy::StateStack & stack, Context context) :
	State(stack, context),
	m_messageBus(context.appInstance.getMessageBus()),
	m_uiContainer(m_messageBus)
{
	m_cursorSprite.setTexture(m_textureResource.get("data/textures/ui/cursor.png"));
	m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
	m_cursorSprite.setScale(0.1f, 0.1f);

	// buildGui();

	xy::App::setMouseCursorVisible(false);
}

bool ld::PlayState::update(float dt)
{
	m_uiContainer.update(dt);

	return false;
}

void ld::PlayState::draw()
{
	auto& renderWindow = getContext().renderWindow;

	renderWindow.setView(renderWindow.getDefaultView());
	renderWindow.draw(m_cursorSprite);

	renderWindow.draw(m_uiContainer);
}

bool ld::PlayState::handleEvent(const sf::Event & ev)
{
	const auto& renderWindow = getContext().renderWindow;
	auto mousePos = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));

	m_uiContainer.handleEvent(ev, mousePos);
	m_cursorSprite.setPosition(mousePos);

	return false;
}

void ld::PlayState::handleMessage(const xy::Message &)
{
}

xy::StateID ld::PlayState::stateID() const
{
	return ld::States::ID::Play;
}
