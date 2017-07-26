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

#include <map>
#include <string>
#include <memory>

using std::map;
using std::string;
using std::shared_ptr;

// Modulos desconocidos que se reconocen mediante enlace estatico (otra opcion mas tocha seria mediante enlace din�mico)

namespace EvolutionEngine
{
	class Message_Handler;
	class EvolutionScene;
	class Component;
	class Entity;
	class Task;
	
    class Module
    {
    public:

		enum PRIORITY
		{
			VERY_HIGH = 4,
			HIGH = 3,
			MEDIUM = 2,
			LOW = 1,
			VERY_LOW = 0
		};

        class Factory
        {
        public:
            virtual shared_ptr < Module > create_module () = 0;
			virtual shared_ptr < Module > create_module(EvolutionScene * owner) = 0;
        };

	public:

        typedef map < string, Module::Factory * > Module_Map;

        static Module_Map modules;

    public:

		virtual Message_Handler * getMessageHandler()
		{
			return nullptr;
		}

        // Reimplementar si se tiene Task.
        virtual Task * get_task ()
        {
            return nullptr;
        }

		virtual Component * create_component (std::shared_ptr < Entity > parent)
		{
			return nullptr;
		}

	protected:
		EvolutionScene * owner;

	public:
		void Set_Owner(EvolutionScene * _owner) { owner = _owner; }
		EvolutionScene * Get_Owner() { return owner; }

    protected:

		Module() {};
		Module(EvolutionScene * owner)
		{
			this->owner = owner;
		}

        virtual ~Module()
        {
        }

	protected:

        static void register_module (const string &id, Module::Factory * factory)
        {
            modules[id] = factory;
        }
	
	public:
        static Module_Map & get_module_map()
        {
            return modules;
        }

	public:

    };

}
