#include "Kernel.hpp"
#include "SceneManager.h"

namespace EvolutionEngine
{
	void Kernel::execute()
	{
		sf::Clock timer;
		float delta_time = 0.017f; // 60 fps

		for (auto task : tasks)
		{
			task->initialize();
		}

		exit = false;
		paused = false;

		float time = 1.f / 60.f;

		while (!exit)
		{
			timer.restart();

			// Para Middleware habrá un módulo de inputs, de momento se cogen desde el scene manager
			//SceneManager::instance()->CheckInputs();

			for (auto task : tasks)
			{
				if (exit)
				{
					break;
				}
				else if (paused)
				{
					while (paused)
					{
						// sobrecarga de add_taskt con un hilo, para que pueda llamar a resume.
						//Thread::current_thread().sleep(10);
					}
				}

				task->do_step(time);  // Hacer una barra de carga
			}

			SceneManager::instance()->Update(delta_time);

			// Hacemos display del window (deberia llamarse desde el módulo de render, pero de momento se queda aquí)
			SceneManager::instance()->Display();
			
			exit = !SceneManager::instance()->getRunning();
			delta_time = (delta_time + timer.getElapsedTime().asSeconds()) * 0.5f;
		}

		for (auto task : tasks)
		{
			task->finalize();
		}
	}

	void Kernel::add_task(Task & task)
	{
		tasks.insert(&task);
		task.set_kernel(this);
	}
}