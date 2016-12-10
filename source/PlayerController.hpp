#pragma once

#include <xygine/components/Component.hpp>
#include <xygine/Entity.hpp>
#include <xygine/physics/RigidBody.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ld
{
	struct MovementButtons
	{
		sf::Keyboard::Key up;
		sf::Keyboard::Key down;
		sf::Keyboard::Key left;
		sf::Keyboard::Key right;

		MovementButtons(sf::Keyboard::Key up = sf::Keyboard::Key::W, sf::Keyboard::Key down = sf::Keyboard::Key::S, sf::Keyboard::Key left = sf::Keyboard::Key::A, sf::Keyboard::Key right = sf::Keyboard::Key::D)
		{
			this->up = up;
			this->down = down;
			this->left = left;
			this->right = right;
		}
	};


	class PlayerController final :
		public xy::Component
	{
	public:
		explicit PlayerController(xy::MessageBus&);
		~PlayerController() = default;

		Component::Type type() const;

		void onStart(xy::Entity&);
		void entityUpdate(xy::Entity&, float);

		void setButtons(MovementButtons buttons);
		void setMovementSpeed(float speed);

	private:
		float m_speed;
		MovementButtons m_buttons;

		xy::Physics::RigidBody* m_body;
	};
}