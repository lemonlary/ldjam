#pragma once
#include <xygine/State.hpp>

namespace ld
{
	namespace States
	{
		enum ID : xy::StateID
		{
			Menu = 0,
			Play, 
			Pause,
			EndGame
		};
	}
}