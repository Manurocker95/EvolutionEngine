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

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
#include <memory>
#include "btBulletDynamicsCommon.h"
#include <Model.hpp>

using std::string;
using std::shared_ptr;

namespace EvolutionEngine
{
	class Entity;

	class Component
	{
	public:
		std::string type;
		std::string parentName;
		std::string objectType;
		
		std::shared_ptr < toolkit::Model > model;
		std::shared_ptr < btRigidBody > physics_model;				///< Modelo fisico.
		bool isCrit;
	
	protected:

		std::shared_ptr< Entity > parent;
		typedef std::map<string, string > Properties;
		Properties properties;
	public:

		Component() {}

		Component(std::shared_ptr <Entity> parent) : parent(parent)
		{
			
		}

		virtual ~Component()
		{
		}

		virtual bool initialize() = 0;

		virtual bool parse_property
		(
			const string & name,
			const string & value
		) = 0;
	  
	   virtual void setPhysics(std::shared_ptr<Entity> parent) {}
	   void set_parent(std::shared_ptr< Entity > _parent) { parent = _parent; }
	   void setIsCrit(bool value) { isCrit = value; }
	   bool IsCrit() { return isCrit; }

	   virtual toolkit::Vector3 getOriginalPosition() { return glm::vec3(0, 0, 0); }
	   virtual glm::vec3 getPosition() { return glm::vec3(0, 0, 0); }
	   virtual glm::vec3 getRotation() { return glm::vec3(0, 0, 0); }
	   virtual glm::vec3 getScale() { return glm::vec3(0, 0, 0); }

	};
}

#endif