/**
 *  UTILITY.H
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


#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <boost/filesystem.hpp>
#include <string.h>
#include <sstream>

#include "int_point.h"
#include "constants.h"


namespace fs=boost::filesystem;

namespace utility {

    /**
     * Converts chunk coordinates to absolute coordinates.
     * Takes a chunka and the coordinates within the chunk and
     * turns it into an absolute coordinate (the coordinates
     * that would be used if there wasn't a chunk system, e.g.
     * chunk(2, 2) coordinates(10, 10) would become absolute(110, 210).
     * This uses the formula ((chunk_height * chunk_y) + y), ((chunk_width *
     * chunk_x) + x).
     * @param chunk The chunk to be multiplied.
     * @param coords The coordinates in the chunk.
     * @return The absolute coordinates.
     */
    IntPoint get_abs(IntPoint, IntPoint);

    /**
     * A function to get all the files in a given directory as strings.
     * @param dir_string The directory to search.
     * @preturn A vector of strings representing all of the flie paths in that directory.
     */
    std::vector<std::string> all_files(const string & dir_string);

    /**
     * Finds all of the files in a directory matching an extension.
     * @param dir_string The directory to search.
     * @param extension The extension to match (no .)
     * @return A vector of strings representing all of the files matching the extension.
     */
    std::vector<std::string> all_files_of_type(const string & dir_string, const string & extension);

    /**
     * Determines whether a file of the given file path exists on the
     * filesystem.
     * @param file_path the fully-qualified or locally-qualified file path.
     */
    bool file_exists(const char* file_path);

    /**
     * Determines whether a file of the given file path exists on the
     * filesystem.
     * @param file_path the fully-qualified or locally-qualified file path.
     */
    bool file_exists(const string& file_path);


    /**
     * Determines whether a file of the given file path exists on the
     * filesystem.
     * @param file_path the fully-qualified or locally-qualified file path.
     */
    bool file_exists(const fs::path& file_path);

    /**
     * Parses settings in the settings.conf.
     * Takes in a string (usually from settings.conf), and places the name of the setting
     * in the original variable passed.  It returns the value of the settings.
     * Settings are in the form of setting=value
     * @param setting The string to be parsed.
     * @return Empty string if no = is found.  Otherwise a string value of the setting.
     */
    std::string parse_settings(string &setting);

    /**
     * Finds the longest string in a vector of strings.
     * @param string_list List of strings to be parsed.
     * @return The length of the longest string in the list.
     */
    int get_max_width(std::vector<std::string> string_list);

    /**
     * Splits a string on the character passed in.
     * @param input The string passed in.
     * @param character The character to split on.
     * @return A vector of strings as the result.
     **/
    std::vector<std::string> split_string(std::string input, char character);

    /**
     * Converts a list of strings to a list of ints.
     * Takes in a list of strings, converts each one to an int,
     * and returns the list of ints. If it's not possible, the int is 0.
     * @param strings The list of strings to convert
     * @return A list of ints.
     */
    std::vector<int> strings_to_ints(std::vector<std::string> strings);

    /**
     * Checks to see if one point (using the chunk, coords convention) is
     * within range of another point, given a certain radius.
     */
    bool in_range(IntPoint chunk, IntPoint coords, IntPoint range_chunk, IntPoint center, IntPoint radius);

    /**
     * Takes in a set of coordinates and checks if they're out of
     * bounds of the chunk boundaries, rolls them over so that they're
     * within the bounds.
     * e.g. height of 51 gets converted to height of 1.
     */
    IntPoint normalize_coords(IntPoint coords);


    /**
     * Takes in a set of coordinates and returns whether or not the
     * chunk should change in reponse.
     * e.g. Height of 51 corresponds to chunk of (1, 0) (move 1 in the
     * positive y direction.
     */
    IntPoint normalize_chunk(IntPoint chunk);

    /**
     * Takes two vectors and iterates over them, combining the elements
     * in alternating order to form a single string.  For instance:
     * vec1 = {"Hello, ", "are you?"};
     * vec2 = {"how "};
     * concatenate vector would return "Hello, how are you?"
     */
    std::string concatenate_vectors(std::vector<std::string> first, std::vector<std::string> second);


    /**
     * Takes in a vector of some type of object and an
     * item, returns true if the item is in the vector.
     */
    template <typename T>
    bool is_in(std::vector<T> vec, T item)
    {
        for(int i=0;i<vec.size();i++)
        {
            if(item == vec[i])
            {
                return true;
            }
        }
        return false;
    }

}
#endif
