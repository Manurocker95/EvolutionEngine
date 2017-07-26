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

#include "Render_Module.h"
#include "EvolutionScene.hpp"

namespace EvolutionEngine
{
	Render_Module::Render_Module_Factory Render_Module::factory;

	bool Render_Module::Render_task::do_step(float time)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

		if (render_module)
			render_module->Get_Owner()->render();
		return true;
	}

	bool Render_Module::Render_task::finalize()
	{
		delete render_module;

		return true;
	}

	bool Render_Module::Render_task::initialize()
	{
		// ...
		return true;
	}
}