#pragma once
#include <xygine/State.hpp>
#include <xygine/Resource.hpp>
#include <xygine/ShaderResource.hpp>
#include <xygine/Scene.hpp>
#include <xygine/physics/World.hpp>
#include <xygine/tilemap/Map.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <array>
#include "States/StateTypes.hpp"

namespace ld
{
	class Tilemap final : public xy::State
	{
	public:
		Tilemap(xy::StateStack &statestack, Context context);
		~Tilemap() = default;

		bool update(float dt) override;
		void draw() override;
		bool handleEvent(const sf::Event& evt) override;
		void handleMessage(const xy::Message&) override;
	private:
		xy::MessageBus& m_messageBus;
		xy::Scene m_scene;
		xy::Physics::World m_physWorld;
		xy::TextureResource m_textureResource;
		xy::FontResource m_fontResource;
		xy::ShaderResource m_shaderResource;
		xy::tmx::Map m_tilemap;

		void buildScene();
	};
}