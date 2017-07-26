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
#include "DebugManager.h"

////////////////////////////

namespace EvolutionEngine
{
	class Entity;
	class Control_Module : public Module
	{
	public:

		class Control_Module_Factory : public Factory
		{

		private:

			std::shared_ptr < Module > control_module;

		public:
			Control_Module_Factory()
			{
				Module::register_module("control", this);
			}

			shared_ptr< Module > create_module() override
			{
				return shared_ptr< Module >();
			}

			shared_ptr< Module > create_module(EvolutionScene * owner) override
			{
				control_module = shared_ptr< Module >(new Control_Module(0, owner));
				return control_module;
			}
		};

		class Control_Message_Handler : public Message_Handler
		{
			EvolutionScene * owner;

			struct Control_Listener : Message_Handler::Listener
			{

				EvolutionScene * owner;

				Control_Listener() {}
				Control_Listener(EvolutionScene * _owner) { owner = _owner; }

				void handle_message(const Message & message) override;
			};

		public:

			Control_Listener * control_listener;

		public:
			Control_Message_Handler()
			{
				control_listener = new Control_Listener;
				//control_listener->handle_message(Message("key_pressed"));
			}

			Control_Message_Handler(EvolutionScene * _owner);

			void Set_Listener_To(Message_Handler * msg_hndlr)
			{
				msg_hndlr->register_listener("key_pressed", *control_listener);
				msg_hndlr->register_listener("key_released", *control_listener);
			}
		};

		class Control_task : public Task
		{
			Control_Module * control_module;

		public:
			Control_task(Control_Module * module) : control_module(module)
			{

			}

			Control_task(int priority, Control_Module * module);

			bool initialize() override;

			bool finalize() override;

			bool do_step(float time) override;
		};

		class Control_Component : public Component
		{
		public:

			EvolutionScene * owner;

		public:

			Control_Component()
			{
				type = "control";
			}

			Control_Component(EvolutionScene * _owner)
			{
				type = "control";
				owner = _owner;
			}

			Control_Component(std::shared_ptr<Entity> parent);

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

		static Control_Module_Factory factory;		

	private:

		Control_task * task;
		Control_Message_Handler * message_handler;

	public:

		Control_Module(const int & priority_task, EvolutionScene * owner)
			: Module(owner)
		{
			task = new Control_task(priority_task, this);
			message_handler = new Control_Message_Handler(owner);
		}

		Message_Handler * getMessageHandler()
		{
			return message_handler;
		}

		Task * get_task() override
		{
			return task;
		}

		void set_task(Control_task * _task)
		{
			task = _task;
		}
	};
}