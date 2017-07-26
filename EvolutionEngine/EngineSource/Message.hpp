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
#include "SFML/Window.hpp"

using namespace std;

namespace EvolutionEngine
{
	struct Message
	{

		string id;
		sf::Keyboard::Key key;

		map < string,  sf::Keyboard::Key > parameters;
		

		Message(const string & id) : id(id)
		{
		}

		void add_parameter(const string & name, sf::Keyboard::Key & value)
		{
			parameters[name] = value;
			key = value;
		}

		sf::Keyboard::Key get_parameter(const string & name)
		{
			return parameters[name];
		}

	};
 }
////////////////////////////////////////////////////////////////////////////////////////////////////

// Message key_pressed("key_pressed");

// key_pressed.add_parameter("key_code", key_code);

// message_handler.multicast(key_pressed);
