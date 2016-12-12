#pragma once
#include <xygine/components/Component.hpp>

namespace ld
{
	enum class RecognizeID
	{
		Player = 0,
		Closet,
		Wall,
		Enemy,
	};

	class Recognize final:
		public xy::Component
	{
	public:
		Recognize(xy::MessageBus&, RecognizeID type);
		~Recognize() = default;

		Component::Type type() const;
		RecognizeID getRecognize() const;

		void entityUpdate(xy::Entity&, float) override;
		
	private:
		RecognizeID m_type;
	};
}