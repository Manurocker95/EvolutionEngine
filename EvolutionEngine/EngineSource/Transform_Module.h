
#pragma once

#ifndef TRANSFORM_MODULE_HPP
#define TRANSFORM_MODULE_HPP

#include <list>
#include "Module.hpp"
#include "Component.hpp"
#include "Task.hpp"

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr

namespace engine
{
	class Scene;
	class Transform_Module : public Module
	{

		////////////////////////////////////////
		////						
		////	TRANSFORM MODULE FACTORY	////
		////
		////////////////////////////////////////

	public:
		class Transform_Module_Factory : public Factory
		{

		public:
			Transform_Module_Factory()
			{
				Module::register_module("transform", this);
			}

		public:
			shared_ptr < Module > create_module() override;
		};

		//////////////////////////////////
		////						
		////	TRANSFORM COMPONENT	  ////
		////
		//////////////////////////////////

		class Transform_Component : public Component
		{
		private:
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;

			glm::mat4 transformation_matrix;

		public:
			Transform_Component(Entity * parent)
				: Component(parent)
				, position(1.f)
				, rotation(1.f)
				, scale(1.f)
				, transformation_matrix(glm::mat4(1.0f))
			{
				//transformation_matrix = glm::mat4(1.0f);
			}
			~Transform_Component() {}

		public:
			bool inicialize()override;
			bool parse_property(const std::string & name, const std::string & value)override;
			void update(Transform_Component * parent);

			float * Get_Model_View_matrix() { return glm::value_ptr(transformation_matrix); }

		};

		////////////////////////////////
		////
		////	TRANSFORM TASK		////
		////
		////////////////////////////////

		class Transform_Task : public Task
		{

			Transform_Module * transform_module;

		public:

			Transform_Task(int priority, Transform_Module * _transform_module) : Task(priority), transform_module(_transform_module)
			{
				int i = 0;
			}

		public:
			bool inicialize() override;
			bool do_step(float time) override; // recorre la lista de render activa material y dibuja mesh
			bool finalize() override;

		private:
			void update_entity(Entity * entity, Transform_Component * parent_transform);


		};


		/////////////////////////////////
		////
		////	TRANSFORM MODULE	////
		////
		////////////////////////////////

	private:
		std::list< shared_ptr< Transform_Component > > transforms;
		Transform_Task task;


	private:
		static Transform_Module_Factory factory;

	public:

		Transform_Module(const int & priority_task)
			: Module()
			, task(priority_task, this)
		{ }

		~Transform_Module() {}

	public:
		Task * get_task() override;
		Component * create_component(Entity * parent)override;
		Entity * get_scene_root();

	};
}
#endif
