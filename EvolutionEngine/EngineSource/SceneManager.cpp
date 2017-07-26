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

#include "SceneManager.h"

namespace EvolutionEngine
{
	// Static instance for singleton
	SceneManager* SceneManager::Instance = NULL;

	// Static method to return the singleton instance, if we don't have any instance, we create it
	SceneManager* SceneManager::instance()
	{
		if (!Instance)
			Instance = new SceneManager;

		return Instance;
	}

	// We delete the pointers when the SceneManager is destroyed so we don't have leaks
	SceneManager::~SceneManager()
	{
		
	}

	// Initialization of variables in SceneManager
	void SceneManager::Start(sf::Window * _window)
	{
		running = true;
		window = _window;
	}

	// Draw: It calls the paint method of the scene
	void SceneManager::Draw()
	{
		if (showing_default_scene)
			scene->render();
		else
			evolscene->render();
	}

	// Update: It calls the update method of the scene
	void SceneManager::Update(float delta_time)
	{
		if (showing_default_scene)
			scene->get("cube2")->rotate_around_y(0.01f);
		else
			evolscene->Update();
	}

	void SceneManager::CreateEvolutionScene(const char * scene_path)
	{
		showing_default_scene = false;
		set_custom_scene(scene_path);

		if (running)
			reset_viewport(*window, *evolscene->getScene()); // Se inicializan algunos elementos de OpenGL:
		else
			return;
	}

	void SceneManager::CreateDefaultScene()
	{
		showing_default_scene = true;
		scene = set_default_scene();
		Scene_Object      * cube = scene->get("cube2");

		// Configuramos la escena
		configure_default_scene(*scene);

		// Se inicializan algunos elementos de OpenGL:
		reset_viewport(*window, *scene);
	}

	void SceneManager::set_custom_scene(const char * scene_path)
	{
		evolscene.reset(new EvolutionScene(scene_path, window));
	}


	shared_ptr< Scene > SceneManager::set_default_scene()
	{
		shared_ptr< Scene  > scene(new Scene);
		shared_ptr< Model  > cube1(new Model);
		shared_ptr< Model  > cube2(new Model);
		shared_ptr< Camera > camera(new Camera(20.f, 1.f, 50.f, 1.f));
		shared_ptr< Light  > light(new Light);

		cube1->add(shared_ptr< Drawable >(new Cube), Material::default_material());
		cube2->add(shared_ptr< Drawable >(new Cube), Material::default_material());

		scene->add("cube1", cube1);
		scene->add("cube2", cube2);
		scene->add("camera", camera);
		scene->add("light", light);

		return scene;
	}

	void SceneManager::configure_default_scene(Scene & scene)
	{
		scene["cube1"]->translate(toolkit::Vector3(-2.f, 0.f, -5.f));
		scene["cube2"]->translate(toolkit::Vector3(0.f, 0.f, -5.f));
		scene["cube2"]->rotate_around_x(0.25f);
		scene["light"]->translate(toolkit::Vector3(10.f, 10.f, 10.f));
	}

	void SceneManager::reset_viewport(const sf::Window & window, Scene & scene)
	{
		GLsizei  width = GLsizei(window.getSize().x);
		GLsizei  height = GLsizei(window.getSize().y);

		scene.get_active_camera()->set_aspect_ratio(float(width) / height);

		glViewport(0, 0, width, height);
	}

	// Check Inputs:  It calls the checkInput method of the scene for inputs and attend the global events such as closing the window
	void SceneManager::CheckInputs()
	{
		// Attend the window events:
		Event event;

		while (window->pollEvent(event))
		{
			
			// If the event is closing the window, we close it
			if (event.type == Event::Closed)
			{
				// We are not running the program anymore, so we close the window from main
				running = false;
				break;
			}

			if (event.type == Event::KeyPressed && !showing_default_scene)
			{
				evolscene->on_key_pressed(event.key.code);
				break;
			}

			if (event.type == Event::KeyReleased && !showing_default_scene)
			{
				evolscene->on_key_released(event.key.code);
				break;
			}

			if (event.type == Event::Resized)
			{
				if (showing_default_scene)
					reset_viewport(*window, *scene);
				else
					reset_viewport(*window, *(evolscene->getScene())); 

				break;
			}
		}
	}

	void SceneManager::Play() 
	{ 
		if (evolscene) 
			evolscene->execute(); 
		else
		{
			sf::Clock timer;
			float delta_time = 0.017f; // 60 fps

			do
			{
				CheckInputs();
				Update(delta_time);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Draw();
				Display();
				delta_time = (delta_time + timer.getElapsedTime().asSeconds()) * 0.5f;
			} while (running);
		}
	}

	bool SceneManager::File_Checker(std::string fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}
}