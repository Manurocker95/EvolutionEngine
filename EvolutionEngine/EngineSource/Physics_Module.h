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
#include "btBulletDynamicsCommon.h"

namespace EvolutionEngine
{
	class Entity;
	class Physics_Module : public Module
	{
	public:

		class Physics_Module_Factory : public Factory
		{
		
		private:

			std::shared_ptr < Module > physics_module;

		public:
			Physics_Module_Factory()
			{
				Module::register_module("physics", this);
			}

			shared_ptr< Module > create_module() override
			{
				return shared_ptr< Module >();
			}

			shared_ptr< Module > create_module(EvolutionScene * owner) override
			{
				physics_module = shared_ptr< Module >(new Physics_Module(0, owner));
				return physics_module;
			}
		};


		class Physics_task : public Task
		{
			Physics_Module * physics_module;

		public:
			Physics_task(Physics_Module * module) : physics_module(module)
			{
				
			}

			Physics_task(int priority, Physics_Module * module);

			bool initialize() override;

			bool finalize() override;

			bool do_step(float time) override;
		};

		class Physics_Component : public Component
		{
		public:

			enum Body_Type
			{
				STATIC,				
				DYNAMIC		
			};

			Body_Type body_type;

			std::shared_ptr< btCollisionShape > shape;
			std::shared_ptr< btRigidBody      > body;
			std::shared_ptr< btDefaultMotionState  > motion;							
			std::shared_ptr< btRigidBody::btRigidBodyConstructionInfo > infoRigidBody;	
			EvolutionScene * owner;

		public:

			Physics_Component()
			{
				type = "physics";
			}

			Physics_Component(EvolutionScene * _owner)
			{
				type = "physics";
				owner = _owner; 
				body_type = STATIC;
				// CREAR INSTANCIA DE RIGID BODY...
			}

			Physics_Component(EvolutionScene * _owner, std::string _body_type)
			{
				type = "physics";
				owner = _owner;
				
				if (_body_type.compare("dynamic") == 0)
				{
					body_type = DYNAMIC;
				}
				else
				{
					body_type = STATIC;
				}
			}

			Physics_Component(std::shared_ptr<Entity> parent);

			void setPhysics(std::shared_ptr<Entity> _parent) override;

			bool initialize() override;

			bool parse_property(const std::string & name, const std::string & value) override
			{
				properties[name] = value;
				return true;
			}

			void do_step()
			{

			}

			std::shared_ptr<btRigidBody> getBody()
			{
				return body;
			}

		};

	public:

		static Physics_Module_Factory factory;

	private:

		Physics_task * task;

	public:

		std::shared_ptr< btDiscreteDynamicsWorld >				physics_world;				///< Escena de Bullet.
		std::shared_ptr< btDefaultCollisionConfiguration >		collisionConfiguration;		///< Configuracion de colisiones.
		std::shared_ptr< btCollisionDispatcher >				collisionDispatcher;		///< Gestor de colisones.
		std::shared_ptr< btDbvtBroadphase >						overlappingPairCache;		///<
		std::shared_ptr< btSequentialImpulseConstraintSolver >	constraintSolver;			///<

	public:

		Physics_Module(const int & priority_task, EvolutionScene * owner)
			: Module(owner)
		{
			task = new Physics_task(priority_task, this);
		}

		Task * get_task() override
		{
			return task;
		}

		void set_task(Physics_task * _task)
		{
			task = _task;
		}
	};
}