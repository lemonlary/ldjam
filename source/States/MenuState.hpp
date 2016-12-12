#pragma once
#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>
#include <xygine/Resource.hpp>
#include <xygine/App.hpp>
#include <xygine/Scene.hpp>
#include <xygine/ShaderResource.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "StateTypes.hpp"

namespace xy
{
	class MessageBus;
}
namespace ld
{
	class MenuState final : public xy::State
	{
	public:
		MenuState(xy::StateStack&, Context);
		~MenuState() = default;

		bool update(float) override;
		void draw() override;
		bool handleEvent(const sf::Event&) override;
		void handleMessage(const xy::Message&) override;
		xy::StateID stateID() const override
		{
			return States::ID::Menu;
		}
	private:
		xy::MessageBus& m_messageBus;
		xy::UI::Container m_uiContainer;
		sf::Sprite m_cursorSprite;
		sf::Sprite m_logoSprite;
		xy::TextureResource m_textureResource;
		xy::FontResource m_fontResource;
		sf::Font m_font;

		void buildMenu();
		void close();
	};
}