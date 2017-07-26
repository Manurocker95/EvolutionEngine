#pragma once

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <map>
#include <string>
#include <memory>
#include <Math.hpp>
#include <iostream> 
#include "Transform_Module.hpp"
#include "Render_Module.h"
#include "Physics_Module.h"
#include "Module_Control.h"
#include "Module_Input.h"
#include "DebugManager.h"
#include "Scene_Object.hpp"
#include "btBulletDynamicsCommon.h"
//#include "SFML\Audio.hpp"

namespace EvolutionEngine
{
	static const float RADIANES = 0.017453f;		///< constante para la conversion de grados a radianes (pi/180º)
	static const float PI = 3.141592f;				///< Valor de PI
	
	class Entity : toolkit::Scene_Object
	{

	protected:

		std::map < string, std::shared_ptr < Component > > components;

	public:

		enum Entity_Type
		{
			PLAYER,				
			FLOOR,				
			PLATFORM,			
			KEY,		
			WALL,
			TRIGGER_DOOR		
		};

	protected:
		//sf::Sound sound;
		Entity_Type	entity_type;				///< Define el tipo de entidad.
		bool jump;
		bool isPlayer;
		bool isKey;
		bool isPlatform;
		bool isTrigger;
		bool detectCollisions;
		bool hasKey;
		bool moving;
		bool active;
		float jump_delay;
		int dir, dir2;
		int key_counter;
		int key_number;
		float	death_zone;		
		float	max_move;		

	public:
		
		Entity()
		{
			dir = 0;
			dir2 = 0;
			key_number = 0;
			key_counter = 0;
			moving = false;
			isKey = false;
			jump = false;
			hasKey = false;
			isTrigger = false;
			active = true;
			jump_delay = 2.0f;
			isPlayer = false;
			entity_type = FLOOR;
			death_zone = -20;
			max_move = 6.f;
			detectCollisions = false;
		}

		Entity(bool isPlayer)
		{
			dir = 0;
			dir2 = 0;
			key_number = 0;
			key_counter = 0;
			active = true;
			moving = false;
			jump = false;
			hasKey = false;
			isKey = false;
			isTrigger = false;
			detectCollisions = false;                          
			jump_delay = 2.0f;
			death_zone = -20;
			max_move = 6.f;
			this->isPlayer = isPlayer;
			if (isPlayer)
				entity_type = PLAYER;
			else
				entity_type = FLOOR;

			if (isPlayer)
				detectCollisions = true;
		}

		Entity(Entity_Type _entity_type)
		{
			dir = 0;
			dir2 = 0;
			key_number = 0;
			key_counter = 0;
			entity_type = _entity_type;
			jump = false;
			active = true;
			hasKey = false;
			jump_delay = 2.0f;
			death_zone = -20;
			max_move = 6.f;
			isKey = false;
			isPlayer = false;
			moving = false;
			detectCollisions = false;

			if (_entity_type == Entity::PLAYER)
				isPlayer = true;
			else if (_entity_type == Entity::KEY)
				isKey = true;
			else if (_entity_type == Entity::TRIGGER_DOOR)
				isTrigger = true;

			if (isPlayer || isKey || isTrigger)
			{
				detectCollisions = true;
			}

			/*
			sf::SoundBuffer buffer;
			if (buffer.loadFromFile("Assets\\Sounds\\good.wav"))
			{
				sound.setBuffer(buffer);
			}
			*/
		}

		Entity(Entity_Type _entity_type, int key_numb)
		{
			dir = 0;
			dir2 = 0;
			key_number = key_numb;
			key_counter = 0;
			entity_type = _entity_type;
			jump = false;
			active = true;
			hasKey = false;
			jump_delay = 2.0f;
			death_zone = -20;
			max_move = 6.f;
			isKey = false;
			isPlayer = false;
			moving = false;
			detectCollisions = false;

			if (_entity_type == Entity::PLAYER)
				isPlayer = true;
			else if (_entity_type == Entity::KEY)
				isKey = true;
			else if (_entity_type == Entity::TRIGGER_DOOR)
				isTrigger = true;

			if (isPlayer || isKey || isTrigger)
			{
				detectCollisions = true;
			}
		}
		int getNumber() { return key_number; }
		int getCounter() { return key_counter; }
		bool IsPlayer() { return isPlayer; }
		bool IsKey() { return isKey; }
		bool IsTrigger() { return isTrigger; }
		bool IsActive() { return active; }
		void SetActive(bool value);

