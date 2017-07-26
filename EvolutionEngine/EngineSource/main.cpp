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


#include "EvolutionGame.h"

using namespace EvolutionEngine;

// Arguments: 0 = program, 1 = Scene
int main(int number_of_arguments, const char * args[])
{
	// Número de argumentos, argumentos, si queremos cargar una escena por defecto. Si no lo seteamos, es false por defecto
	EvolutionGame game(number_of_arguments, args, false);
	game.Play();

	return (EXIT_SUCCESS);
}
