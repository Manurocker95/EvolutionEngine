#pragma once
#include "Settings.h"
#include "SceneManager.h"
#include "EvolutionScene.hpp"
#include "EvolutionWindow.h"

#include "SFML/Window.hpp"

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>

using namespace std;
using namespace toolkit;

namespace EvolutionEngine
{
	class EvolutionGame
	{
	public:

		EvolutionGame(int number_of_arguments, const char * args[], bool show_default_scene = false);
		void Play();
	private:
		EvolutionWindow * window;
		bool canRun;
	};
}