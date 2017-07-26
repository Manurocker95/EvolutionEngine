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


#include "EvolutionScene.hpp"

#include "rapidxml.hpp"
#include <iostream>
#include <cassert>
#include <sstream>
#include <fstream>

#include "tiny_obj_loader.h"
#include <memory>
#include "Vector.hpp"
#include "DebugManager.h"
#include <Light.hpp>
#include <Camera.hpp>
#include "Render_Module.h"
#include "Physics_Module.h"
#include "Transform_Module.hpp"


using namespace rapidxml;

namespace EvolutionEngine
{
	EvolutionScene::EvolutionScene(const string & scene_description_file_path) : kernel(new Kernel), scene(new Scene)
	{
		Initialize();
		if (load_scene(scene_description_file_path))
		{
			init_kernel();
		}
		else
		{
			DebugLogError("No se ha podido cargar la escena. Revisa que el archivo de escena sea correcto y vuelve a intentarlo.", "Error de inicialización de la escena", false);
			return;
		}
	}

	EvolutionScene::EvolutionScene(const string & scene_description_file_path, sf::Window * w) : kernel(new Kernel), scene(new Scene)
	{
		Evolwindow = w;
		if (load_scene(scene_description_file_path))
		{
			init_kernel();
		}
		else
		{
			DebugLogError("No se ha podido cargar la escena. Revisa que el archivo de escena sea correcto y vuelve a intentarlo.", "Error de inicialización de la escena", false);
			return;
		}
	}

	void EvolutionScene::Initialize()
	{
		wallsdisappeared = false;
	}


	//Método que carga la escena

	bool EvolutionScene::load_scene(const string & scene_description_file_path)
	{

		// primero cargamos los módulos

		//Module map debe ser publico y estático
		Module::Module_Map & module_map = Module::get_module_map();

		 // Input no es un módulo asignable
		//Component c.reset(new Render_Module::Render_Component(model));

		if (!LoadDataFromXML(scene_description_file_path))
		{
			DebugLogError("No se han podido leer los datos de la escena correctamente");
			return false;
		}

		// Parseando componentes de entidades:
		// ejemplo: Render
		for (auto component : xml_components)
		{
			Module * module = nullptr;

			// si entre nuestros módulos creados no tenemos render
			if (modules.count(component->type) == 0)
			{
				// Comprobamos si nuestro modulemap tiene ese tipo de módulo registrado
				if (module_map.count(component->type) == 0)
				{
					DebugLogError("No hay ningún módulo de " + component->type +"en los módulos posibles");
					// si es crítico, sale, sino lo omite 
					if (component->isCrit)
					{	
						DebugLogError("El componente de tipo:"+component->type+" es crítico y no existe en module_map.");
						return false;
					}
				}
				// Si sí que existe ese modulo, lo creamos
				else
				{
					modules[component->type] = module_map[component->type]->create_module(this);
					module = modules[component->type].get();
				}
			}
			// Si ya tenemos el módulo creado previamente, solo accedemos a él
			else
			{
				module = modules[component->type].get();
			}

			// si module no es null
			if (module != nullptr)
			{
				// INSTANCIAR EL COMPONENTE Y AÑADIRLO A LA ENTIDAD
				// component = modules["render"]->create_component(entities["entity"]);
				// component->initialize(); // Ya le hemos pasado el padre antes

				entities[component->parentName]->add_component(component->type, component);

				if (!component->initialize())
				{
					DebugLogError("No se puede inicializar el componente " + component->type + " perteneciente a la entidad de nombre " + component->parentName);
					return false;
				}

				if (component->type.compare("render") == 0)
				{
					scene->add(component->parentName, component->model);
					//component->model->translate(toolkit::Vector3(-2.f, 0.f, -5.f));
				}

				if (component->type.compare("physics") == 0)
				{
					component->setPhysics(entities[component->parentName]);
				}
			}
			else
			{
				DebugLogError("Se ha producido un error al cargar el modulo de "+component->type+".", "");
				return false;
			}
		}

		for (auto & entity : entities)
		{
			entity.second->go_to_original_position();
		}

		// otros
		shared_ptr< toolkit::Camera > camera(new toolkit::Camera(20.f, 1.f, 50.f, 1.f));
		shared_ptr< toolkit::Light  > light(new toolkit::Light);
		light->translate(toolkit::Vector3(10.f, 10.f, 10.f));

		scene->add("camera", camera);
		scene->add("light", light);
		scene->set_active_camera("camera");
		
		return true;
	}


