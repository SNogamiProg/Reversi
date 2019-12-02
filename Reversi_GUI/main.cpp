#pragma warning(disable: 4996)
#include "DxLib.h"
#include "sub.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) 
{
	SetWindowIconID(101), SetOutApplicationLogValidFlag(false), ChangeWindowMode(TRUE), SetAlwaysRunFlag(TRUE), SetGraphMode(1280, 720, 32), SetMainWindowText("Reversi"), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK), SetMouseDispFlag(TRUE);

	g_status status;
	ip_status i_status;

	if (game_initialize(&status) != 0) return EXIT_FAILURE;

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && status.exitflag == 0)
	{
		mouse_update(&i_status);
		game_update(&status, i_status);
		FPSUpdate();
		game_draw(status, i_status);
		if (status.DebugMode == DEBUG_ON)
		{
			mouse_status_draw(i_status);
		}
		FPSDraw();
		FPSWait();
	}

	game_finalize(&status);

	DxLib_End();
	return EXIT_SUCCESS;
}