#include "EvolutionGame.h"
#include "DebugManager.h"

namespace EvolutionEngine
{ 
	// Arguments: 0 = program, 1 = Scene

	EvolutionGame::EvolutionGame(int number_of_arguments, const char * args[], bool show_default_scene)
	{
		canRun = true;

		// if we don't send 2 arguments, we show an error
		if (number_of_arguments != 2)
		{
			// We show the error and close the window 
			DebugManager::instance()->show_error("Hay que indicar un archivo de escena.", "Error de inicializacion");
			canRun = false;
			return;
		}

		// We take the window name from the path of the scene file
		std::string window_name = args[1];
		size_t offset = window_name.find_last_of('\\') + 1;
		window_name = window_name.substr(offset, (window_name.length()) - offset);

		size_t of = window_name.find_last_of('.') + 1;
		std::string file_extension = window_name.substr(of, (window_name.length()) + of);

		if (file_extension.compare(scene_file_extension) != 0)
		{
			DebugManager::instance()->show_error("No se ha pasado un archivo correcto de escena. ", "Error de inicializacion");
			canRun = false;
			return;
		}

		window = new EvolutionWindow(window_name);

		// We initialize the scene and send it the window reference
		SceneManager::instance()->Start(&window->get_window());

		// Creamos una escena

		if (show_default_scene)
			SceneManager::instance()->CreateDefaultScene();
		else
			SceneManager::instance()->CreateEvolutionScene(args[1]);

		glClearColor(0.1f, 0.1f, 0.1f, 1.f);

		if (!SceneManager::instance()->getRunning())
			canRun = false;
	}

	void EvolutionGame::Play()
	{
		if (!canRun)
			return;
			
		SceneManager::instance()->Play();
	}
	
}