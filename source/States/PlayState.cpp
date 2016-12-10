#include "PlayState.hpp"

void ld::PlayState::buildScene()
{
	m_textureResource.setFallbackColour(sf::Color::Magenta);

	if (!m_tilemap.load("data/maps/testMap.tmx"))
	{
		xy::Logger::log("Cannot load map, cannot open", xy::Logger::Type::Error);
	}
		
	auto entity = xy::Entity::create(m_messageBus);
	const auto& layers = m_tilemap.getLayers();

	for (const auto& l : layers)
	{
		auto drawable = m_tilemap.getDrawable(m_messageBus, *l, m_textureResource, m_shaderResource);
		if (drawable)
		{
			xy::Logger::log("created layer drawable, adding to scene...");
			entity->addComponent(drawable);
		}
	}

	m_scene.addEntity(entity, xy::Scene::Layer::BackFront);

	auto camera = xy::Component::create<xy::Camera>(m_messageBus, getContext().defaultView);
	auto body = xy::Component::create<xy::Physics::RigidBody>(m_messageBus, xy::Physics::BodyType::Dynamic);
	auto drawable = xy::Component::create<xy::SfDrawableComponent<sf::RectangleShape>>(m_messageBus);
	auto collShape = xy::Physics::CollisionRectangleShape(sf::Vector2f(32,32));

	drawable->getDrawable().setSize((sf::Vector2f)m_textureResource.get("data/textures/player.png").getSize());
	drawable->getDrawable().setTexture(&m_textureResource.get("data/textures/player.png"), true);
	// start facing (up)
	drawable->getDrawable().setTextureRect(sf::IntRect(0, 0, 32, 32));

	body->addCollisionShape(collShape);
	
	auto player = xy::Entity::create(m_messageBus);
	auto cameraPtr = player->addComponent(camera);
	player->addComponent(body);
	player->addComponent(drawable);
	player->setPosition(0, 0);

	m_scene.addEntity(player, xy::Scene::Layer::FrontFront);
	m_scene.setActiveCamera(cameraPtr);
}

ld::PlayState::PlayState(xy::StateStack & stack, Context context) :
	State(stack, context),
	m_messageBus(context.appInstance.getMessageBus()),
	m_uiContainer(m_messageBus),
	m_scene(m_messageBus),
	m_physWorld(m_messageBus)
{
	launchLoadingScreen();

	m_cursorSprite.setTexture(m_textureResource.get("data/textures/ui/cursor.png"));
	m_cursorSprite.setPosition(context.renderWindow.mapPixelToCoords(sf::Mouse::getPosition(context.renderWindow)));
	m_cursorSprite.setScale(0.1f, 0.1f);

	// buildGui();

	xy::App::setMouseCursorVisible(false);

	m_scene.setView(context.defaultView);
	m_shaderResource.preload(xy::Shader::Tilemap, xy::Shader::tmx::vertex, xy::Shader::tmx::fragment);
	// We dont have gravity in top-down 
	m_physWorld.setGravity(sf::Vector2f(0, 0));

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

	renderWindow.setView(m_scene.getView());
	
	renderWindow.draw(m_cursorSprite);
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
