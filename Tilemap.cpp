#include "TileMap.hpp"
#include <xygine/Reports.hpp>
#include <xygine/Entity.hpp>

#include <xygine/App.hpp>
#include <xygine/Log.hpp>
#include <xygine/util/Vector.hpp>

#include <xygine/components/TileMapLayer.hpp>
#include <xygine/components/Camera.hpp>
#include <xygine/components/SfDrawableComponent.hpp>
#include <xygine/physics/RigidBody.hpp>
#include <xygine/physics/CollisionCircleShape.hpp>
#include <xygine/shaders/Tilemap.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>

namespace
{
	const sf::Keyboard::Key upKey = sf::Keyboard::W;
	const sf::Keyboard::Key downKey = sf::Keyboard::S;
	const sf::Keyboard::Key leftKey = sf::Keyboard::A;
	const sf::Keyboard::Key rightKey = sf::Keyboard::D;
	const sf::Keyboard::Key fireKey = sf::Keyboard::Space;

	const float joyDeadZone = 25.f;
	const float joyMaxAxis = 100.f;

	enum Input
	{
		Up = 0x1,
		Down = 0x2,
		Left = 0x4,
		Right = 0x8
	};
	const float moveSpeed = 1500.f;

	xy::Entity* ent = nullptr;
	sf::Uint8 input = 0;

	bool drawOverlay = false;
}

ld::Tilemap::Tilemap(xy::StateStack &statestack, Context context)
	: State(statestack, context),
	m_messageBus(context.appInstance.getMessageBus()),
	m_scene(m_messageBus),
	m_physWorld(m_messageBus)

{
	launchLoadingScreen();
	m_scene.setView(context.defaultView);
	m_shaderResource.preload(xy::Shader::Tilemap, xy::Shader::tmx::vertex, xy::Shader::tmx::fragment);
	m_physWorld.setGravity({ 0.f, 980.f });

	buildScene();
	quitLoadingScreen();
}
bool ld::Tilemap::update(float dt)
{
	m_scene.update(dt);
	return true;
}

void ld::Tilemap::draw()
{
	auto &win = getContext().renderWindow;

	win.draw(m_scene);
	win.setView(m_scene.getView());
	if (drawOverlay) 
		win.draw(m_physWorld);
}

bool ld::Tilemap::handleEvent(const sf::Event &ev)
{
	return true;
}

void ld::Tilemap::handleMessage(const xy::Message &msg)
{
	m_scene.handleMessage(msg);
}

void ld::Tilemap::buildScene()
{
	if (m_tilemap.load("data/maps/testMap.tmx"))
	{
		auto entity = xy::Entity::create(m_messageBus);
		const auto& layers = m_tilemap.getLayers();
		for (const auto& l : layers)
		{
			if (l->getType() == xy::tmx::Layer::Type::Object)
			{
				xy::Logger::log("found object layer - attempting to create physics components", xy::Logger::Type::Info);
				auto rb = m_tilemap.createRigidBody(m_messageBus, *l);
				entity->addComponent(rb);
			}
			else
			{
				auto drawable = m_tilemap.getDrawable(m_messageBus, *l, m_textureResource, m_shaderResource);
				if (drawable)
				{
					xy::Logger::log("created layer drawable, adding to scene...");
					entity->addComponent(drawable);
				}
			}
		}
		m_scene.addEntity(entity, xy::Scene::Layer::BackFront);

		static const float radius = 30.f;

		auto body = xy::Component::create<xy::Physics::RigidBody>(m_messageBus, xy::Physics::BodyType::Dynamic);
		auto cs = xy::Physics::CollisionCircleShape(radius);
		cs.setDensity(0.9f);
		cs.setRestitution(1.f);
		body->addCollisionShape(cs);

		auto drawable = xy::Component::create<xy::SfDrawableComponent<sf::CircleShape>>(m_messageBus);
		drawable->getDrawable().setRadius(radius);
		drawable->getDrawable().setOrigin({ radius, radius });
		drawable->getDrawable().setFillColor({ 255, 255, 255, 200 });
		drawable->getDrawable().setOutlineThickness(2.f);

		auto cam = xy::Component::create<xy::Camera>(m_messageBus, getContext().defaultView);
		cam->lockTransform(xy::Camera::TransformLock::Rotation, true);
		cam->lockBounds(m_tilemap.getBounds());

		entity = xy::Entity::create(m_messageBus);
		entity->setPosition(800.f, 400.f);
		entity->addComponent(body);
		entity->addComponent(drawable);
		auto camPtr = entity->addComponent(cam);

		ent = m_scene.addEntity(entity, xy::Scene::Layer::FrontFront);
		m_scene.setActiveCamera(camPtr);
	}
}