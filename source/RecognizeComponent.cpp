#include "RecognizeComponent.hpp"

ld::Recognize::Recognize(xy::MessageBus & m, RecognizeID type) : xy::Component(m, this)
{
	m_type = type;
}

xy::Component::Type ld::Recognize::type() const
{
	return Component::Type::Script;
}

ld::RecognizeID ld::Recognize::getRecognize() const
{
	return m_type;
}

void ld::Recognize::entityUpdate(xy::Entity &, float)
{
}
