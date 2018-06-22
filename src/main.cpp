/**
 *  MAIN.CPP
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

#include <boost/filesystem.hpp>

#include "gui.h"

using namespace std;
namespace fs=boost::filesystem;

int main(int argc, char* args[]) {

    /*
    fs::path chunk_dir(CHUNK_DIR);
    for(fs::directory_iterator end, it(chunk_dir); it != end; it++) {
        remove(it->path());
    }
    */

    //cout<<"Testing boost with /etc/fstab file size: "<<fs::file_size("/etc/fstab")<<endl;

    GUI theApp;
    return theApp.OnExecute();
}
