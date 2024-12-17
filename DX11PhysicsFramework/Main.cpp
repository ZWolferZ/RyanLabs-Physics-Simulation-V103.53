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

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
		{
			application.Update();
		}
	}

	return static_cast<int>(msg.wParam);
}