#include "EvolutionWindow.h"
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

namespace EvolutionEngine
{

	EvolutionWindow::EvolutionWindow(const std::string & title, int width, int height, bool fullscreen)
	{	
		// Se crea la ventana y el contexto de OpenGL asociado a ella:
		
		window = new sf::Window(sf::VideoMode(width, height),
			title,
			WindowStyle(fullscreen),
			sf::ContextSettings(default_buffer_bit_depth, default_stencil, default_antialising, major_context_version, minor_context_version, sf::ContextSettings::Core)
			);

		assert(window != nullptr);

		// Se determinan las características de OpenGL disponibles en la máquina:

		glbinding::Binding::initialize();

		// Se activa la sincronización vertical:

		window->setVerticalSyncEnabled(true);
	}

	EvolutionWindow::~EvolutionWindow()
	{
		delete window;
	}

	sf::Uint32 EvolutionWindow::WindowStyle(bool fullscreen)
	{
		if (fullscreen)
		{
			return sf::Style::Fullscreen;
		}
		else
		{
			return sf::Style::Default;
		}
	}
}