	void EvolutionScene::render()
	{
		scene->render();
	}

	bool EvolutionScene::LoadDataFromXML(const string & xml_path)
	{
		xml_document<> doc;
		xml_node<> * root_node;

		bool can_read = false;

		// Intentamos abrir el XML. 
		std::ifstream xml_file (xml_path);

		// Si podemos abrirlo sin problemas continuamos
		if (xml_file.good())
		{
			can_read = true;
		}
		else
		{
			DebugLogError("No se puede leer el xml");
			return false;
		}

		if (can_read)
		{
			vector<char> buffer((std::istreambuf_iterator<char>(xml_file)), std::istreambuf_iterator<char>());
			buffer.push_back('\0');

			// Parseamos el buffer parseando la librería a Doc
			doc.parse<0>(&buffer[0]);

			// Encontramos el primer nodo (Nodo raíz = Scene)
			root_node = doc.first_node("scene");

			// strings parsed

			std::string name = "", model = "", id = "", bodyType="";
			int counter = 0;
			glm::vec3 position, rotation, scale;
			int key_num = 0;

			for (xml_node<> * object_node = root_node->first_node("entity"); object_node; object_node = object_node->next_sibling())
			{
				counter++;

				if (object_node->first_attribute("id") && object_node->first_attribute("id")->value_size() > 0)
				{
					id = object_node->first_attribute("id")->value();
				}
				else
				{
					DebugLogError("Hay un objeto sin la propiedad ID. Por favor, revisa tu archivo de escena");
					return false;
				}

				// Comprobamos de qué tipo es el componente
				if (object_node->first_attribute("name") && object_node->first_attribute("name")->value_size() > 0)
				{
					name = object_node->first_attribute("name")->value();

					if (entities.count(name) == 0)
					{
						if (!player)
						{
							if (id.compare("player") == 0)
							{
								entities[name].reset(new Entity(Entity::Entity_Type::PLAYER));
								player = entities[name];
							}
							else
							{
								if (id.compare("key") == 0)
								{
									entities[name].reset(new Entity(Entity::Entity_Type::KEY, key_num));
									key_num++;
								}
								else if (id.compare("trigger") == 0)
									entities[name].reset(new Entity(Entity::Entity_Type::TRIGGER_DOOR));
								else if (id.compare("platform") == 0)
									entities[name].reset(new Entity(Entity::Entity_Type::PLATFORM));
								else if (id.compare("wall") == 0)
									entities[name].reset(new Entity(Entity::Entity_Type::WALL));
								else
									entities[name].reset(new Entity(Entity::Entity_Type::FLOOR));
							}
						}
						else
						{
							if (id.compare("key") == 0)
							{
								entities[name].reset(new Entity(Entity::Entity_Type::KEY, key_num));
								key_num++;
							}				
							else if (id.compare("trigger") == 0)
								entities[name].reset(new Entity(Entity::Entity_Type::TRIGGER_DOOR));
							else if (id.compare("platform") == 0)
								entities[name].reset(new Entity(Entity::Entity_Type::PLATFORM));
							else if (id.compare("wall") == 0)
								entities[name].reset(new Entity(Entity::Entity_Type::WALL));
							else
								entities[name].reset(new Entity(Entity::Entity_Type::FLOOR));
						}
					}			
					else
					{
						DebugLogError("Hay más de un objeto con el mismo nombre");
						return false;
					}
						
				}
				else
				{

					DebugLogError("Hay un objeto sin la propiedad nombre. Por favor, revisa tu archivo de escena");
					return false;
				}

				xml_node<> * components_node = object_node->first_node("components");

				// Dentro de cada entity parseamos cada componente
				for (xml_node<> * object_node_child = components_node->first_node("component"); object_node_child; object_node_child = object_node_child->next_sibling())
				{
					bool isCrit = false;
					std::string type;
					std::shared_ptr < Component > c = nullptr;

					// Comprobamos de qué tipo es el componente
					if (object_node_child->first_attribute("type") && object_node_child->first_attribute("type")->value_size() > 0)
					{
						type = object_node_child->first_attribute("type")->value();
					}
					else
					{
						DebugLogError("No está definido el tipo de componente en una de las entidades de la escena");
						return false;
					}

					// Comprobamos si es crítico o no
					if (object_node_child->first_attribute("critical") && object_node_child->first_attribute("critical")->value_size() > 0)
					{
						std::string a = object_node_child->first_attribute("critical")->value();

						if (a.compare("true") == 0)
							isCrit = true;
						else
							isCrit = false;

						if (isCrit != true && isCrit != false)
						{
							DebugLogError("Hay algún error en alguno de los componentes respecto a si es crítico o no.");
							return false;
						}
					}

					if (type.compare("render") == 0)
					{
						
						// Datos del módulo de render
						// ...     
						// Para esta práctica (La de ánimación 2, no usaré mis propios modelos sino cajas)
						if (object_node_child->first_node("model"))
						{
							xml_node<> * model_node = object_node_child->first_node("model");
							model = model_node->value();
							
						}
						else
						{
							DebugLogError("El componente de render de una de las entidades no tiene definido el modelo");
							return false;
						}

						c.reset(new Render_Module::Render_Component(model));
					}
					else if (type.compare("transform") == 0)
					{
						// Datos del módulo de transform
						// ...
						// Comprobamos que el nodo de posición existe
						if (object_node_child->first_node("position"))
						{
							xml_node<> * position_node = object_node_child->first_node("position");
							position = string_to_vec3f(position_node->value());
						}
						else
						{
							DebugLogError("El componente de transform de una de las entidades no tiene definida la posición.");
							return false;
						}

						// Comprobamos que el nodo de rotación existe
						if (object_node_child->first_node("rotation"))
						{
							xml_node<> * rotation_node = object_node_child->first_node("rotation");
							rotation = string_to_vec3f(rotation_node->value());
						}
						else
						{
							DebugLogError("El componente de transform de una de las entidades no tiene definida la rotación.");
							return false;
						}

						// Comprobamos que el nodo de escala existe
						if (object_node_child->first_node("scale"))
						{
							xml_node<> * scale_node = object_node_child->first_node("scale");
							scale = string_to_vec3f(scale_node->value());
						}
						else
						{
							DebugLogError("El componente de transform de una de las entidades no tiene definida la escala.");
							return false;
						}
						c.reset(new Transform_Module::Transform_Component(position, rotation, scale));
					}
					else if (type.compare("physics") == 0)
					{
						// Datos del módulo de físcias
						// ...
						if (object_node_child->first_node("bodyType"))
						{
							xml_node<> * bodytype_node = object_node_child->first_node("bodyType");
							bodyType = bodytype_node->value();
						}
						else
						{
							DebugLogError("El componente de físicas de una de las entidades no tiene definido el tipo de cuerpo.");
							return false;
						}

						c.reset(new Physics_Module::Physics_Component(this, bodyType));
					}
					else if (type.compare("control") == 0)
					{
						// Datos del módulo de control
						// ...

						c.reset(new Control_Module::Control_Component(this));
					}
					else if (type.compare("input") == 0)
					{
						// Datos del módulo de input
						// ...

						c.reset(new Input_Module::Input_Component(this));
					}
					if (c)
					{
						c->parentName = name;
						xml_components.push_back(c);
					}
				}

				std::cout << std::endl;
			}
		}

		return true;
	}

