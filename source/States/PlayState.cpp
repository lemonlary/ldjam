#include "PlayState.hpp"

namespace
{
	void triggerAction(xy::Component * component, const xy::Message & msg)
	{
		auto& msgData = msg.getData<xy::Message::PhysicsEvent>();


		if (msgData.event == xy::Message::PhysicsEvent::BeginContact)
		{
			if (msgData.contact->getCollisionShapeA()->getRigidBody()->getName() == "ClosetSensor")
			{
				xy::Logger::log("<Here should be cutscene or something>");
			}
		}
	}

}

void ld::PlayState::buildScene()
{
	// World setup
	{
		m_scene.setView(this->getContext().defaultView);
		m_shaderResource.preload(xy::Shader::Tilemap, xy::Shader::tmx::vertex, xy::Shader::tmx::fragment);
		// We dont have gravity in top-down 
		m_physWorld.setGravity(sf::Vector2f(0, 0));
	}

	sf::Vector2f playerPositon(0, 0);

	// Map setup
	// To-Do, loading closet position etc
	{
		m_textureResource.setFallbackColour(sf::Color::Magenta);

		if (!m_tilemap.load("data/maps/Room1.tmx"))
		{
			xy::Logger::log("Cannot load map, cannot open", xy::Logger::Type::Error);
		}

		auto entity = xy::Entity::create(m_messageBus);
		auto closet = xy::Entity::create(m_messageBus);
		const auto& layers = m_tilemap.getLayers();

		for (const auto& l : layers)
		{
			if (l->getName() == "PlayerSpawn" && l->getType() == xy::tmx::Layer::Type::Object)
			{
				xy::Logger::log("found PlayerSpawn object, seting position", xy::Logger::Type::Info);
				const auto* objectLayer = dynamic_cast<const xy::tmx::ObjectGroup*>(&*l);
				playerPositon = objectLayer->getObjects()[0].getPosition();
				
				continue;
			}

			if (l->getName() == "Closet" && l->getType() == xy::tmx::Layer::Type::Object)
			{
				xy::Logger::log("found Closet object, setting position", xy::Logger::Type::Info);
				
				auto rb = m_tilemap.createRigidBody(m_messageBus, *l);
				rb->setName("Closet");

				closet->addComponent(rb);
				closet->addComponent(xy::Component::create<Recognize>(m_messageBus, RecognizeID::Closet));

				continue;
			}

			if (l->getName() == "ClosetSensor" && l->getType() == xy::tmx::Layer::Type::Object)
			{
				xy::Logger::log("found Closet SENSOR object, setting position", xy::Logger::Type::Info);
				
				auto sensor = m_tilemap.createRigidBody(m_messageBus, *l);
				sensor->setName("ClosetSensor");
				for (auto& cs : sensor->getCollisionShapes())
					cs->setIsSensor(true);

				closet->addComponent(sensor);
				
				continue;
			}


			if (l->getName() == "WallCollider" && l->getType() == xy::tmx::Layer::Type::Object)
			{
				xy::Logger::log("found WallCollider, updating colliders", xy::Logger::Type::Info);
				auto rb = m_tilemap.createRigidBody(m_messageBus, *l);
				rb->setName("WallCollider");
				entity->addComponent(rb);

				continue;
			}

			auto drawable = m_tilemap.getDrawable(m_messageBus, *l, m_textureResource, m_shaderResource);
			if (drawable)
			{
				xy::Logger::log("created layer drawable, adding to scene...");
				entity->addComponent(drawable);
			}
		}

		m_scene.addEntity(entity, xy::Scene::Layer::BackFront);
		m_scene.addEntity(closet, xy::Scene::Layer::BackFront);
	}
	
	// Player setup
	{
		auto camera = xy::Component::create<xy::Camera>(m_messageBus, getContext().defaultView);
		auto body = xy::Component::create<xy::Physics::RigidBody>(m_messageBus, xy::Physics::BodyType::Dynamic);
		auto collShape = xy::Physics::CollisionRectangleShape(sf::Vector2f(32, 32), sf::Vector2f(-16,-16));
		auto animation = xy::Component::create<xy::AnimatedDrawable>(m_messageBus, m_textureResource.get("data/textures/game/player.png"));
		xy::Component::MessageHandler triggerHandler;



		animation->setFrameSize(sf::Vector2i(32, 32));
		animation->setOrigin(16, 16);
		animation->loadAnimationData("data/textures/game/playerAnimation.xya");
		animation->play(sf::Int16(0), sf::Int16(-1), sf::Int16(0));
		animation->setLooped(true);

		triggerHandler.action = std::bind(triggerAction, std::placeholders::_1, std::placeholders::_2);
		triggerHandler.id = xy::Message::PhysicsMessage;
		animation->addMessageHandler(triggerHandler);

		body->addCollisionShape(collShape);
		body->setGravityScale(0);
		body->setAngularDamping(0);
		body->setName("Player");

		camera->setZoom(3.f);
		camera->lockTransform(xy::Camera::TransformLock::Rotation, true);


		auto player = xy::Entity::create(m_messageBus);
		auto cameraPtr = player->addComponent(camera);
		

		
		player->setWorldPosition(playerPositon);
		player->addComponent(body);
		player->addComponent(animation);
		player->addComponent(xy::Component::create<PlayerController>(m_messageBus));
		player->addComponent(xy::Component::create<Recognize>(m_messageBus, RecognizeID::Player));

		m_scene.addEntity(player, xy::Scene::Layer::FrontFront);
		m_scene.setActiveCamera(cameraPtr);
	}
}


ld::PlayState::PlayState(xy::StateStack & stack, Context context) :
	State(stack, context),
	m_messageBus(context.appInstance.getMessageBus()),
	m_uiContainer(m_messageBus),
	m_scene(m_messageBus),
	m_physWorld(m_messageBus)
{
	launchLoadingScreen();

	// Move to buildGUI()
	m_cursorSprite.setTexture(m_textureResource.get("data/textures/ui/cursor.png"));
	m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
	m_cursorSprite.setScale(0.8f, 0.8f);
	xy::App::setMouseCursorVisible(false);

	// buildGui();
	
	buildScene();

	quitLoadingScreen();
}

bool ld::PlayState::update(float dt)
{
	m_uiContainer.update(dt);
	m_scene.update(dt);

	return false;
}

void ld::PlayState::draw()
{
	auto& renderWindow = getContext().renderWindow;
	renderWindow.draw(m_scene);
	renderWindow.draw(m_uiContainer);

	auto mousePos = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
	m_cursorSprite.setPosition(mousePos);
	renderWindow.draw(m_cursorSprite);
	renderWindow.draw(m_physWorld);
}

bool ld::PlayState::handleEvent(const sf::Event & ev)
{
	const auto& renderWindow = getContext().renderWindow;
	auto mousePos = renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow));
	m_cursorSprite.setPosition(mousePos);

	m_uiContainer.handleEvent(ev, mousePos);

	return false;
}

void ld::PlayState::handleMessage(const xy::Message & msg)
{
	m_scene.handleMessage(msg);
}

xy::StateID ld::PlayState::stateID() const
{
	return ld::States::ID::Play;
}

void ld::PlayState::updateLoadingScreen(float dt, sf::RenderWindow& rw)
{
	static sf::Sprite s (m_textureResource.get("data/textures/ui/loadingScreen.png"));
	s.setScale(2, 1.5f);
	rw.draw(s);
}
