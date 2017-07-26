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
#include "Model.hpp"
#include "Cube.hpp"
#include "Material.hpp"

namespace EvolutionEngine
{
	class Render_Module : public Module
	{
	public:

		class Render_Module_Factory : public Factory
		{
		public:

			Render_Module_Factory()
			{
				Module::register_module("render", this);
			}

			shared_ptr< Module > create_module() override
			{
				return shared_ptr< Module >(new Render_Module(0, nullptr));
			}

			shared_ptr< Module > create_module(EvolutionScene * owner) override
			{
				return shared_ptr< Module >(new Render_Module(0, owner));
			}
		};

		class Render_task : public Task
		{
		public:
			Render_Module * render_module;
			
		public:
			Render_task(Render_Module * module) : render_module(module)
			{

			}

			Render_task(int priority, Render_Module * module) : render_module(module), Task(priority)
			{

			}

			bool initialize() override;

			bool finalize() override;

			bool do_step(float time) override;
		};

		class Render_Component : public Component
		{
		private:
			// Variables
		public:

			Render_Component(std::string object_type)
			{
				type = "render";
				objectType = object_type;
				model.reset(new toolkit::Model());
				model->add(shared_ptr< toolkit::Drawable >(new toolkit::Cube), toolkit::Material::default_material());
			}

			Render_Component(std::shared_ptr<Entity> parent) : Component (parent)
			{
				type = "render";
				model.reset(new toolkit::Model());
				model->add(shared_ptr< toolkit::Drawable >(new toolkit::Cube), toolkit::Material::default_material());

				//transformation_matrix = glm::mat4(1.0f);
			}

			~Render_Component() {}

		public:

			bool initialize() override
			{
				// creamos el objeto según nos toque

				if (objectType.compare("cube") == 0)
				{
					return true;
				}

				return false;
			}

			bool parse_property(const std::string & name, const std::string & value)override
			{
				properties[name] = value;
				return true;
			}

			void update(float delta_time)
			{

			}
		};

		static Render_Module_Factory factory;

		Render_task task;

	public:

		Render_Module(const int & priority_task, EvolutionScene * owner)
			: Module(owner)
			, task (priority_task, this)
		{
			
		}

		Render_Module() : task(0, this)
		{

		}

		Task * get_task() override
		{
			return &task;
		}
	};
}