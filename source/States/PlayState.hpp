#pragma once

#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>
#include <xygine/Resource.hpp>
#include <xygine/App.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>

#include "StateTypes.hpp"

namespace ld
{
	class PlayState final:
		public xy::State
	{
	public:
		PlayState(xy::StateStack &, Context);
		~PlayState() = default;

		bool update(float) override;
		void draw() override;
		bool handleEvent(const sf::Event&) override;
		void handleMessage(const xy::Message&) override;
		xy::StateID stateID() const override;

	private:
		xy::MessageBus& m_messageBus;
		xy::UI::Container m_uiContainer;
		sf::Sprite m_cursorSprite;
		xy::TextureResource m_textureResource;
		xy::FontResource m_fontResource;

	};
}