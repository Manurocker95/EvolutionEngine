#pragma once

#include <string>
#include <cassert>
#include "SFML/Window.hpp"
#include "Settings.h"
#include "EvolutionWindow.h"

using namespace sf;

namespace EvolutionEngine
{

	class EvolutionWindow
	{
	public:

		enum Fullscreen_Type
		{
			WINDOWED,
			FULLSCREEN
		};

	private:

		Window  * window;

	public:
		EvolutionWindow(const std::string & title = "Evolution Engine", int width = window_width, int height = window_height, bool fullscreen = false);

		~EvolutionWindow();

		sf::Uint32 WindowStyle(bool fullscreen = false);
		void set_fullscreen(Fullscreen_Type type = FULLSCREEN)
		{
			// ...
		}

		void set_windowed()
		{
			// ...
		}

		void set_position(int new_left_x, int new_top_y)
		{
			// ...
		}

		void set_size(int new_width, int new_height)
		{
			// ...
		}


		sf::Window & get_window()
		{
			return *window;
		}

		void display()
		{
			window->display();
		}
	};


}
