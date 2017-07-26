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

#include "Module_Control.h"
#include "EvolutionScene.hpp"
#include "Message.hpp"

namespace EvolutionEngine
{
	Control_Module::Control_Module_Factory Control_Module::factory;

	bool  Control_Module::Control_task::initialize()
	{
		return true;
	}

	bool Control_Module::Control_task::finalize()
	{
		// ...
		return true;
	}

	bool Control_Module::Control_task::do_step(float time)
	{

		return true;
	}

	Control_Module::Control_task::Control_task(int priority, Control_Module * _module) : control_module(_module), Task(priority)
	{

	}

	Control_Module::Control_Component::Control_Component(std::shared_ptr<Entity> parent) : Component(parent)
	{
		type = "control";
	}



	Control_Module::Control_Message_Handler::Control_Message_Handler(EvolutionScene * _owner)
	{
		owner = _owner;
		control_listener = new Control_Listener(_owner);
		if (owner->get_modules().count("input") > 0)
		{
			Set_Listener_To(owner->get_modules()["input"].get()->getMessageHandler());
			//owner->get_modules()["input"].get()->getMessageHandler()->register_listener("key_pressed", *control_listener);
		}
	}

	void Control_Module::Control_Message_Handler::Control_Listener::handle_message(const Message & message)
	{
		if (message.id.compare("key_pressed") == 0)
		{
			if (message.key == sf::Keyboard::Right)
			{
				if (owner->get_player())
					owner->get_player()->move(true, -1, 0);
			}

			if (message.key == sf::Keyboard::Left)
			{
				if (owner->get_player())
					owner->get_player()->move(true, 1, 0);
			}

			if (message.key == sf::Keyboard::Up)
			{
				if (owner->get_player())
					owner->get_player()->move(true, 0, 1);
			}

			if (message.key == sf::Keyboard::Down)
			{
				if (owner->get_player())
					owner->get_player()->move(true, 0, -1);
			}

			if (message.key == sf::Keyboard::Space)
			{
				if (owner->get_player())
					owner->get_player()->start_jump();
			}
		}

		if (message.id.compare("key_released") == 0)
		{
			if (message.key == sf::Keyboard::Right)
			{
				if (owner->get_player())
					owner->get_player()->move(false, 0);
			}

			if (message.key == sf::Keyboard::Left)
			{
				if (owner->get_player())
					owner->get_player()->move(false, 0);
			}

			if (message.key == sf::Keyboard::Down)
			{
				if (owner->get_player())
					owner->get_player()->move(true, 0, 0);
			}

			if (message.key == sf::Keyboard::Up)
			{
				if (owner->get_player())
					owner->get_player()->move(true, 0, 0);
			}
		}
	}

	bool Control_Module::Control_Component::initialize()
	{
		return true;
	}
}