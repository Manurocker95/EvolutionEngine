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

namespace EvolutionEngine
{
	class Entity;
	class Script_Module : public Module
	{
	public:

		class Script_Module_Factory : public Factory
		{

		private:

			std::shared_ptr < Module > script_module;

		public:
			Script_Module_Factory()
			{
				Module::register_module("script", this);
			}

			shared_ptr< Module > create_module() override
			{
				return shared_ptr< Module >();
			}

			shared_ptr< Module > create_module(EvolutionScene * owner) override
			{
				script_module = shared_ptr< Module >(new Script_Module(0, owner));
				return script_module;
			}
		};


		class Script_task : public Task
		{
			Script_Module * script_module;

		public:
			Script_task(Script_Module * module) : script_module(module)
			{

			}

			Script_task(int priority, Script_Module * module);

			bool initialize() override;

			bool finalize() override;

			bool do_step(float time) override;
		};

		class Script_Component : public Component
		{
		public:

			EvolutionScene * owner;

		public:

			Script_Component()
			{
				type = "script";
			}

			Script_Component(EvolutionScene * _owner)
			{
				type = "script";
				owner = _owner;
			}

			Script_Component(std::shared_ptr<Entity> parent);

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

		static Script_Module_Factory factory;

	private:

		Script_task * task;

	public:

		Script_Module(const int & priority_task, EvolutionScene * owner)
			: Module(owner)
		{
			task = new Script_task(priority_task, this);
		}

		Task * get_task() override
		{
			return task;
		}

		void set_task(Script_task * _task)
		{
			task = _task;
		}
	};
}