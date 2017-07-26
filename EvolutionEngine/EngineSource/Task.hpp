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

// Tareas consumibles o no consumibles
// Secuencia de tareas (hereda de task y tiene una lista de tasks)

#ifndef TASK_HPP
#define TASK_HPP

#include "Kernel.hpp"

namespace EvolutionEngine
{
	class Kernel;
	class EvolutionScene;

	class Task
	{

	protected:

		Kernel * kernel;
		EvolutionScene * owner_scene;
		int priority;

	public:

		Task()
		{
			priority = 0;
			kernel = nullptr;
		}

		Task(int priority) : priority(priority)
		{
			kernel = nullptr;
		}

		virtual ~Task()
		{

		}

		Kernel * get_kernel() { return kernel; }
		void set_kernel(Kernel * new_kernel) { kernel = new_kernel; }
		void Set_Owner_Scene(EvolutionScene * _owner_scene) { owner_scene = _owner_scene; }
		EvolutionScene * Get_Owner_Scene() { return owner_scene; }

	public:

		virtual bool initialize() = 0;
		virtual bool finalize() = 0;
		virtual bool do_step(float time) = 0;

	public:

		bool operator < (const Task & other) const
		{
			return this->priority < other.priority;
		}
	};
}

#endif