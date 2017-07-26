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

#include "Module.hpp"
#include "Task.hpp"
#include "Component.hpp"
#include <memory>
#include "Message_Handler.h"

namespace EvolutionEngine
{
	class Entity;
	class Input_Module : public Module
	{
	public:

		class Input_Module_Factory : public Factory
		{

		private:

			std::shared_ptr < Module > input_module;

		public:
			Input_Module_Factory()
			{
				Module::register_module("input", this);
			}

			shared_ptr< Module > create_module() override
			{
				return shared_ptr< Module >();
			}

			shared_ptr< Module > create_module(EvolutionScene * owner) override
			{
				input_module = shared_ptr< Module >(new Input_Module(0, owner));
				return input_module;
			}
		};

		class Input_Message_Handler : public Message_Handler
		{
		public:
			Input_Message_Handler()
			{

			}

		};

		class Input_task : public Task
		{
			Input_Module * input_module;

		public:
			Input_task(Input_Module * module) : input_module(module)
			{

			}

			Input_task(int priority, Input_Module * module);

			bool initialize() override;

			bool finalize() override;

			bool do_step(float time) override;
		};

		class Input_Component : public Component
		{
		public:

			EvolutionScene * owner;

		public:

			Input_Component()
			{
				type = "input";
			}

			Input_Component(EvolutionScene * _owner)
			{
				type = "input";
				owner = _owner;
			}

			Input_Component(std::shared_ptr<Entity> parent);

			bool initialize() override;

			bool parse_property(const std::string & name, const std::string & value) override
			{
				properties[name] = value;
				return true;
			}

			void do_step()
			{

			}
		};

	public:

		static Input_Module_Factory factory;
	private:

		Input_task * task;
		Input_Message_Handler * message_handler;

	public:

		Message_Handler * getMessageHandler() override
		{
			return message_handler;
		}

		Input_Module(const int & priority_task, EvolutionScene * owner)
			: Module(owner)
		{
			task = new Input_task(priority_task, this);
			message_handler = new Input_Message_Handler;
		}

		Task * get_task() override
		{
			return task;
		}

		void set_task(Input_task * _task)
		{
			task = _task;
		}
	};
}