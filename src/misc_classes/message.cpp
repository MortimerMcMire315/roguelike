/**
 *  MESSAGE.H
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

#include "message.h"

MessageBoard& MessageBoard::instance()
{
    static MessageBoard mb_instance;
    return mb_instance;
}

MessageBoard::MessageBoard()
{
    max_messages = 200;
    message_index = 0;
    messages.push_back("Testing.");
}

void MessageBoard::add_message(std::string message, int id)
{
    //only add messages that pertain to the main character (id -1)
    if(id == -1)
    {
        message_index += 1;
        if(message_index >= max_messages)
        {
            message_index = 0;
            messages[message_index] = message;
        } else {
            messages.push_back(message);
        }
    }
}

int MessageBoard::num_messages() {
    return messages.size();
}

std::string MessageBoard::get_current_message()
{
    return messages[message_index];
}

std::string MessageBoard::get_message(int index)
{
    if(index >= 0 && index < max_messages)
    {
        int mess_num = message_index - index;
        if(mess_num < 0) {
            mess_num = num_messages() + mess_num;
        }
        return messages[mess_num];
    }
}

