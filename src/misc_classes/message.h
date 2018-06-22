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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <stdlib.h>
#include <vector>

/**
 * A singleton instance of a board that writes messages.
 */
class MessageBoard
{
    public:

        /**
         * Returns the single instance of the message board, or
         * creates one if it doesn't exist.
         */
        static MessageBoard& instance();

        /**
         * Adds a message to the message queue at the appropriate place.
         * Takes in an id, and will only add the message if the id is
         * from the main_character (-1).
         */
        void add_message(std::string message, int id);

        /**
         * Returns the number of messages currently in the message board.
         */
        int num_messages();

        /**
         * Gets the most recent message.
         */
        std::string get_current_message();

        /**
         * Gets a message at the given index.
         * Indexing starts at 0 for the most recent
         * message (most recent is 0, next previous is
         * 1, etc.)
         */
        std::string get_message(int index);

    private:
        /**
         * A vector containing the messages that have occcurred
         * to this point.
         */
        std::vector<std::string> messages;

        /**
         * The maximum number of messages in the message queue.
         */
        int max_messages;

        /**
         * The index of the most recent message.
         */
        int message_index;

        /**
         * Constructs a message board.  Private so that it can't
         * be called outside of the messageboard class.
         */
        MessageBoard();

        /**
         * The copy constructor, made private to guarantee that
         * it can't be called.
         */
        MessageBoard(MessageBoard const&);


        /**
         * The assignment operator, made private so that it can
         * never be called.
         */
        MessageBoard& operator=(MessageBoard const&);

        /**
         * The destructor, private so it can't be called.
         */
        //~MessageBoard();
};

#endif
