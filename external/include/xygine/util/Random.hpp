/*********************************************************************
Matt Marchant 2014 - 2016
http://trederia.blogspot.com

xygine - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#ifndef XY_UTIL_RANDOM_HPP_
#define XY_UTIL_RANDOM_HPP_

#include <xygine/Assert.hpp>

#include <SFML/Graphics/Rect.hpp>

#include <random>
#include <ctime>

namespace xy
{
    namespace Util
    {
        /*!
        \brief Pseudo random number generation functions
        */
        namespace Random
        {
            static std::mt19937 rndEngine(static_cast<unsigned long>(std::time(nullptr)));

            /*!
            \brief Returns a pseudo random floating point value
            \param begin Minimum value
            \param end Maximum value
            */
            static inline float value(float begin, float end)
            {
                XY_ASSERT(begin < end, "first value is not less than last value");
                std::uniform_real_distribution<float> dist(begin, end);
                return dist(rndEngine);
            }
            /*!
            \brief Returns a pseudo random integer value
            \param begin Minimum value
            \param end Maximum value
            */
            static inline int value(int begin, int end)
            {
                XY_ASSERT(begin < end, "first value is not less than last value");
                std::uniform_int_distribution<int> dist(begin, end);
                return dist(rndEngine);
            }
            /*!
            \brief Returns a poission disc sampled distribution of points within a given area
            \param area sf::FloatRect within which the points are distributed
            \param minDist minimum distance between points
            \param maxPoints maximum number of points to try generating
            */
            XY_EXPORT_API std::vector<sf::Vector2f> poissonDiscDistribution(const sf::FloatRect& area, float minDist, std::size_t maxPoints);
        }
    }
}

#endif //XY_UTIL_RANDOM_HPP_