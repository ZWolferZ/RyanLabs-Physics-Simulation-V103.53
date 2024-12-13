#include "DX11PhysicsFramework.h"
#include <windows.h>
#include <comdef.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	auto application = DX11PhysicsFramework();
	

	if (FAILED(application.Initialise(hInstance, nCmdShow)))
	{
		// Skill Issue
		return -1;
	}

	

	// Main message loop
	MSG msg = { nullptr };
	Timer _frameTimer;

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			bool handled = false;

			if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
			{
				handled = true;
			}
			else if (WM_QUIT == msg.message)
				break;

			if (!handled)
			{
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		else
		{
			static float accumulator = 0;

			accumulator += _frameTimer.GetDeltaTime();

            #ifdef _DEBUG
			if (accumulator > 1.0f) // assume come back from breakpoint
				accumulator = FPS60;
            #endif
			Debug::Debug_WriteString(std::to_string(accumulator));

			while (accumulator >= FPS60)
			{
				application.Update();
				accumulator -= FPS60;
				_frameTimer.Tick();
			}

			const double alpha = accumulator / FPS60;

			application.Draw(alpha);
		}
	}

	return static_cast<int>(msg.wParam);
}