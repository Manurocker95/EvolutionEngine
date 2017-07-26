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

#include "Kernel.hpp"
#include "Module.hpp"
#include "Vector.hpp"
#include "Scene.hpp"
#include "Scene_Object.hpp"
#include <btBulletDynamicsCommon.h>
#include "Entity.hpp"
#include "EvolutionWindow.h"

using namespace std;

namespace EvolutionEngine
{
	class Particle_System;
	class EvolutionScene : public toolkit::Scene
	{

	private:
		typedef map< string, shared_ptr< Module > > Module_Map;
		typedef map< string, shared_ptr< Entity > > Entity_Map;

	private:
		std::shared_ptr < Scene > scene;

		Entity_Map entities;
		
		sf::Window * Evolwindow;
		bool wallsdisappeared;
		Module_Map modules;
		Kernel   *  kernel;
		
		Entity     root;

		std::shared_ptr < Entity > player;

		std::vector < std::shared_ptr < Component > > xml_components;
		
		toolkit::Vector3 camera_position;					///< Posicion de la cmara.
		float    camera_rotation_x;					///< Rotacion en el eje X de la camara
		std::shared_ptr< btDiscreteDynamicsWorld >	physics_world;				///< Escena de Bullet.

	public:

		Module_Map get_modules()
		{
			return modules;
		}

		std::shared_ptr< btDiscreteDynamicsWorld >	 getPhysicsWorld()
		{
			return physics_world;
		}

		void setPhysicsWorld(btDiscreteDynamicsWorld * world)
		{
			physics_world.reset(world);
		}

		sf::Window * getEvolwindow()
		{
			return Evolwindow;
		}

	public:
		EvolutionScene(const string & scene_description_file_path, sf::Window * window);
		EvolutionScene(const string & scene_description_file_path);
		bool LoadDataFromXML(const string & xml_path);
		toolkit::Vector3f string_to_vector3f(std::string s);
		glm::vec3 string_to_vec3f(std::string s);
		std::shared_ptr < Entity > get_player() { return player; }
	private:
		void Initialize();
		bool load_scene(const string & scene_description_file_path);
		void init_kernel();
		
	public:
		void execute();
		void render();
		void Update();
		void on_key_pressed(int key_code);
		void on_key_released(int key_code);
		void Check_Collisions();
		std::shared_ptr < Scene > getScene() { return scene; }
	};
}