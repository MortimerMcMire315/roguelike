/**
 *  HELPER.CPP
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


#include <helper.h>

IntPoint get_abs(IntPoint chunk, IntPoint coords) {
    return IntPoint(chunk.row * CHUNK_HEIGHT + coords.row,
            chunk.col * CHUNK_WIDTH + coords.col);
}

std::vector<std::string> all_files(const string & dir_string)
{
    path dir_path(dir_string);
    std::vector<std::string> files;
    if(exists(dir_path))
    {
        directory_iterator end_path;
        for(directory_iterator iter(dir_path); iter!=end_path; iter++)
        {
            files.push_back(iter->path().leaf().string());
        }
    return files;
    }
}

/* Returns all the files in a directory of a given type
*/
std::vector<std::string> all_files_of_type(const string & dir_string, const string & extension)
{
    path dir_path(dir_string);
    std::vector<std::string> files;
    if(exists(dir_path))
    {
        directory_iterator end_path;
        for(directory_iterator iter(dir_path); iter!=end_path; iter++)
        {
            if(iter->path().extension() == extension)
            {
                files.push_back(iter->path().leaf().stem().string());
            }
        }
    return files;
    }
}

std::string parse_settings(string &setting)
{
    unsigned int position = setting.find("=");
    if(position == setting.npos)
    {
        return "";
    }
    else
    {
        string value = setting.substr(position+1);
        setting = setting.substr(0, position);
        return value;
    }
}

int get_max_width(vector<string> string_list)
{
    int length = 0;
    for(int i=0; i<string_list.size(); i++)
    {
        if(string_list[i].size() > length)
        {
            length = string_list[i].size();
        }
    }
    return length;
}

std::vector<std::string> split_string(std::string input, std::string character)
{
    std::vector<std::string> strings;
    unsigned int position = input.find(character);
    while(position != input.npos)
    {
        strings.push_back(input.substr(0, position));
        input = input.substr(position+1);
        position = input.find(character);
    }
    strings.push_back(input);
    return strings;
}
