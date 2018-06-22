/**
 *  INT_POINT.CPP
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

#include "int_point.h"

IntPoint::IntPoint() {
}

IntPoint::IntPoint(int _row, int _col) {
    row = _row;
    col = _col;
}

void IntPoint::print() const {
    cout<<"("<<row<<", "<<col<<")";
}

ostream& operator<<(ostream &out, const IntPoint &I) {
    I.print();
    return out;
}

bool IntPoint::operator==(const IntPoint& i)
{
    return (row == i.row && col == i.col);
}

bool IntPoint::operator!=(const IntPoint& i)
{
    return !(*this == i);
}

IntPoint IntPoint::operator+(const int &i)
{
    return IntPoint(row + i, col + i);
}

IntPoint IntPoint::operator+(const IntPoint& i)
{
    return IntPoint(row + i.row, col + i.col);
}

IntPoint IntPoint::operator-(const IntPoint& i)
{
    return IntPoint(row - i.row, col - i.col);
}

IntPoint IntPoint::operator-(const int &i)
{
    return IntPoint(row - i, col - i);
}
