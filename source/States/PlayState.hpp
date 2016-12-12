#pragma once

#include <xygine/State.hpp>
#include <xygine/ui/Container.hpp>
#include <xygine/Resource.hpp>
#include <xygine/App.hpp>
#include <xygine/physics/World.hpp>
#include <xygine/tilemap/Map.hpp>
#include <xygine/tilemap/Object.hpp>
#include <xygine/tilemap/ObjectGroup.hpp>
#include <xygine/Scene.hpp>
#include <xygine/ShaderResource.hpp>

#include <xygine/shaders/Tilemap.hpp>
#include <xygine/components/SfDrawableComponent.hpp>
#include <xygine/physics/RigidBody.hpp>
#include <xygine/physics/CollisionRectangleShape.hpp>
#include <xygine/components/TileMapLayer.hpp>
#include <xygine/components/AnimatedDrawable.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "StateTypes.hpp"
#include "../PlayerController.hpp"
#include "../RecognizeComponent.hpp"

namespace ld
{
	class PlayState final:
		public xy::State
	{
	private:
		void buildScene();

	public:
		PlayState(xy::StateStack &, Context);
		~PlayState() = default;

		bool update(float) override;
		void draw() override;
		bool handleEvent(const sf::Event&) override;
		void handleMessage(const xy::Message&) override;
		xy::StateID stateID() const override;
		void updateLoadingScreen(float, sf::RenderWindow&) override;

	private:
		xy::MessageBus& m_messageBus;
		xy::UI::Container m_uiContainer;
		sf::Sprite m_cursorSprite;
		xy::TextureResource m_textureResource;
		xy::FontResource m_fontResource;

		xy::Scene m_scene;
		xy::Physics::World m_physWorld;
		xy::tmx::Map m_tilemap;
		xy::ShaderResource m_shaderResource;

	};
}