	toolkit::Vector3f EvolutionScene::string_to_vector3f(std::string s)
	{
		std::vector<float> v3_values;
		std::stringstream ss(s);

		float i;

		while (ss >> i)
		{
			v3_values.push_back(i);

			if (ss.peek() == ',' || ss.peek() == ' ')
			{
				ss.ignore();
			}
		}

		toolkit::Vector3f v;

		v[0] = v3_values.at(0);
		v[1] = v3_values.at(1);
		v[2] = v3_values.at(2);

		return v;
	}

	glm::vec3 EvolutionScene::string_to_vec3f(std::string s)
	{
		std::vector<float> v3_values;
		std::stringstream ss(s);

		float i;

		while (ss >> i)
		{
			v3_values.push_back(i);

			if (ss.peek() == ',' || ss.peek() == ' ')
			{
				ss.ignore();
			}
		}

		glm::vec3 v;

		v[0] = v3_values.at(0);
		v[1] = v3_values.at(1);
		v[2] = v3_values.at(2);

		return v;
	}

	void EvolutionScene::init_kernel()
	{
		for (auto module : modules)
		{
			Task * task = module.second->get_task();

			if (task)
			{
				task->Set_Owner_Scene(this);
				kernel->add_task(*task);
			}
		}
	}

	void EvolutionScene::execute()
	{
		kernel->execute();
	}


