/**
 *  @file COLOR_LOAD.CPP
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

#include <iostream>
#include <boost/filesystem.hpp>

#include "utility.h"
#include "color_load.h"
#include "conf_util.h"
#include "string.h"
#include "ini.h"

namespace fs=boost::filesystem;

namespace color_load {

    static int handle_ini_entry(void* colordefs, const char* section, const char* name, const char* value) {
        //Cast the void pointer to an unordered map
        std::unordered_map<string, int>* colormap =
                (std::unordered_map<string, int>*) colordefs;

        int colorval;
        try {
            colorval = stoi(value);
        } catch(...) {
            cout<<"Error loading "<<COLORINI<<"... could not convert string to"<<
                " integer on color key "<<name<<".";
            return -1;
        }
        (*colormap)[name]=colorval;
        return 0;
    }

    std::unordered_map<string, int> load_conf() {
        std::unordered_map<string, int> colormap;
        if (conf_util::conf_exists(COLORINI)) {
            fs::path colorini(COLORINI);

            if (ini_parse(colorini.string().c_str(), handle_ini_entry, &colormap) < 0) {
                exit(EXIT_FAILURE);
            }

        } else {
            exit(EXIT_FAILURE);
        }

        return colormap;
    }
}
