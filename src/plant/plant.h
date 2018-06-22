/**
 *  @file PLANT.H
 *  @author Michael Yoder
 *
 *  @section LICENSE
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

#ifndef _PLANT_H
#define _PLANT_H

#include "defs.h"
#include "item.h"
#include "int_point.h"

/**
 * The class which is used for plants, so that plants can grow and
 * whatnot.
 */

typedef std::vector<std::vector<Tile> > TileMatrix;

class Plant
{
    private:
        /**
         * X coordinate of the plant.
         */
        int x;

        /**
         * Y coordinate of the plant.
         */
        int y;

        /**
         * The x coordinate of the chunk of the plant.
         */
        int chunk_x;

        /**
         * The y coordinate of the chunk of the plant.
         */
        int chunk_y;

        /**
         * The name of the plant.
         */
        std::string name;

        /**
        * The character representing the plant.
         * TODO: Make this a 2d vector so that plants
         * can be as biiiiig as we want them to be.
         */
        TileMatrix sprites;

        /**
         * The category of tools/equipment that can harvest the plant.
         */
        int harvest_tool;

        /**
         * The possible items that the plant can drop.
         */
        std::vector<Misc*> drops;

    public:

        /**
         * Basic constructor.
         */
        //Plant(int _x, int _y, Tile _sprite);

        /**
         * Constructor taking in a plant type.
         */
        Plant(int _x, int _y, int _chunk_x, int _chunk_y, PlantType plant);

        /**
         * Empty constructor.
         */
        Plant();

        /**
         * Destructor to delete the drops.
         */
        ~Plant();

        /**
         * Copy constructor for the plant.
         */
        Plant(const Plant& p);

       /**
        * Assignment operator for the plant.
        */
        Plant& operator=(const Plant& p);

        /**
         * Public accessor for the x coordinate.
         */
        int get_x();

        /**
         * Public accessor for the y coordinate.
         */
        int get_y();

        /**
         * Public accessor for the x coordinate of the chunk.
         */
        int get_chunk_x();

        /**
         * Public accessor for the y coordinate of the chunk.
         */
        int get_chunk_y();

        /**
         * Public accessor for the chunk coordinates.
         */
        IntPoint get_chunk();

        /**
         *  Public accessor for the sprite.
         */
        Tile* get_sprite(int y, int x);

        /**
         * Public accessor for tilematrix.
         */
        TileMatrix* get_sprites();

        /**
         * Returns the coordinates as an IntPoint.
         */
        IntPoint get_coords();

        /**
         * Pass in a tilematrix to use as the sprites for this plant.
         */
        void set_sprites(TileMatrix tm);

        int get_harvest_tool();

        /**
         * Returns the drops without removing them.
         */
        std::vector<Misc*> get_drops();

        /**
         * Returns  the drops and removes them from
         * the plant.
         */
        std::vector<Misc*> pop_drops();

        /**
         * Public accessor for the name.
         */
        std::string get_name();

        void set_x(int _x);
        void set_y(int _y);

        /**
         * Checks if the coords overlap with the plants coords.
         */
        bool in_plant(IntPoint coords);

};

#endif
