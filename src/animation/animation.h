/**
 *  @file ANIMATION.H
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

#ifndef __ANIMATION_H
#define __ANIMATION_H

#include <vector>
#include <iostream>
#include "frame.h"
#include "actor.h"

/**
 * The class for performing animations.  Contains coordinates for the animation to be executed at, a frame rate, and a set of frames.
 */
class Animation
{
    private:
        /**
         * Contains the frames for animation.
         */
        std::vector<Frame> frames;

        /**
         * The current frame of the animation.
         */
        int current_frame;

        /**
         * The number of millisecondes per frame.
         */
        int ms_per_frame;

        /**
         * The number of milliseconds which have accumulated since the
         * execution of the last frame.
         */
        long current_ms;

        /**
         * X coordinate of the center of the animation.
         */
        int x;

        /**
         * X coordinate of the center of the animation.
         */
        int y;

        /**
         * X coordinate of the center of the animation.
         */
        int chunk_x;

        /**
         * X coordinate of the center of the animation.
         */
        int chunk_y;

        /**
         * If the animation has completed execution or not.
         */
        bool done;

    public:
        /**
         * Constructor with prebuilt frames.
         */
        Animation(std::vector<Frame> _frames, int frame_rate, int _x, int _y, int chunk_x, int chunk_y);

        /**
         * Animation with frames to be added.
         */
        Animation(int frame_rate, int num_frames, int _x, int _y, int chunk_x, int chunk_y);

        /**
         * Empty constructor.
         */
        Animation();

        /**
         * Steps the animation one frame forward, but only if enough
         * time (ms) has passed.
         */
        void step(long ms);

        /**
         * Gets the current frame.
         */
        Frame get_frame();

        /**
         * Adds a new frame at a selected location. It will replace the
         * frame at the current location.
         */
        void add_frame(Frame new_frame, int location);

        /**
         * Returns the current frame rate (in ms).
         */
        int frame_rate();

        /**
         * Adds a new frame to the end.
         */
        void new_frame(Frame new_frame);

        /**
         * Adds an actor to a frame at a given index.
         */
        void add_actor_to_frame(Actor actor, int frame);

        /**
         * Basic accessor functions.
         */
        bool get_done();
        int get_x();
        int get_y();
        int get_chunk_x();
        int get_chunk_y();
};

#endif