	void EvolutionScene::Update()
	{
		for (auto entity : entities)
		{
			entity.second->update();
		}

		if (showing_middleware_demo && !wallsdisappeared && player && player->getCounter() >= number_of_keys)
		{		
			entities["wall1"]->set_visible(false);
			entities["wall2"]->set_visible(false);
			entities["wall3"]->set_visible(false);
			entities["wall4"]->set_visible(false);
			wallsdisappeared = true;
		}

		
	}
	void EvolutionScene::on_key_pressed(int key_code)
	{
		if (key_code == sf::Keyboard::Space || key_code == sf::Keyboard::Up)
		{
			if (player)
				player->start_jump();
		}
		if (key_code == sf::Keyboard::Right)
		{
			if (player)
				player->move(true, -1);
		}
		if (key_code == sf::Keyboard::Left)
		{
			if (player)
				player->move(true, 1);
		}

	}

	void EvolutionScene::on_key_released(int key_code)
	{

		if (key_code == sf::Keyboard::Right)
		{
			if (player)
				player->move(false, 0);
		}
		
		if (key_code == sf::Keyboard::Left)
		{
			if (player)
				player->move(false,  0);
		}

	}

	void EvolutionScene::Check_Collisions()
	{
		if (!physics_world)
			return;

		int manifold_count = physics_world->getDispatcher()->getNumManifolds();
		btDispatcher * a = physics_world->getDispatcher();

		for (int i = 0; i < manifold_count; i++)
		{
			btPersistentManifold * manifold = physics_world->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject * object_a = (btCollisionObject *)(manifold->getBody0());
			btCollisionObject * object_b = (btCollisionObject *)(manifold->getBody1());

			int numContacts = manifold->getNumContacts();

			for (int j = 0; j < numContacts; j++)
			{
				btManifoldPoint & point = manifold->getContactPoint(j);
				if (point.getDistance() < 0.001f)
				{
					Entity * entity_1 = reinterpret_cast<Entity *>(object_a->getUserPointer());
					Entity * entity_2 = reinterpret_cast<Entity *>(object_b->getUserPointer());

					entity_1->collision_to(entity_2);
					entity_2->collision_to(entity_1);
				}
			}
		}
	}
}