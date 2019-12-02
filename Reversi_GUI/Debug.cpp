/* デバッグモード */

#pragma warning(disable: 4996)
#include "DxLib.h"
#include "sub.h"

void debug_draw(g_status status, ip_status i_status)
{
	char status1[81], status2[81];
	switch (status.phase)
	{
	case PHASE_INPUT_INIT:
	{
		strcpy(status1, "入力準備");
		break;
	}
	case PHASE_INPUT:
	{
		strcpy(status1, "入力待ち");
		break;
	}
	case PHASE_FLIP:
	{
		strcpy(status1, "裏返し処理");
		break;
	}
	case PHASE_SUCCESSFUL:
	{
		strcpy(status1, "成功処理");
		break;
	}
	case PHASE_FAILURE:
	{
		strcpy(status1, "失敗処理");
		break;
	}
	default:
	{
		strcpy(status1, "未定義");
		break;
	}
	}
	switch (status.GameStatus)
	{
	case GAME_PLAYING:
	{
		strcpy(status2, "ゲーム中");
		break;
	}
	case GAME_END:
	{
		strcpy(status2, "ゲーム終了");
		break;
	}
	default:
	{
		strcpy(status2, "未定義");
		break;
	}
	}

	DrawFormatStringToHandle(0, FONTSIZE * 2, GetColor(0, 255, 0), status.FontData2, "game phase:%s,phase:%s,passflag:%d,player:%d,(%d,%d)(%d,%d)", status2, status1, status.passflag, status.player, status.mouseX, status.mouseY, status.clickX, status.clickY);

	DrawFormatStringToHandle(0, FONTSIZE * 3, GetColor(0, 255, 0), status.FontData2, "black:%d,white:%d", status.komaA, status.komaB);

	DrawFormatStringToHandle(0, FONTSIZE * 4, GetColor(0, 255, 0), status.FontData2, "%d, %.3lf sec,%.3lf sec",status.time_num, status.time_rest[0], status.time_rest[1]);

	DrawFormatStringToHandle(0, FONTSIZE * 5, GetColor(0, 255, 0), status.FontData2, "Black:%.3lf sec,White:%.3lf sec", status.time_used[0], status.time_used[1]);

	DrawFormatStringToHandle(0, FONTSIZE * 6, GetColor(0, 255, 0), status.FontData2, "%d,%d,%d", status.b_mouseon, status.b_clickon, status.b_conf);

	DrawFormatStringToHandle(0, FONTSIZE * 7, GetColor(0, 255, 0), status.FontData2, "put:%d,%d", status.putX, status.putY);

	DrawFormatStringToHandle(0, FONTSIZE * 8, GetColor(0, 255, 0), status.FontData2, "prev:%d,%d", status.prevputX, status.prevputY);

	DrawFormatStringToHandle(0, FONTSIZE * 9, GetColor(0, 255, 0), status.FontData2, "cpuflag:%d", status.ai_flag);

	DrawFormatStringToHandle(0, FONTSIZE * 10, GetColor(0, 255, 0), status.FontData2, "tekazu:%d", status.tekazu);

	DrawFormatStringToHandle(0, FONTSIZE * 11, GetColor(0, 255, 0), status.FontData2, "Com Operation Count:%d", status.com_count);

	DrawFormatStringToHandle(0, FONTSIZE * 12, GetColor(0, 255, 0), status.FontData2, "%02d-%02d-%02d %02d:%02d:%02d", status.StartTimeData.Year, status.StartTimeData.Mon, status.StartTimeData.Day, status.StartTimeData.Hour, status.StartTimeData.Min, status.StartTimeData.Sec);

	if (status.GameStatus == GAME_PLAYING)
	{
		unsigned int color;
		for (int y = 0;y < 8;++y)
		{
			for (int x = 0;x < 8;++x)
			{
				DrawFormatStringToHandle(64 * (x + 6) + 4, 64 * (y + 1) + 4, GetColor(0, 0, 255), status.FontData2, "%d", status.flipmap[y][x]);
				if (status.prevputX == x && status.prevputY == y)
				{
					color = GetColor(255, 255, 0);
				}
				else
				{
					color = GetColor(255, 0, 0);
				}
				if (status.mapscore[y][x] > -INF - 1)
				{
					DrawFormatStringToHandle(64 * (x + 6) + 4, 64 * (y + 1) + 4 + 15, color, status.FontData2, "%d", status.mapscore[y][x]);
				}
			}
		}
	}


	/*
	for (int x = 0;x <= 1280;x += 16)
	{
		for (int y = 0;y <= 720;y += 16)
		{
			DrawPixel(x, y, GetColor(255, 255, 255));
		}
	}
	*/
}