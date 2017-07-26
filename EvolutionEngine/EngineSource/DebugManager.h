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

#include <sstream>
#include <fstream>
#include <iostream>
#include <direct.h>  
#include <stdlib.h>  
#include <stdio.h> 
#include "windows.h"
#include "Settings.h"

namespace EvolutionEngine
{

	using namespace std;

	class DebugManager
	{
	public:
		static DebugManager* instance();
		~DebugManager();
		 void show_error(std::string msg = "Se ha encontrado un error", std::string windowname = "Error encontrado", UINT button = MB_OK, bool savinginlog = true);
		 void show_error_and_close(std::string msg = "Se ha encontrado un error", std::string windowname = "Fatal Error", bool savinginlog = true);
		 static void Log(std::string msg, std::string windowname = "Log Message", UINT button = MB_OK);

	private:
		DebugManager() {};  // Private so that it can  not be called
		DebugManager(DebugManager const&) {};             // copy constructor is private
		DebugManager& operator=(DebugManager const&) {};  // assignment operator is private
		static DebugManager* Instance;
	};

	static void DebugLog(std::string msg, std::string windowname = "", UINT button = MB_OK) { DebugManager::instance()->Log(msg, windowname, button); }
	static void DebugLogError(std::string msg = "Se ha encontrado un error", std::string windowname = "Error encontrado", bool savinginlog = true, bool close_after_showing = false, UINT button = MB_OK)
	{
		if (close_after_showing)
			DebugManager::instance()->show_error(msg, windowname, button, savinginlog);
		else
			DebugManager::instance()->show_error_and_close(msg, windowname, savinginlog);
	}
}