/**
 *  @file PLANT.CPP
 *  @author Michael Yoder
 *
 *  @section LICENSE
 *  This file is part of ROGUELIKETHING.
 *`
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

#include "plant.h"
/**
Plant::Plant(int _x, int _y, Tile _sprite)
{
    x = _x;
    y = _y;
    sprite = _sprite;
}
*/
Plant::Plant(int _x, int _y, int _chunk_x, int _chunk_y, PlantType plant)
{
    x = _x;
    y = _y;
    chunk_x = _chunk_x;
    chunk_y = _chunk_y;
    sprites = plant.sprites;
    harvest_tool = plant.harvest_tool;
    name = plant.name;
    for(int i=0;i<plant.drops.size();i++)
    {
        drops.push_back(new Misc(IntPoint(y, x), plant.drops[i]));
    }
}

Plant::Plant()
{
}

Plant::Plant(const Plant& p)
{
    x = p.x;
    y = p.y;
    chunk_x = p.chunk_x;
    chunk_y = p.chunk_y;
    sprites = p.sprites;
    name = p.name;
    for(int i=0;i<p.drops.size();i++)
    {
        Misc* item = new Misc;
        *item = *p.drops[i];
        drops.push_back(item);
    }
}

Plant& Plant::operator=(const Plant& p)
{
    x = p.x;
    y = p.y;
    chunk_x = p.chunk_x;
    chunk_y = p.chunk_y;
    sprites = p.sprites;
    name = p.name;
    for(int i=0;i<drops.size();i++)
    {
        delete drops[i];
    }
    drops.resize(0);
    for(int i=0;i<p.drops.size();i++)
    {
        Misc* item = new Misc;
        *item = *p.drops[i];
        drops.push_back(item);
    }
}

Plant::~Plant()
{
    for(int i=0;i<drops.size();i++)
    {
        delete drops[i];
        drops.erase(drops.begin() + i);
    }
}

int Plant::get_x()
{
    return x;
}

int Plant::get_y()
{
    return y;
}

int Plant::get_chunk_x()
{
    return chunk_x;
}

int Plant::get_chunk_y()
{
    return chunk_y;
}

IntPoint Plant::get_chunk()
{
    return IntPoint(chunk_y, chunk_x);
}

Tile* Plant::get_sprite(int y, int x)
{
    return &sprites[y][x];
}

TileMatrix* Plant::get_sprites()
{
    return &sprites;
}

// \todo can i pass by reference???
void Plant::set_sprites(TileMatrix tm) {
    sprites = TileMatrix(tm.size(), std::vector<Tile>(tm[0].size()));
    for(int i = 0; i < tm.size(); i++) {
        for(int j = 0; j < tm[i].size(); j++) {
            sprites[i][j] = tm[i][j];
        }
    }
}

void Plant::set_x(int _x) {
    x = _x;
}

void Plant::set_y(int _y) {
    y = _y;
}

IntPoint Plant::get_coords()
{
    return IntPoint(y, x);
}

int Plant::get_harvest_tool()
{
    return harvest_tool;
}

std::string Plant::get_name()
{
    return name;
}

std::vector<Misc*> Plant::get_drops()
{
    return drops;
}

std::vector<Misc*> Plant::pop_drops()
{
    std::vector<Misc*> temp;
    for(int i=0;i<drops.size();i++)
    {
        temp.push_back(drops.back());
        drops.pop_back();
    }
    return temp;
}

bool Plant::in_plant(IntPoint coords)
{
    bool is_y = (coords.row >= y && coords.row < (y + sprites.size()));
    bool is_xy = is_y && (coords.col >= x && coords.col < (x + sprites[coords.row - y].size()));
    return is_xy;
}
