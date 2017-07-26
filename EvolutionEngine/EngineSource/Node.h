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
#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include <SFML/OpenGL.hpp>
#include "Shader_Program.hpp"

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr

#include "tiny_obj_loader.h"
#include <memory>


namespace EvolutionEngine
{
	using namespace std;

	class Node
	{
		
	protected:

		glm::mat4 transform;
		glm::mat4 combined_transform;

		Shader_Program shader_program;

		vector <shared_ptr <Node>> children;
		std::string node_name;
		bool loaded;

	public:

		virtual void update(float delta_time)
		{
			for (auto & child : children)
			{
				child->update(delta_time);
			}
		}

		virtual void render(const glm::mat4 & parent_model_view_matrix, GLint model_view_matrix_id)
		{
			combined_transform = parent_model_view_matrix * transform;

			for (auto & child : children) 
			{
				child->render(combined_transform, model_view_matrix_id);
			}
		}

		std::string get_name() { return node_name; }

		template< typename NODE_CLASS >
		void add_child(shared_ptr<NODE_CLASS> & child)
		{
			children.push_back(child);
		}

		void add_child(shared_ptr<Node> & child)
		{
			children.push_back(child);
		}

		vector <shared_ptr <Node>> get_children()
		{
			return children;
		}

		template< typename NODE_CLASS >
		vector <shared_ptr <NODE_CLASS>> get_children()
		{
			return children;
		}

		void set_name(std::string name)
		{
			node_name = name;
		}

		bool loaded_correctly() { return loaded; }

		void translate(const glm::vec3 & displacement)
		{
			transform = glm::translate (transform, displacement);
		}

		void Scale(float scale)
		{
			transform = transform * glm::scale(transform, glm::vec3(scale, scale, scale));
		}

		void Rotate(float x, float y, float z, float angle)
		{
			transform = glm::rotate(transform, angle, glm::vec3(x, y, z));
		}

		void rotate_around_x(const float & angle);

		void rotate_around_y(const float & angle);

		void rotate_around_z(const float & angle);

		
	};
}