		bool initialize()
		{
			bool result = true;

			for (auto component : components)
			{
				if (component.second->initialize() == false)
				{
					result = false;
				}
			}

			return result;
		}

		void go_to_original_position()
		{
			glm::vec3 rotation, scale;
			toolkit::Vector3 original_pos;

			shared_ptr < toolkit::Model > model = nullptr;

			if (components.count("render") > 0)
			{
				model = components["render"]->model;
			}

			if (components.count("transform") > 0)
			{
				original_pos = components["transform"]->getOriginalPosition();
				rotation = components["transform"]->getRotation();
				scale = components["transform"]->getScale();

				if (model)
				{
					model->translate(original_pos);
					model->rotate_around_x(rotation[0]);
					model->rotate_around_y(rotation[1]);
					model->rotate_around_z(rotation[2]);
					model->scale(scale[0], scale[1], scale[2]);
				}
			}
		}

		btDiscreteDynamicsWorld * getPhysicsWorld()
		{
			return nullptr;
//			return parent_scene->getPhysicsWorld().get();
		}

		glm::vec3 getScale()
		{
			if (components.count("transform") > 0)
			{
				return components["transform"]->getScale();
			}
			else
			{
				return glm::vec3(-1, -1, -1);
			}
		}

		glm::vec3 getRotation()
		{
			if (components.count("transform") > 0)
			{
				return components["transform"]->getRotation();
			}
			else
			{
				return glm::vec3(1, 1, 1);
			}
		}

		toolkit::Vector3 getPosition()
		{
			if (components.count("transform") > 0)
			{
				return components["transform"]->getPosition();
			}
			else
			{
				return toolkit::Vector3(0, 0, 0);
			}
		}

		std::shared_ptr < toolkit::Model > getModel()
		{
			if (components.count("render") > 0)
			{
				return components["render"]->model;
			}
			else
			{
				return nullptr;
			}
		}

		virtual void update();

		void collision_to(Entity * other);

		void Entity::reset_position(btTransform & tr);

		virtual void do_something() {}

		void move(bool m = false, int _dir = 0, int _dir2 = 0) 
		{
			moving = m;
			dir = _dir;
			dir2 = _dir2;
		}

		void start_jump()
		{
			if (jump)
			{
				if (components.count("physics") > 0)
				{
					components["physics"]->physics_model->applyCentralImpulse(btVector3(0.f, 10.f, 0.f));
					jump = false;
				}
			}

		}

		void set_can_jump_again()
		{
			if (jump_delay < 0)
			{
				jump = true;
				jump_delay = 2.f;
			}
			else
			{
				jump_delay -= 0.1f;
			}
		}

		void set_trigger()
		{
			if (components.count("physics") > 0)
				components["physics"]->physics_model->setCollisionFlags(components["physics"]->physics_model->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			else
				DebugLog("No se puede añadir el flag");
		}

		void set_visible(const bool & visible)
		{
			components["render"]->model->set_visible(visible);
		}

		std::shared_ptr < toolkit::Model > get_graphic_model()
		{
			return components ["render"]->model;
		}

		btRigidBody * get_physics_model()
		{
			if (components.count("physics") > 0)
			{
				btRigidBody * mm = components["physics"]->physics_model.get();
				return components["physics"]->physics_model.get();
			}
			else
			{
				return nullptr;
			}
		}

		shared_ptr< btRigidBody > & get_physics_model_ptr()
		{
			return components["physics"]->physics_model;
		}
		
		Entity::Entity_Type get_entity_type() const
		{
			return entity_type;
		}

		bool add_component
		(
			const string & type,
			shared_ptr< Component > & component
		)
		{
			if (components.count(type) != 0)
			{
				return false;
			}
			else
			{
				components[type] = component;
				return true;
			}
		}

	};

}

#endif