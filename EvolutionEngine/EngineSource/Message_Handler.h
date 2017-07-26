/*
This file is part of Evolution Engine

Started 21 / 05 / 2017

Copyright (C) 2017 Manuel Rodríguez Matesanz
>    This program is free software: you can redistribute it and/or modify
>    it under the terms of the GNU General Public License as published by
>    the Free Software Foundation, either version 3 of the License, or
>    (at your option) any later version.
>
>    This program is distributed in the hope that it will be useful,
>    but WITHOUT ANY WARRANTY; without even the implied warranty of
>    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
>    GNU General Public License for more details.
>
>    You should have received a copy of the GNU General Public License
>    along with this program.  If not, see <http://www.gnu.org/licenses/>.
>    See LICENSE for information.

> email: manuelrodriguezmatesanz@gmail.com

*/

#pragma once
#include <string>
#include <map>
#include <vector>
#include <list>
#include "Message.hpp"

using namespace std;

namespace EvolutionEngine
{

	class Message_Handler
	{

	protected:

		struct Listener
		{
			virtual void handle_message(const Message & message) = 0;
		};

		map < string, list< Listener * > > listeners;

	public:

		void register_listener(const string & message_id, Listener & listener)
		{
			listeners[message_id].push_back(&listener);
		}

		void unregister_listener(const string & message_id, Listener & listener)
		{
			//isteners[message_id].pop_back(listener);
		}

		// Enviar a todos los listeners.
		void multicast(const Message & message)
		{
			if (listeners.count(message.id) > 0)
			{
				for (auto listener : listeners[message.id])
				{
					listener-> handle_message(message);
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////

		// Message key_pressed("key_pressed");

		// key_pressed.add_parameter("key_code", key_code);

		// message_handler.multicast(key_pressed);
	};
}