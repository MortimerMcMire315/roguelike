/**
 *  @file ANIMATION.CPP
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

#include "animation.h"

Animation::Animation(std::vector<Frame> _frames, int frame_rate, int _x, int _y, int _chunk_x, int _chunk_y)
{
    frames = _frames;
    current_frame = 0;
    current_ms = 0;
    ms_per_frame = frame_rate;
    x = _x;
    y = _y;
    chunk_x = _chunk_x;
    chunk_y = _chunk_y;
    done = false;
}

Animation::Animation(int frame_rate, int num_frames, int _x, int _y, int _chunk_x, int _chunk_y)
{
    frames = std::vector<Frame>();
    frames.resize(num_frames);
    current_frame = 0;
    current_ms = 0;
    ms_per_frame = frame_rate;
    x = _x;
    y = _y;
    chunk_x = _chunk_x;
    chunk_y = _chunk_y;
    done = false;
}

Animation::Animation()
{
}

void Animation::step(long ms)
{
    current_ms += ms;
    if(current_ms >= ms_per_frame)
    {
        current_ms -= ms_per_frame;
        current_frame += 1;
    }
    if(current_frame >= frames.size())
    {
        current_frame = 0;
        done = true;
    }
}

Frame Animation::get_frame()
{
    return frames[current_frame];
}

void Animation::new_frame(Frame new_frame)
{
    frames.push_back(new_frame);
}

void Animation::add_frame(Frame frame, int location)
{
    if(location < frames.size())
    {
        frames[location] = frame;
    }
}

void Animation::add_actor_to_frame(Actor actor, int frame)
{
    if(frame < frames.size())
    {
        frames[frame].add_actor(actor);
    }
}

int Animation::frame_rate()
{
    return ms_per_frame;
}

bool Animation::get_done()
{
    return done;
}

int Animation::get_x()
{
    return x;
}

int Animation::get_y()
{
    return y;
}

int Animation::get_chunk_x()
{
    return chunk_x;
}

int Animation::get_chunk_y()
{
    return chunk_y;
}
