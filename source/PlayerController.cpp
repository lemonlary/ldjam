#include "PlayerController.hpp"

ld::PlayerController::PlayerController(xy::MessageBus & messageBus) : xy::Component(messageBus, this)
{
	m_speed = 50.f;
	m_buttons = MovementButtons();
	m_body = nullptr;
}

xy::Component::Type ld::PlayerController::type() const
{
	return Component::Type::Script;
}

void ld::PlayerController::onStart(xy::Entity & et)
{
	m_body = et.getComponent<xy::Physics::RigidBody>();

	if (m_body == nullptr)
		xy::Logger::log("Cannot use PlayerController, no rigidbody", xy::Logger::Type::Error);
}

void ld::PlayerController::entityUpdate(xy::Entity & et, float dt)
{
	if (m_body == nullptr)
		return;

	sf::Vector2f move(0, 0);

	if (sf::Keyboard::isKeyPressed(m_buttons.up))
		move.y = -m_speed;
	if (sf::Keyboard::isKeyPressed(m_buttons.down))
		move.y = m_speed;
	if (sf::Keyboard::isKeyPressed(m_buttons.left))
		move.x = -m_speed;
	if (sf::Keyboard::isKeyPressed(m_buttons.right))
		move.x = m_speed;

	m_body->setLinearVelocity(move);
}

void ld::PlayerController::setButtons(MovementButtons buttons)
{
	m_buttons = buttons;
}

void ld::PlayerController::setMovementSpeed(float speed)
{
	m_speed = speed;
}
