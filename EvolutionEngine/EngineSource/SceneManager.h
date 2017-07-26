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

#include <Cube.hpp>
#include "EvolutionScene.hpp"
#include <Model.hpp>
#include <Light.hpp>
#include <Camera.hpp>

#include <sstream>
#include <fstream>
#include <iostream>
#include <direct.h>  
#include <stdlib.h>  
#include <stdio.h> 

#include "Settings.h"

#include <SFML/Window.hpp>		// Cargamos la ventana

namespace EvolutionEngine
{
	using namespace sf;
	using namespace std;
	using namespace toolkit;

	class SceneManager
	{
	public:
		static SceneManager* instance();
		~SceneManager();
		void Start(sf::Window * window);
		void CheckInputs();
		void Draw();
		void Update(float delta_time);
		bool getRunning() { return running; }
		void setRunning(bool value) { running = value; }
		bool File_Checker(std::string fileName);
		void CreateDefaultScene();
		void CreateEvolutionScene(const char * scene_path);
		shared_ptr< Scene > set_default_scene();
		void set_custom_scene(const char * scene_path);
		void configure_default_scene(Scene & scene);
		void reset_viewport(const sf::Window & window, Scene & scene);

	private:
		sf::Window * window;
		shared_ptr< Scene > scene;					// Escena por defecto
		shared_ptr < EvolutionScene > evolscene;	// Escena custom
		bool running, showing_default_scene;
		SceneManager() {};  // Private so that it can  not be called
		SceneManager(SceneManager const&) {};             // copy constructor is private
		SceneManager& operator=(SceneManager const&) {};  // assignment operator is private
		static SceneManager* Instance;
	public:
		void Display() { window->display(); }
		void Play();
	};
}