#pragma once
#include <xygine/App.hpp>
#include <xygine/KeyBinds.hpp>
#include "States/PlayState.hpp"

namespace ld
{
	class Game final : 
		public xy::App
	{
	public:
		Game();
		~Game() = default;
		Game(const Game&) = delete;
		Game& operator = (const Game&) = delete;

	private:
		xy::StateStack m_stateStack;

		void handleEvent(const sf::Event&) override;
		void handleMessage(const xy::Message&) override;

		void registerStates() override;
		void updateApp(float dt) override;
		void draw() override;

		void initialise() override;
		void finalise() override;
	};
}