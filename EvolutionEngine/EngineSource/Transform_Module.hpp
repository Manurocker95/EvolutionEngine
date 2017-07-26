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

#ifndef TRANSFORM_MODULE_HPP
#define TRANSFORM_MODULE_HPP

#include <list>
#include "Module.hpp"
#include "Component.hpp"
#include "Task.hpp"

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr

namespace EvolutionEngine
{
	class EvolutionScene;
	class Transform_Module : public Module
	{

		////////////////////////////////////////
		////						
		////	TRANSFORM MODULE FACTORY	////
		////
		////////////////////////////////////////

	public:
		class Transform_Module_Factory : public Factory
		{

		public:
			Transform_Module_Factory()
			{
				Module::register_module("transform", this);
			}

		public:
			shared_ptr< Module > create_module() override
			{
				return nullptr;
			}

			shared_ptr< Module > create_module (EvolutionScene * owner) override
			{
				return shared_ptr< Module >(new Transform_Module(0, owner));
			}
		};

		//////////////////////////////////
		////						
		////	TRANSFORM COMPONENT	  ////
		////
		//////////////////////////////////

		class Transform_Component : public Component
		{
		private:
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			toolkit::Vector3 start_position;	///< lugar de inicio.

			glm::mat4 transformation_matrix;

		public:
			Transform_Component(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
				: position(pos)
				, rotation(rotation)
				, scale(scale)
				, transformation_matrix(glm::mat4(1.0f))
			{
				type = "transform";
				start_position[0] = pos[0];
				start_position[1] = pos[1];
				start_position[2] = pos[2];
			}

			Transform_Component(std::shared_ptr<Entity> parent)
				: Component (parent)
				, position(1.f)
				, rotation(1.f)
				, scale(1.f)
				, transformation_matrix(glm::mat4(1.0f))
			{
				type = "transform";
				//transformation_matrix = glm::mat4(1.0f);
			}

			toolkit::Vector3 getOriginalPosition() override {return start_position ;}
			glm::vec3 getPosition() override { return position; }
			glm::vec3 getRotation() override { return rotation; }
			glm::vec3 getScale() override { return scale; }

			~Transform_Component() {}

		public:
			bool initialize() override
			{

				return true;
			}

			bool parse_property(const std::string & name, const std::string & value)override
			{
				properties[name] = value;
				return true;
			}

			void do_step()
			{

			}
			void update(Transform_Component * parent)
			{

			}

			float * Get_Model_View_matrix() { return glm::value_ptr(transformation_matrix); }

		};

		////////////////////////////////
		////
		////	TRANSFORM TASK		////
		////
		////////////////////////////////

		class Transform_Task : public Task
		{

			Transform_Module * transform_module;

		public:

			Transform_Task(Transform_Module * module) : transform_module(module)
			{

			}

			Transform_Task(int priority, Transform_Module * module) : Task(priority), transform_module(module)
			{
				int i = 0;

				// Este es el que mueve, I guess

			}

		public:
			bool initialize() override
			{
				return true;
			}
			
			bool do_step(float time) override // recorre la lista de render activa material y dibuja mesh
			{
				return true;
			}

			bool finalize() override
			{
				return false;
			}

		private:
			void update_entity(Entity * entity, Transform_Component * parent_transform)
			{

			}
		};

		/////////////////////////////////
		////						 ////
		////	TRANSFORM MODULE	 ////
		////						 ////
		/////////////////////////////////

	private:
		std::list< shared_ptr< Transform_Component > > transforms;
		Transform_Task task;

	public:

		static Transform_Module_Factory factory;

	public:

		Transform_Module(const int & priority_task, EvolutionScene * owner)
			: Module(owner)
			, task(priority_task, this)
		{

		}

		~Transform_Module() {}

	public:
		Task * get_task() override
		{
			return &task;
		}
		
		Component * create_component (std::shared_ptr<Entity> parent) override
		{
			return new Transform_Component (parent);
		}

		std::shared_ptr<Entity> get_scene_root()
		{
			
		}

	};
}
#endif
