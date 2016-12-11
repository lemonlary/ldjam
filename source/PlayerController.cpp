#include "PlayerController.hpp"

using xy::Util::Const::PI;

ld::PlayerController::PlayerController(xy::MessageBus & messageBus) : xy::Component(messageBus, this)
{
	m_speed = 5000.f;
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
	{
		move.y = -m_speed;
	}
	if (sf::Keyboard::isKeyPressed(m_buttons.down))
	{
		move.y = m_speed;
	}
	if (sf::Keyboard::isKeyPressed(m_buttons.left))
	{
		move.x = -m_speed;
	}
	if (sf::Keyboard::isKeyPressed(m_buttons.right))
	{
		move.x = m_speed;
	}

	m_body->setLinearVelocity(move * dt);
	static float rotation = 0.0f;
	if (move.x == 0 && move.y == 0)
	{
		et.setRotation(rotation);
		
		if (et.getComponent<xy::AnimatedDrawable>()->playing())
		{
			et.getComponent<xy::AnimatedDrawable>()->play();
			et.getComponent<xy::AnimatedDrawable>()->setPaused(true);
		}
	
		return;
	}
	else
		et.getComponent<xy::AnimatedDrawable>()->setPaused(false);

	// Rotation
	sf::Vector2f position = et.getPosition();

	float x = fabsf(static_cast<float>(move.x + position.x) - position.x);
	float y = fabsf(static_cast<float>(move.y + position.y) - position.y);
	float z = sqrtf(x * x + y * y);

	float angle = 0.0f;
	if (move.y + position.y < position.y)
		y = -y;
	if ((move.x + position.x >= position.x) && (move.y + position.y >= position.y))
		angle += asinf(y / z) * 180.0f / PI;
	if ((move.x + position.x < position.x) && (move.y + position.y >= position.y))
		angle += 180.0f - asinf(y / z) * 180.0f / PI;
	if ((move.x + position.x < position.x) && (move.y + position.y < position.y))
		angle += 270.0f - (PI / 2.0f + asinf(y / z)) * 180.0f / PI;
	if ((move.x + position.x >= position.x) && (move.y + position.y < position.y))
		angle += (asinf(y / z) + 2.0f * PI) * 180.0f / PI;

	et.setRotation(angle);
	rotation = angle;
}

void ld::PlayerController::setButtons(MovementButtons buttons)
{
	m_buttons = buttons;
}

void ld::PlayerController::setMovementSpeed(float speed)
{
	m_speed = speed;
}
