/**
 *  @file SETTLEMENT.H
 *  @author Michael and Seth Yoder
 *
 *  @section LICENSE
 *
 *  This file is part of ROGUELIKETHING.
 *
 *  ROGUELIKETHING is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROGUELIKETHING is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROGUELIKETHING.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SETTLEMENT_H
#define _SETTLEMENT_H

#include <vector>

#include "int_point.h"

/**
 * The class that holds settlements. This can be as small as a
 * 1 chunk wide village, or as large as a half-continent spanning
 * city.  While it does not define the rules for building the
 * chunks (those are in chunk.cpp), it does define the rules
 * for how the settlement gets broken up into districts, e.g.
 * market, residential, nobility, etc.
 *

 * Big Cities = 12,000-100,000 people, rarely exceeding 50,000.
 * Cities = 8,000-12,000 people.
 * Towns = 1,000-8,000 people.
 * Villages = 20-1,000 people.
 * Isolated dwellings = >20 people.
 * These will be generated in order of size to fill the population of a nation.
 * The more rural settlements will make up the majority of the population
 * (villages and isolated settlements), with ~3% of the population existing
 * in urbanized locations.
 */
class Settlement
{
    private:
        /**
         * A list of the city_districts in the area.
         */
        std::vector<CityDistrict> districts;

        /**
         * Generates types of city districts from a 2D vector
         * of IntPoints.
         */
        void districts_from_intpoints(std::vector<IntPoint> points);

    public:
        /**
         * The basic constructor.  Creates a list of City Districts
         * from a list of IntPoints.
         */
         Settlement(std::vector<<IntPoint> points);

        /**
         * Accessor for the districts.
         */
        std::vector<CityDistrict>& get_districts();

        /**
         * Accessor for a specific district.
         */
        CityDistrict& district_at_loc(IntPoint coords);
}
#endif
