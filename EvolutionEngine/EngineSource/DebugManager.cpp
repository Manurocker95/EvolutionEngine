#include "DebugManager.h"

#include "SceneManager.h"

namespace EvolutionEngine
{
	// Static instance for singleton
	DebugManager* DebugManager::Instance = NULL;

	// Static method to return the singleton instance, if we don't have any instance, we create it
	DebugManager* DebugManager::instance()
	{
		if (!Instance)
			Instance = new DebugManager;

		return Instance;
	}

	// We delete the pointers when the SceneManager is destroyed so we don't have leaks
	DebugManager::~DebugManager()
	{
	
	}

	// I'd have a MessageManager, but I'll set this here now
	// Show Error: Displays a Message with Window's API when an error is produced during the program exec
	// We can also save this error in a txt (Set true in settings.h)
	void DebugManager::show_error(std::string msg, std::string windowname, UINT button, bool savinginlog)
	{
		std::wstring wide_string = std::wstring(windowname.begin(), windowname.end());
		const wchar_t* window_name = wide_string.c_str();

		std::wstring stemp = std::wstring(msg.begin(), msg.end());
		LPCWSTR  error = (LPCWSTR)stemp.c_str();
		MessageBox(NULL, error, window_name, button);

		// If we can save the errorlog
		if (save_errorlog && savinginlog)
		{
			MessageBox(NULL, L"Se va a guardar el error en la carpeta Errorlog", L"Guardando Errorlog", button);
			// If errorlog folder doesn't exists, we create it
			_mkdir(errorLog_Folder);

			// We add the error to the errorlog file. If it doesn't exists, it's created
			std::ofstream outfile;
			outfile.open(std::string(errorLog_Folder) + std::string(errorlog), std::ios_base::app);

			if (outfile.good())
			{
				outfile << msg + "\n";
				outfile.close();
			}
		}
	}

	void DebugManager::Log(std::string msg, std::string windowname, UINT button)
	{
		std::wstring wide_string = std::wstring(windowname.begin(), windowname.end());
		const wchar_t* window_name = wide_string.c_str();

		std::wstring stemp = std::wstring(msg.begin(), msg.end());
		LPCWSTR  error = (LPCWSTR)stemp.c_str();
		MessageBox(NULL, error, window_name, button);		
	}

	// We show an error and close the window.
	void DebugManager::show_error_and_close(std::string msg, std::string windowname, bool savinginlog)
	{
		show_error(msg, windowname, MB_OK, savinginlog);
		SceneManager::instance()->setRunning(false);
	}
}
