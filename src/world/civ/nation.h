/**
 *  @file NATION.H
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

#ifndef _NATION_H
#define _NATION_H


/**
 * The class that will hold nations.  This will be used to generate settlements.
 * Nations will be responsible for holding a set of cities, which will in turn be
 * associated with a series of houses.  NPCs will then be associated with houses.
 * A nation is associated with a specific tract of land, so NPCs which will
 * be generated in a nation will be associted with that nation (in a manner
 * of speaking).
 */
class Nation
{
    private:
        /**
         * The number of square kilometers occupied by the nation.
         * This represents how large the nation is in terms of geographical
         * area occupied.  It is one of several factors used to calculate
         * the population.
         */
        int size;

        /**
         * How many people the land can support.
         * This will essentially give a measure of population density.  It
         * is defined by the type of land that the nation occupies, and is
         * also used to calculate population.
         */
        int density;

        /**
         * The number of people in the nation.
         */
        int population;

        /**
         * A list of the cities within the nation.
         * This list should be subject to change as the nation grows/shrinks,
         * and as individual cities are destroyed or created.  This will not
         * be implemented for some time though.
         */
        std::vector<CityDistrict*> settlements;

    public:

        /**
         * The default constructor.  Does nothing.
         */
        Nation();

        /**
         * Constructs a nation by taking in population density and land mass.
         * Uses population density and land mass to calculate the total population.
         * This is then divided up in to cities, towns, villages, and isolated dwellings
         * based on numbers found in Medieval Cities by Norman Pounds, as well as John S.
         * Ross's Medieval Demographics made easy.  NPC's will then be generated for these
         * settlements, although it is likely that these will not be the only NPC's which
         * exist.  Only cities and towns will be added to the city list.
         * All others will be generated but exist independently of the list.
         *
         * Population = (rand(.8, 1.0)) * (density * size))
         * @param _density The average population density of the nation.
         * @param _land_mass The total land mass of the nation.
         */

         Nation(int _density, int _land_mass);

}
