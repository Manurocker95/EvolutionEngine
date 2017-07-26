#include "Entity.hpp"

namespace EvolutionEngine
{
	void Entity::update()
	{	
		btTransform physics_transform;
		toolkit::Matrix44	graphics_transform;

		if (components.count("physics") > 0)
		{
			components["physics"]->physics_model->getMotionState()->getWorldTransform(physics_transform);
			physics_transform.getOpenGLMatrix(glm::value_ptr(graphics_transform));

			if (isPlayer && active && components.count("transform") > 0)
			{
				if (moving)
				{
					btVector3 v_lineal = components["physics"]->physics_model->getLinearVelocity();
					if (v_lineal.getX() > -max_move && v_lineal.getX() < max_move)
						components["physics"]->physics_model->applyCentralImpulse(btVector3(-dir, 0, -dir2));
				}

				btTransform tr = components["physics"]->physics_model->getWorldTransform();
				btScalar hy = tr.getOrigin().getY();

				if (hy < death_zone) 
					reset_position(tr);
			}
		}
		if (components.count("render") > 0 && components.count("transform") > 0)
		{
			components["render"]->model->set_transformation(graphics_transform);
			components["render"]->model->scale(components["transform"]->getScale()[0], components["transform"]->getScale()[1], components["transform"]->getScale()[2]);
		}

	}

	void Entity::reset_position(btTransform & tr)
	{
		components["physics"]->physics_model->setLinearVelocity(btVector3(0, 0, 0));
		tr.setOrigin(btVector3(components["transform"]->getOriginalPosition()[0], components["transform"]->getOriginalPosition()[1], components["transform"]->getOriginalPosition()[2]));
		components["physics"]->physics_model->setWorldTransform(tr);
	}

	void Entity::SetActive(bool value)
	{
		active = value;
		if (components.count("render") > 0)
		{
			components["render"]->model->set_visible(active);
		}
	}

	void Entity::collision_to(Entity * other)
	{
		if (this->isPlayer)
		{
			if (other->get_entity_type() == Entity::Entity_Type::KEY)
			{
				if (components.count("physics") > 0)
				{
					components["physics"]->physics_model->setIgnoreCollisionCheck(other->get_physics_model(), true);
					
					if (key_counter == other->getNumber())
					{
						if (showing_middleware_demo)
						{
							DebugLog("Tenías: " + to_string(key_counter));
							key_counter += 1;
							DebugLog("Has cogido el número: " + to_string(other->getNumber()));
						}

						other->SetActive(false);
						hasKey = true;
						//sound.play();
					}
					else
					{
						//DebugLog("Has cogido el número: " + to_string(other->getNumber()));
					}
					
				}
			}

			else if (other->get_entity_type() == Entity::Entity_Type::TRIGGER_DOOR)
			{
				if (hasKey)
				{
					if (components.count("physics") > 0)
					{
						components["physics"]->physics_model->setIgnoreCollisionCheck(other->get_physics_model(), true);
						other->set_visible(false);
					}
				}
				else
				{
					//DebugLog("¡Te falta una llave!");
				}
			}
			else if (other->get_entity_type() == Entity::Entity_Type::PLATFORM)
			{
				if (hasKey)
				{
					if (other->components.count("physics") > 0)
					{
						other->components["physics"]->physics_model->setLinearVelocity(btVector3(0, 1, 0));
					}
				}

				set_can_jump_again();
			}
			else if (other->get_entity_type() == Entity::Entity_Type::FLOOR)
			{
				set_can_jump_again();
			}
		}
	}
}
