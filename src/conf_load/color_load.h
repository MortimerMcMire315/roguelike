/**
 *  @file COLOR_LOAD.H
 *  @author Michael Yoder, Seth Yoder
 *
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

#ifndef COLOR_LOAD_H
#define COLOR_LOAD_H

#include <unordered_map>

namespace color_load {
    /**
     * A callback function for the color.ini file parser.
     * @param colordefs A pointer to the unordered_map that holds the parsed content
     * @param section The name of the [section] in the .ini that is being parsed
     * @param name The key that is currently being parsed
     * @param value The value of the key that is being parsed
     * @return 0 for success, -1 for failure.
     */
    static int handle_ini_entry(void* colordefs, const char* section,
                                const char* name, const char* value);

    /**
     * Load the color.ini file using the inih library.
     * @return an unordered map matching color names to SDL integers.
     */
    std::unordered_map<string, int> load_conf();
}

#endif
