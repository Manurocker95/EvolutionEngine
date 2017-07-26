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

#include "Module_Input.h"
#include "EvolutionScene.hpp"
#include "SceneManager.h"
namespace EvolutionEngine
{
	Input_Module::Input_Module_Factory Input_Module::factory;

	bool  Input_Module::Input_task::initialize()
	{
		return true;
	}

	bool Input_Module::Input_task::finalize()
	{
		// ...
		return true;
	}

	

	bool Input_Module::Input_task::do_step(float time)
	{
		if (!input_module->Get_Owner()->getEvolwindow())
		{
			return false;
		}

		 Event event;

		 while (input_module->Get_Owner()->getEvolwindow()->pollEvent(event))
		 {
			 if (event.type == Event::Closed)
			 {
				 Message close_window("close");
				 SceneManager::instance()->setRunning(false);
				 break;
			 }

			 if (event.type == Event::KeyPressed)
			 {
				 Message key_pressed("key_pressed");
				 key_pressed.add_parameter("key_code", event.key.code);
				 input_module->getMessageHandler()->multicast(key_pressed);
				 break;
			 }

			 if (event.type == Event::KeyReleased)
			 {
				 Message key_released("key_released");
				 key_released.add_parameter("key_code", event.key.code);
				 input_module->getMessageHandler()->multicast(key_released);
				 break;
			 }

			 if (event.type == Event::Resized)
			 {
				 Message key_released("key_resized");
				 SceneManager::instance()->reset_viewport(*input_module->Get_Owner()->getEvolwindow(), *input_module->Get_Owner()->getScene());
			 }
		 }

		return true;
	}

	Input_Module::Input_task::Input_task(int priority, Input_Module * _module) : input_module(_module), Task(priority)
	{

	}

	Input_Module::Input_Component::Input_Component(std::shared_ptr<Entity> parent) : Component(parent)
	{
		type = "input";
	}


	bool Input_Module::Input_Component::initialize()
	{
		return true;
	}
}