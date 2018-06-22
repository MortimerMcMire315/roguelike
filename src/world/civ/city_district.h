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

#ifndef _CITY_DISTRICT_H
#define _CITY_DISTRICT_H

#include "int_point.h"
#include "binary_space.h"
#include "block.h"

/**
 The class responsible for generating all of the buildings inside
 * of a single chunk that is a part of a city.
 */

class CityDistrict
{
    private:

        /**
         * The chunk that the settlement's top left corner is in.  For now
         * the top left corner will just be at (0, 0) of the chunk.
         */
        IntPoint chunk;

        /**
         * The size of the city, in terms of number of tiles.  Will eventually
         * be determined by the number of buildings, which is in turn
         * determined by the number of people.  For now, I'll just assigned
         * it a size.
         */
        int height;

        int width;

        /**
         * The number of people currently living in the settlement.
         */
        int population;

        /**
         * The city blocks which are present.  These also contain
         * the houses.
         */
        std::vector<Block> blocks;

        /**
         * Whether or not the settlement has walls.
         */
        bool walled;

        /**
         * The age of the settlement.
         */
        int age;

        /**
         * The number of stages of developments the town has undergone.
         * This will manifest as different sections of walls if the city
         * has walls, or a different pattern of building if it does not.
         * This is primarily determined by the age of the city.
         */
        int developments;

        /**
         * The number of centerpoints that the settlement has.
         * Occasionally, two towns would run into each other, or one town
         * would "split."  This would create binary towns, and theoretically
         * tertiary towns could exist.  This ends up looking like a town
         * in the process of mitosis with two very clear centers.
         */
        int num_focals;

        /**
         * The point(s) from which the settlement was built.
         * This is primarily for settlements which were not planned.
         * Planned settlements will most likely conform to a linear,
         * parallel street plan which does not necessarily have a center.
         * Centers may be buildings or landmarks, like rivers or defensible
         * hilltops.
         */
        std::vector<IntPoint> centers;

        /**
         * The governing figure of the town.
         * This figure may answer to someone higher, but that's not
         * necessary to know for the purpose of generation.  This may
         * also be NULL, which indicates that the town is self-governing.
         * The ruling figure helps to dictate the organization of the
         * town as well as the presence of a larger structure like a
         * castle or palace.  The larger the settlement, the less likely
         * it is that it will be self governing.
         * \todo Implement this, as NPCs don't exist yet.
         */
        //NPC* ruler;


        /**
         * The percentage of occupants of the settlement involved in agriculture.
         * This will effect the layout of the settlement.  Larger settlements
         * generally mean less agriculture.  The smallest settlements (villages
         * and isolated settlements) will be almost exculsively agricultural.
         */
         int agriculture;

        /**
         * Whether or not the settlement is based in feudalism.
         * If the settlement is feudally based, then it is not truly
         * a town, village, or city.  Rather it has essentially become
         * a massive plantation where each tenant is given a plot of land
         * where a large percentage of the food is taken from them.  It
         * will look like a series of farm plots with incredibly
         * impoverished tenants.  Cities and towns were effectivley the
         * next step after feudalism, and as such the older the nation,
         * the fewer feudal settlements there will be.
         */
        bool feudal;

        /**
         * Whether or not the settlement was deliberately planned.
         * This will actually be the primary influence.  The older the
         * civilization, the more planned cities there will be.  These
         * will almost always have a ruler, and will typically have less
         * of a center. Planned towns will also have more parallel streets.
         * However, there will still be an element of chaos, more so the
         * older the town is and the more developments which have been added.
         */
        bool planned;

        /**
         * The potential materials available to make buildings.
         * This represents the materials which can be used to create buildings.
         * This is subject to change over time.  It is determined by the technical
         * ability of the city and nation, and determines how high buildings can
         * be built.  Also has an influence on the rate of fires in a town.
         */
        std::vector<Tile> building_mats;

        /**
         * The type of MapTile of this city.
         */
        MapTile type;

    public:
        /**
         * The constructor for settlements.  As with buildings, I'm going to end
         * up coding it in a series of refinements by getting a simple working version
         * and then adding complexity as I go.  In the first draft, most*
         * of the variables will not be used.
         */
        CityDistrict(int chunk_x, int chunk_y, int _height, int _width, MapTile _type);

        /**
         * Makes a settlement out of a binary space partition.
         */
        void settlement_from_bst(BSpaceTree& bst);

        /**
         * Creates city blocks from a binary space partition.
         */
        void blocks_from_bst(BSpaceTree& bst);

        std::vector<Block>& get_blocks();

};

#endif
