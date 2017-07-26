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

#include "Physics_Module.h"
#include "EvolutionScene.hpp"

namespace EvolutionEngine
{
	Physics_Module::Physics_Module_Factory Physics_Module::factory;
	
	bool  Physics_Module::Physics_task::initialize() 
	{
		return true;
	}

	bool Physics_Module::Physics_task::finalize() 
	{
		// ...
		return true;
	}

	bool Physics_Module::Physics_task::do_step(float time)
	{
		physics_module->physics_world->stepSimulation(time, 1);
		physics_module->Get_Owner()->Check_Collisions();
		return true;
	}

	Physics_Module::Physics_task::Physics_task(int priority, Physics_Module * _module) : physics_module(_module), Task(priority)
	{
		physics_module->overlappingPairCache.reset(new btDbvtBroadphase());
		physics_module->collisionConfiguration.reset(new btDefaultCollisionConfiguration());
		physics_module->collisionDispatcher.reset(new btCollisionDispatcher(_module->collisionConfiguration.get()));
		physics_module->constraintSolver.reset(new btSequentialImpulseConstraintSolver);

		physics_module->physics_world.reset(new btDiscreteDynamicsWorld(_module->collisionDispatcher.get(), _module->overlappingPairCache.get(), _module->constraintSolver.get(), _module->collisionConfiguration.get()));
		physics_module->physics_world->setGravity(btVector3(0.f, -9.f, 0));
		physics_module->Get_Owner()->setPhysicsWorld(physics_module->physics_world.get());
	}

	Physics_Module::Physics_Component::Physics_Component(std::shared_ptr<Entity> parent) : Component(parent)
	{
		type = "physics";
	}

	void Physics_Module::Physics_Component:: setPhysics(std::shared_ptr<Entity> _parent)
	{
		parent = _parent;
		
		if (body_type == Body_Type::DYNAMIC)
		{
			shape.reset(new btBoxShape(btVector3(parent->getScale()[0], parent->getScale()[1], parent->getScale()[2])));

			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(parent->getPosition()[0], parent->getPosition()[1], parent->getPosition()[2]));
			
			btScalar  mass = 1.f;
			btVector3 localInertia(0, 0, 0);

			shape->calculateLocalInertia(mass, localInertia);

			motion.reset(new btDefaultMotionState(transform));

			infoRigidBody.reset(new btRigidBody::btRigidBodyConstructionInfo(mass, motion.get(), shape.get(), localInertia));

			physics_model.reset(new btRigidBody(*infoRigidBody.get()));

			physics_model->setUserPointer(parent.get());
			physics_model->setAngularFactor(btVector3(0, 0, 0));

			if (parent->get_entity_type() == Entity::Entity_Type::PLATFORM)
			{
				physics_model->setGravity(btVector3(0, 0, 0));
				physics_model->setLinearFactor(btVector3(0, 1, 0));
			}
			
		}
		else if (body_type == Body_Type::STATIC)
		{
			shape.reset(new btBoxShape(btVector3(parent->getScale()[0], parent->getScale()[1], parent->getScale()[2])));
			//DebugLog("Static. x:" + std::to_string(parent->getScale()[0]) + ", y:" + std::to_string(parent->getScale()[1])+ ", y:" + std::to_string(parent->getScale()[2]));
			
			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(btVector3(parent->getPosition()[0], parent->getPosition()[1], parent->getPosition()[2]));

			motion.reset(new btDefaultMotionState(transform));

			infoRigidBody.reset(new btRigidBody::btRigidBodyConstructionInfo(0, motion.get(), shape.get()));

			physics_model.reset(new btRigidBody(*infoRigidBody.get()));
			physics_model->setUserPointer(parent.get());
		}

		body = physics_model;
		owner->getPhysicsWorld()->addRigidBody(physics_model.get());

		if (parent->IsPlayer())
		{
			physics_model->setActivationState(DISABLE_DEACTIVATION);
		}
		else if (parent->IsKey() || parent->IsTrigger())
		{
			parent->set_trigger();
		}
	}

	bool Physics_Module::Physics_Component::initialize()
	{
		return true;
	}
}