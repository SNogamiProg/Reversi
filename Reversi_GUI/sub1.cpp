/* ゲーム処理 */

#pragma warning(disable: 4996)
#include "DxLib.h"
#include <math.h>
#include "sub.h"

/* 初期化 */
int game_initialize(g_status* status)
{
	status->FontFile = AddFontFile("font/ラノベPOP.otf");
	SRand(GetNowCount());
	if ((read_map_file("data/map1.csv", status->map) == 0)) return -1;
	setting_file_load("setting.txt", &status->time_setting, &status->use_timer, &status->matta_setting, &status->matta_enable);
	status->blackImage = LoadGraph("image/koma_b.png");
	status->whiteImage = LoadGraph("image/koma_w.png");
	status->mapImage = LoadGraph("image/map.png");
	status->blackGimage = LoadGraph("image/guide_b.png");
	status->whiteGimage = LoadGraph("image/guide_w.png");
	status->selectedImage = LoadGraph("image/select.png");
	status->clickedImage = LoadGraph("image/click.png");
	status->black_status_area = LoadGraph("image/black_status.png");
	status->white_status_area = LoadGraph("image/white_status.png");
	status->background = LoadGraph("image/background.png");
	status->messagebox = LoadGraph("image/messagebox.png");
	status->blackPutimage = LoadGraph("image/putmark_b.png");
	status->whitePutimage = LoadGraph("image/putmark_w.png");
	status->FontData1 = CreateFontToHandle("07ラノベPOP", FONTSIZE1, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	status->FontData2 = CreateFontToHandle("07ラノベPOP", FONTSIZE2, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	status->loadingImage = LoadGraph("image/button_B.png");	/* 読込中...などを表示 */
	button_init(&status->Button[BUTTON_PASS], 640-64, 360-16, 640+64, 360+16, "image/button_B.png","OK",32);
	button_init_with_icon(&status->Button[BUTTON_RETRY], 640 - 32, 360 - 32, 640 + 32, 360 + 32, "image/button_B.png", "image/Retry.png","設定画面に戻る");
	button_init(&status->Button[BUTTON_CPU_BLACK], 160, 20, 480, 220, "image/button_B.png", "CPU黒", 50);
	button_init(&status->Button[BUTTON_CPU_NONE], 480, 20, 800, 220, "image/button_B.png", "CPUなし", 50);
	button_init(&status->Button[BUTTON_CPU_WHITE], 800, 20, 1120, 220, "image/button_B.png", "CPU白", 50);
	button_init(&status->Button[BUTTON_TIME_NONE], 160, 240, 480, 340, "image/button_B.png", "制限時間なし", 30);
	button_init(&status->Button[BUTTON_TIME_TYPEA], 160, 340, 480, 440, "image/button_B.png", "一手", 30);
	button_init(&status->Button[BUTTON_MATTA_OFF], 160, 460, 480, 520, "image/button_B.png", "待ったなし", 30);
	button_init(&status->Button[BUTTON_MATTA_ON], 160, 520, 480, 580, "image/button_B.png", "待ったあり（制限）", 30);
	button_init(&status->Button[BUTTON_MATTA_INF], 160, 580, 480, 640, "image/button_B.png", "待った無制限", 30);
	button_init(&status->Button[BUTTON_GAME_START], 840, 460, 1120, 640, "image/button_B.png", "OK", 30);
	button_init(&status->Button[BUTTON_TIME_10UP], 480, 240, 800, 260, "image/button_B.png", "+10sec", 20);
	button_init(&status->Button[BUTTON_TIME_1UP], 800, 240, 1120, 260, "image/button_B.png", "+1sec", 20);
	button_init(&status->Button[BUTTON_TIME_10DOWN], 480, 420, 800, 440, "image/button_B.png", "-10sec", 20);
	button_init(&status->Button[BUTTON_TIME_1DOWN], 800, 420, 1120, 440, "image/button_B.png", "-1sec", 20);
	button_init(&status->Button[BUTTON_MATTA_1UP], 480, 460, 820, 480, "image/button_B.png", "+1", 20);
	button_init(&status->Button[BUTTON_MATTA_1DOWN], 480, 620, 820, 640, "image/button_B.png", "-1", 20);
	button_init(&status->Button[BUTTON_GO_GAMESETTING], 500, 360, 780, 400, "image/button_B.png", "はじめから", 20);
	button_init(&status->Button[BUTTON_CONTINUE], 500, 420, 780, 460, "image/button_Oran.png", "つづきから", 20);
	button_init(&status->Button[BUTTON_REPLAY], 500, 480, 780, 520, "image/button_B.png", "リプレイ", 20);
	button_init_with_icon(&status->Button[BUTTON_BACK_SCENE], 20, 620, 100, 700, "image/button_B.png", "image/goback.png","タイトルに戻る");
	button_init_with_icon(&status->Button[BUTTON_PREV_MAP], 120, 620, 200, 700, "image/button_B.png", "image/prev.png", "待った");
	button_init(&status->Button[BUTTON_EXIT], 500, 540, 780, 580, "image/button_B.png", "終了", 20);
	status->FontData3 = CreateFontToHandle("07ラノベPOP", FONTSIZE3, 3, DX_FONTTYPE_ANTIALIASING_8X8);
	status->FontData4 = CreateFontToHandle("07ラノベPOP", FONTSIZE4, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	status->sound1 = LoadSoundMem("sound/nyu1.wav");
	status->sound2 = LoadSoundMem("sound/nyu2.wav");
	status->sound3 = LoadSoundMem("sound/gong-played1.wav");
	status->sound4 = LoadSoundMem("sound/gong-played2.wav");
	status->sound5 = LoadSoundMem("sound/nyu3.wav");
	status->sound6 = LoadSoundMem("sound/warning2.wav");
	for (int i = 0;i < 2;++i)
	{
		status->time_rest[i] = status->time_max;
	}
	return 0;
}

void button_init(
	but_status* button,
	int x1, 
	int y1, 
	int x2, 
	int y2, 
	const char* image, 
	const char* text, 
	int fontsize
)
{
	button->x1 = x1;
	button->y1 = y1;
	button->x2 = x2;
	button->y2 = y2;
	button->img = LoadGraph(image);
	button->img_disable = MakeScreen(64, 64, TRUE);
	GraphFilterBlt(button->img, button->img_disable, DX_GRAPH_FILTER_MONO, 0, 0);
	strcpy(button->text, text);
	button->font = CreateFontToHandle("07ラノベPOP", fontsize, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	button->icon = FALSE;
}

void button_init_with_icon(but_status* button, int x1, int y1, int x2, int y2, const char* image, const char* icon, const char* text)
{
	button->x1 = x1;
	button->y1 = y1;
	button->x2 = x2;
	button->y2 = y2;
	button->img = LoadGraph(image);
	button->img_disable = MakeScreen(64, 64, TRUE);
	GraphFilterBlt(button->img, button->img_disable, DX_GRAPH_FILTER_MONO, 0, 0);
	button->button_icon = LoadGraph(icon);
	button->icon = TRUE;
	button->font = CreateFontToHandle("07ラノベPOP", 16, 2, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	strcpy(button->text, text);
}

int point_map(g_status* status, ip_status i_status)
{
	if ((i_status.MouseX >= 64 * 6) &&
		(i_status.MouseX < 64 * 14) &&
		(i_status.MouseY >= 64 * 1) &&
		(i_status.MouseY < 64 * 9))
	{
		for (int y = 0;y < 8;++y)
		{
			for (int x = 0;x < 8;++x)
			{
				if ((i_status.MouseX >= 64 * (x + 6)) &&
					(i_status.MouseX < 64 * (x + 7)) &&
					(i_status.MouseY >= 64 * (y + 1)) &&
					(i_status.MouseY < 64 * (y + 2)))
				{
					status->mouseonflag = MOUSE_ON;
					status->mouseX = x;
					status->mouseY = y;
					if ((status->clickX == -1) &&
						(status->clickY == -1) &&
						(i_status.click == 1) &&
						(status->clickflag == CLICK_NOTCLICK))	/* 枠内でクリック */
					{
						status->clickflag = CLICK_CLICKING;
						status->clickX = status->mouseX;
						status->clickY = status->mouseY;
					}
					if ((i_status.click == 0) &&
						(status->clickflag == CLICK_CLICKING)&&
						(status->clickX == -1) &&
						(status->clickY == -1))	/* 枠内でクリック */
					{
						status->clickflag = CLICK_NOTCLICK;
						status->clickX = -1;
						status->clickY = -1;
					}
				}
			}
		}
	}
	else
	{
		status->mouseonflag = MOUSE_NOTON;
		status->mouseX = -1;
		status->mouseY = -1;
	}
	if ((status->mouseX != status->clickX) &&
		(status->mouseY != status->clickY) &&
		(status->clickflag == CLICK_CLICKING) &&
		(i_status.click == 0))	/* クリック離す */
	{
		status->clickflag = CLICK_NOTCLICK;
		status->clickX = -1;
		status->clickY = -1;
	}
	if ((status->clickX == status->mouseX) &&
		(status->clickY == status->mouseY) &&
		(status->clickX >= 0) &&
		(status->clickY >= 0) &&
		(i_status.click == 0) &&
		(status->clickflag == CLICK_CLICKING))
	{
		status->putX = status->clickX;
		status->putY = status->clickY;
		status->clickX = -1;
		status->clickY = -1;
		status->clickflag = CLICK_NOTCLICK;
		return 1;
	}
	return 0;
}

int point_button(g_status* status, ip_status i_status)
{
	int flagon = 0;
	int flagclick = -1;

	status->b_conf = -1;
	for (int i = 0;i < BUTTON_NUM;++i)
	{
		if (status->Button[i].button_flag == BUTTON_ENABLE)
		{
			if ((i_status.MouseX >= status->Button[i].x1) &&
				(i_status.MouseX < status->Button[i].x2) &&
				(i_status.MouseY >= status->Button[i].y1) &&
				(i_status.MouseY < status->Button[i].y2))
			{
				status->mouseonflag = MOUSE_ON;
				status->b_mouseon = i;
				flagon = 1;
				if ((status->b_mouseon >= 0) &&
					(status->b_clickon == -1) &&
					(i_status.click == 1))
				{
					status->clickflag = CLICK_CLICKING;
					status->b_clickon = status->b_mouseon;
				}
				else
				{
					if ((status->b_mouseon != status->b_clickon) &&
						(status->clickflag == CLICK_CLICKING) &&
						(i_status.click == 0))
					{
						status->clickflag = CLICK_NOTCLICK;
						status->b_clickon = -1;
					}
				}
			}
			else
			{
				status->mouseonflag = MOUSE_NOTON;
			}
			if ((status->b_clickon == status->b_mouseon) &&
				(status->b_clickon >= 0) &&
				(i_status.click == 0) &&
				(status->clickflag == CLICK_CLICKING))
			{
				status->b_conf = status->b_clickon;
				status->b_clickon = -1;
				status->b_mouseon = -1;
				status->clickflag = CLICK_NOTCLICK;
				PlaySoundMem(status->sound5, DX_PLAYTYPE_BACK, TRUE);
				return status->b_conf;
			}
		}
	}
	if (flagon == 0 || status->Button[status->b_mouseon].button_flag == BUTTON_DISABLE || status->Button[status->b_mouseon].button_flag == BUTTON_INVISIBLE)
	{
		status->b_mouseon = -1;
	}
	return -1;
}

/* 計算 */
void game_update(g_status* status, ip_status i_status)
{
	switch (status->GameStatus)
	{
	case GAME_TITLE:
	{
		switch (status->phase)
		{
		case PHASE_TITLE_INIT:
		{
			status->Button[BUTTON_GO_GAMESETTING].button_flag =
				status->Button[BUTTON_EXIT].button_flag =
				BUTTON_ENABLE;
			status->continueflag = 0;
			FILE* fp = fopen("backup.bac","r");
			if (fp == NULL)
			{
				status->Button[BUTTON_CONTINUE].button_flag =
					BUTTON_DISABLE;
			}
			else
			{
				status->Button[BUTTON_CONTINUE].button_flag =
					BUTTON_ENABLE;
				fclose(fp);
			}

			status->Button[BUTTON_REPLAY].button_flag =
				BUTTON_DISABLE;

			status->phase = PHASE_TITLE;
			break;
		}
		case PHASE_TITLE:
		{
			switch (point_button(status, i_status))
			{
			case BUTTON_GO_GAMESETTING:
			{
				status->Button[BUTTON_GO_GAMESETTING].button_flag =
					status->Button[BUTTON_CONTINUE].button_flag =
					status->Button[BUTTON_REPLAY].button_flag =
					status->Button[BUTTON_EXIT].button_flag =
					BUTTON_INVISIBLE;

				status->GameStatus = GAME_SETTING;
				status->phase = PHASE_SETTING_INIT;
				break;
			}
			case BUTTON_CONTINUE:
			{
				status->Button[BUTTON_GO_GAMESETTING].button_flag =
					status->Button[BUTTON_CONTINUE].button_flag =
					status->Button[BUTTON_REPLAY].button_flag =
					status->Button[BUTTON_EXIT].button_flag =
					BUTTON_INVISIBLE;

				status->continueflag = 1;
				status->GameStatus = GAME_INITIALIZE;
				status->phase = PHASE_INPUT_INIT;
				break;
			}
			case BUTTON_EXIT:
			{
				status->exitflag = 1;
				break;
			}
			}
			break;
		}
		}
		break;
	}
	case GAME_SETTING:
	{
		switch (status->phase)
		{
		case PHASE_SETTING_INIT:
		{
			status->Button[BUTTON_CPU_BLACK].button_flag =
				status->Button[BUTTON_CPU_NONE].button_flag =
				status->Button[BUTTON_CPU_WHITE].button_flag =
				status->Button[BUTTON_TIME_NONE].button_flag =
				status->Button[BUTTON_TIME_TYPEA].button_flag =
				status->Button[BUTTON_MATTA_OFF].button_flag =
				status->Button[BUTTON_MATTA_ON].button_flag =
				status->Button[BUTTON_MATTA_INF].button_flag =
				status->Button[BUTTON_GAME_START].button_flag =
				status->Button[BUTTON_TIME_10UP].button_flag =
				status->Button[BUTTON_TIME_1UP].button_flag =
				status->Button[BUTTON_TIME_10DOWN].button_flag =
				status->Button[BUTTON_TIME_1DOWN].button_flag =
				status->Button[BUTTON_MATTA_1UP].button_flag =
				status->Button[BUTTON_MATTA_1DOWN].button_flag =
				status->Button[BUTTON_BACK_SCENE].button_flag =
				status->Button[BUTTON_MATTA_ON].button_flag =
				status->Button[BUTTON_MATTA_INF].button_flag =
				status->Button[BUTTON_MATTA_1UP].button_flag =
				status->Button[BUTTON_MATTA_1DOWN].button_flag =
				BUTTON_ENABLE;

				status->phase = PHASE_SETTING;
			break;
		}
		case PHASE_SETTING:
		{
			if (status->use_timer == -1)
			{
				status->Button[BUTTON_TIME_10UP].button_flag =
					status->Button[BUTTON_TIME_1UP].button_flag =
					status->Button[BUTTON_TIME_10DOWN].button_flag =
					status->Button[BUTTON_TIME_1DOWN].button_flag =
					BUTTON_DISABLE;
			}
			else if (status->use_timer == 0)
			{
				status->Button[BUTTON_TIME_10UP].button_flag =
					status->Button[BUTTON_TIME_1UP].button_flag =
					status->Button[BUTTON_TIME_10DOWN].button_flag =
					status->Button[BUTTON_TIME_1DOWN].button_flag =
					BUTTON_ENABLE;
			}
			if (status->matta_enable == 0)
			{
				status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					BUTTON_ENABLE;
			}
			else
			{
				status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					BUTTON_DISABLE;
			}

			switch (point_button(status, i_status))
			{
			case BUTTON_CPU_BLACK:
			{
				status->cpu = 1;
				break;
			}
			case BUTTON_CPU_NONE:
			{
				status->cpu = 0;
				break;
			}
			case BUTTON_CPU_WHITE:
			{
				status->cpu = -1;
				break;
			}
			case BUTTON_TIME_NONE:
			{
				status->use_timer = -1;
				break;
			}
			case BUTTON_TIME_TYPEA:
			{
				status->use_timer = 0;
				break;
			}
			case BUTTON_TIME_1UP:
			{
				status->time_setting++;
				if (status->time_setting > status->time_max_setting)
				{
					status->time_setting = status->time_max_setting;
				}
				break;
			}
			case BUTTON_TIME_1DOWN:
			{
				status->time_setting--;
				if (status->time_setting < status->time_min_setting)
				{
					status->time_setting = status->time_min_setting;
				}
				break;
			}
			case BUTTON_TIME_10UP:
			{
				status->time_setting += 10;
				if (status->time_setting > status->time_max_setting)
				{
					status->time_setting = status->time_max_setting;
				}
				break;
			}
			case BUTTON_TIME_10DOWN:
			{
				status->time_setting -= 10;
				if (status->time_setting < status->time_min_setting)
				{
					status->time_setting = status->time_min_setting;
				}
				break;
			}
			case BUTTON_MATTA_OFF:
			{
				status->matta_enable = -1;
				status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					BUTTON_DISABLE;
				break;
			}
			case BUTTON_MATTA_ON:
			{
				status->matta_enable = 0;
				status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					BUTTON_ENABLE;
				break;
			}
			case BUTTON_MATTA_INF:
			{
				status->matta_enable = 1;
				status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					BUTTON_DISABLE;
				break;
			}
			case BUTTON_MATTA_1UP:
			{
				status->matta_setting++;
				if (status->matta_setting > status->matta_max_setting)
				{
					status->matta_setting = status->matta_max_setting;
				}
				break;
			}
			case BUTTON_MATTA_1DOWN:
			{
				status->matta_setting--;
				if (status->matta_setting < status->matta_min_setting)
				{
					status->matta_setting = status->matta_min_setting;
				}
				break;
			}
			case BUTTON_GAME_START:
			{
				status->Button[BUTTON_CPU_BLACK].button_flag =
					status->Button[BUTTON_CPU_NONE].button_flag =
					status->Button[BUTTON_CPU_WHITE].button_flag =
					status->Button[BUTTON_TIME_NONE].button_flag =
					status->Button[BUTTON_TIME_TYPEA].button_flag =
					status->Button[BUTTON_MATTA_OFF].button_flag =
					status->Button[BUTTON_MATTA_ON].button_flag =
					status->Button[BUTTON_MATTA_INF].button_flag =
					status->Button[BUTTON_GAME_START].button_flag =
					status->Button[BUTTON_TIME_10UP].button_flag =
					status->Button[BUTTON_TIME_1UP].button_flag =
					status->Button[BUTTON_TIME_10DOWN].button_flag =
					status->Button[BUTTON_TIME_1DOWN].button_flag =
					status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					status->Button[BUTTON_BACK_SCENE].button_flag =
					BUTTON_INVISIBLE;
				setting_file_save("setting.txt", &status->time_setting, &status->use_timer, &status->matta_setting, &status->matta_enable);
				status->GameStatus = GAME_INITIALIZE;
				status->phase = PHASE_INPUT_INIT;
				if (status->use_timer != -1)
				{
					status->time_max = status->time_setting;
				}
				break;
			}
			case BUTTON_BACK_SCENE:
			{
				status->Button[BUTTON_CPU_BLACK].button_flag =
					status->Button[BUTTON_CPU_NONE].button_flag =
					status->Button[BUTTON_CPU_WHITE].button_flag =
					status->Button[BUTTON_TIME_NONE].button_flag =
					status->Button[BUTTON_TIME_TYPEA].button_flag =
					status->Button[BUTTON_MATTA_OFF].button_flag =
					status->Button[BUTTON_MATTA_ON].button_flag =
					status->Button[BUTTON_MATTA_INF].button_flag =
					status->Button[BUTTON_GAME_START].button_flag =
					status->Button[BUTTON_TIME_10UP].button_flag =
					status->Button[BUTTON_TIME_1UP].button_flag =
					status->Button[BUTTON_TIME_10DOWN].button_flag =
					status->Button[BUTTON_TIME_1DOWN].button_flag =
					status->Button[BUTTON_MATTA_1UP].button_flag =
					status->Button[BUTTON_MATTA_1DOWN].button_flag =
					status->Button[BUTTON_BACK_SCENE].button_flag =
					BUTTON_INVISIBLE;
				status->GameStatus = GAME_TITLE;
				status->phase = PHASE_TITLE_INIT;
			}
			}
			break;
		}
		}
		break;
	}
	case GAME_INITIALIZE:
	{
		for (int j = 0;j < 8;++j)
		{
			for (int i = 0;i < 8;++i)
			{
				status->kakutei[j][i] = 0;
			}
		}
		SRand(GetNowCount());
		status->tekazu = 0;
		status->prevputX = -1;
		status->prevputY = -1;
		status->ai_flag = 0;
		status->player = 1;
		if (status->matta_enable != -1)
		{
			status->matta_rest = status->matta_setting;
		}
		else
		{
			status->matta_rest = 0;
		}
		GetDateTime(&status->StartTimeData);
		read_map_file("data/map1.csv", status->map);
		for (int j = 0; j < 8; ++j)
		{
			for (int i = 0; i < 8; ++i)
			{
				status->startmap[j][i] = status->map[j][i];
				status->mapscore[j][i] = -INF - 1;
			}
		}
		read_aimap_file("data/aimap.csv", status->aimap);
		for (int i = 0; i < 2; ++i)
		{
			status->time_used_prev[i] = 0.0;
			for (int j = 0; j < 3; ++j)
			{
				status->time_used_total[i][j] = 0;
			}
		}

		if (status->continueflag == 1)
		{
			game_backup_load
			(
				"backup.bac",
				status->startmap,
				status->map,
				status->history,
				status->timehistory,
				&status->cpu,
				&status->tekazu,
				&status->time_setting,
				&status->use_timer,
				&status->matta_setting,
				&status->matta_enable,
				&status->player,
				&status->StartTimeData,
				&status->matta_rest
				);
			if (backup_check(status->startmap, status->map, status->history, status->tekazu) != 0)
			{
				remove("backup.bac");
				status->GameStatus = GAME_TITLE;
				status->phase = PHASE_TITLE_INIT;
			}
			for (int count = 0; count < status->tekazu; count++)
			{
				if (status->history[count][0] == 1)
				{
					status->time_used_total[0][1] += (int)status->timehistory[count];
					status->time_used_total[0][2] += (int)(status->timehistory[count] * 1000) % 1000;
					status->time_used_prev[0] = status->timehistory[count];
					operate_time(&status->time_used_total[0][0], &status->time_used_total[0][1], &status->time_used_total[0][2]);
				}
				if (status->history[count][0] == -1)
				{
					status->time_used_total[1][1] += (int)status->timehistory[count];
					status->time_used_total[1][2] += (int)(status->timehistory[count] * 1000) % 1000;
					operate_time(&status->time_used_total[1][0], &status->time_used_total[1][1], &status->time_used_total[1][2]);
					status->time_used_prev[1] = status->timehistory[count];
				}
			}
			if (status->tekazu > 0)
			{
				status->prevputX = status->history[status->tekazu - 1][1];
				status->prevputY = status->history[status->tekazu - 1][2];
			}
		}
		status->GameStatus = GAME_PLAYING;
		status->phase = PHASE_INPUT_INIT;
		status->cannotputflag = PUT_SUCCESSFUL;
		status->init_ok_flag = true;
		for (int i = 0; i < 2; ++i)
		{
			status->time_rest[i] = status->time_max;
			status->time_used[i] = 0.0;
		}
		break;
	}
	case GAME_PLAYING:
	{
		switch (status->phase)
		{
		case PHASE_INPUT_INIT:
		{
			status->Button[BUTTON_BACK_SCENE].button_flag =
				BUTTON_ENABLE;

			if (status->matta_enable == 1)
			{
				sprintf(status->Button[BUTTON_PREV_MAP].text, "待った");

				status->Button[BUTTON_PREV_MAP].button_flag =
					BUTTON_ENABLE;
			}
			else
			{
				if (status->matta_rest > 0)
				{
					sprintf(status->Button[BUTTON_PREV_MAP].text, "待った 残り%d回使用可能", status->matta_rest);

					status->Button[BUTTON_PREV_MAP].button_flag =
						BUTTON_ENABLE;
				}
				else
				{
					strcpy(status->Button[BUTTON_PREV_MAP].text, "待った 使用不可能");

					status->Button[BUTTON_PREV_MAP].button_flag =
						BUTTON_DISABLE;
				}
			}
			if (status->tekazu <= 0 || (status->tekazu == 1 && status->history[0][0] == status->cpu))
			{
				strcpy(status->Button[BUTTON_PREV_MAP].text, "待った 使用不可能");

				status->Button[BUTTON_PREV_MAP].button_flag =
					BUTTON_DISABLE;
			}

			count_koma(&status->komaA, &status->komaB, status->map);
			if (all_can_put_koma(status->player, status->map, status->flipmap) > 0)
			{
				switch (status->player)
				{
				case BLACK:
				{
					status->time_num = 0;
					break;
				}
				case WHITE:
				{
					status->time_num = 1;
					break;
				}
				}
				if (status->cannotputflag == PUT_SUCCESSFUL)
				{
					status->time_rest[status->time_num] = status->time_max;
					status->time_used[status->time_num] = 0.0;
					status->StartTime = GetNowCount();
				}
				status->phase = PHASE_INPUT;
			}
			else
			{
				if (status->passflag == 1)
				{
					status->cannotputflag = PUT_PASS;
					if (status->Button[BUTTON_PASS].button_flag != BUTTON_ENABLE)
					{
						status->Button[BUTTON_PASS].button_flag = BUTTON_ENABLE;
					}
					if (status->soundflag == 1)
					{
						PlaySoundMem(status->sound3, DX_PLAYTYPE_BACK, TRUE);
						status->soundflag = 0;
					}
					status->Button[BUTTON_BACK_SCENE].button_flag =
						status->Button[BUTTON_PREV_MAP].button_flag =
						BUTTON_DISABLE;
					if (point_button(status, i_status) == BUTTON_PASS)
					{
						status->player *= (-1);
						status->passflag = 0;
						status->time_rest[status->time_num] = status->time_max;
						status->phase = PHASE_INPUT_INIT;
						status->Button[BUTTON_PASS].button_flag = BUTTON_INVISIBLE;
						status->Button[BUTTON_BACK_SCENE].button_flag =
							status->Button[BUTTON_PREV_MAP].button_flag =
							BUTTON_ENABLE;
						status->soundflag = 1;
					}
				}
				else
				{
					count_koma(&status->komaA, &status->komaB, status->map);
					if (status->komaA > status->komaB)
					{
						status->Winner = BLACK;
					}
					else if (status->komaB > status->komaA)
					{
						status->Winner = WHITE;
					}
					else
					{
						status->Winner = 0;
					}
					status->GameStatus = GAME_END;
				}
			}
			break;
		}
		case PHASE_INPUT:
		{
			switch (point_button(status,i_status))
			{
			case BUTTON_BACK_SCENE:
			{
				status->Button[BUTTON_BACK_SCENE].button_flag =
					status->Button[BUTTON_PREV_MAP].button_flag =
					BUTTON_INVISIBLE;
				int tmp_tekazu = status->tekazu - 1;
				game_backup(
					"backup.bac",
					status->startmap,
					status->map,
					status->history,
					status->timehistory,
					&status->cpu,
					&tmp_tekazu,
					&status->time_setting,
					&status->use_timer,
					&status->matta_setting,
					&status->matta_enable,
					&status->StartTimeData,
					&status->matta_rest
				);
				status->GameStatus = GAME_TITLE;
				status->phase = PHASE_TITLE_INIT;
				break;
			}
			case BUTTON_PREV_MAP:
			{
				Matta(&status->player, status->cpu, status->history, status->timehistory, status->map, status->flipmap, &status->tekazu, &status->matta_rest, status->time_used_prev, status->time_used_total);
				if (status->matta_enable == 1)
				{
					status->matta_rest = status->matta_setting;
				}
				if (status->tekazu > 0)
				{
					status->prevputX = status->history[status->tekazu - 1][1];
					status->prevputY = status->history[status->tekazu - 1][2];
				}
				status->phase = PHASE_INPUT_INIT;
				status->cannotputflag = PUT_SUCCESSFUL;
				break;
			}
			}
			if ((status->time_rest[status->time_num] >= 0) || (status->use_timer == -1))
			{
				if (status->player == status->cpu)
				{
					status->com_count = 0;
					if (ai_put_koma(status->cpu, &status->putX, &status->putY, status->map, status->flipmap, &status->ai_flag, status->tekazu, status->aimap, 0, &status->com_count, status->mapscore) == 0)
					{
						status->phase = PHASE_FLIP;
					}
				}
				else
				{
					if (point_map(status, i_status) == 1)
					{
						status->phase = PHASE_FLIP;
					}
				}
				double used_time = (double)(GetNowCount() - (double)status->StartTime) / 1000.0;
				status->time_rest[status->time_num] -= used_time;
				status->time_used[status->time_num] += used_time;
				status->StartTime = GetNowCount();
				break;
			}
			else
			{
				status->TimeupFlag = 1;
				switch (status->player)
				{
				case BLACK:
				{
					status->Winner = WHITE;
					break;
				}
				case WHITE:
				{
					status->Winner = BLACK;
					break;
				}
				}
				status->GameStatus = GAME_END;
				break;
			}
		}
		case PHASE_FLIP:
		{
			if (status->putX >= 0 && status->putY >= 0)
			{
				if (status->flipmap[status->putY][status->putX] > 0)
				{
					flip_koma(status->player, status->map, status->flipdir, status->putX, status->putY);
					status->time_used_prev[(status->time_num)] = (double)status->time_used[(status->time_num)];
					status->time_used_total[status->time_num][2] += (int)(status->time_used[(status->time_num)] * 1000);
					operate_time(
						&status->time_used_total[status->time_num][0],
						&status->time_used_total[status->time_num][1],
						&status->time_used_total[status->time_num][2]
					);
					status->time_used[(status->time_num)] = 0.0;
					status->passflag = 1;

					status->history[status->tekazu][0] = status->player;
					status->history[status->tekazu][1] = status->putX;
					status->history[status->tekazu][2] = status->putY;

					for (int i = 0; i < 8; ++i)
					{
						status->history[status->tekazu][i + 3] = status->flipdir[i];
					}
					status->timehistory[status->tekazu] = status->time_used_prev[(status->time_num)];

					game_backup(
						"backup.bac",
						status->startmap,
						status->map,
						status->history,
						status->timehistory,
						&status->cpu,
						&status->tekazu,
						&status->time_setting,
						&status->use_timer,
						&status->matta_setting,
						&status->matta_enable,
						&status->StartTimeData,
						&status->matta_rest
					);

					status->cannotputflag = PUT_SUCCESSFUL;
					status->phase = PHASE_SUCCESSFUL;
					break;
				}
				else
				{
					status->cannotputflag = PUT_FAILURE;
					status->phase = PHASE_FAILURE;
					break;
				}
			}
			else
			{
				status->cannotputflag = PUT_FAILURE;
				status->phase = PHASE_FAILURE;
				break;
			}
		}
		case PHASE_SUCCESSFUL:
		{
			status->prevputX = status->putX;
			status->prevputY = status->putY;
			StopSoundMem(status->sound6);
			if (status->cpu == status->player)
			{
				PlaySoundMem(status->sound2, DX_PLAYTYPE_BACK, TRUE);
			}
			else
			{
				PlaySoundMem(status->sound1, DX_PLAYTYPE_BACK, TRUE);
			}
			status->player *= (-1);
			status->tekazu++;
			status->phase = PHASE_INPUT_INIT;
			break;
		}
		case PHASE_FAILURE:
		{
			status->phase = PHASE_INPUT_INIT;
			break;
		}
		}
		break;
	}
	case GAME_END:
	{
		status->Button[BUTTON_BACK_SCENE].button_flag =
			status->Button[BUTTON_PREV_MAP].button_flag =
			BUTTON_INVISIBLE;
		if (status->soundflag == 1)
		{
			PlaySoundMem(status->sound4, DX_PLAYTYPE_BACK, TRUE);
			status->soundflag = 0;
		}
		remove("backup.bac");
		status->mouseX = -1;
		status->mouseY = -1;
		status->clickX = -1;
		status->clickY = -1;
		status->putX = -1;
		status->putY = -1;
		status->Button[BUTTON_RETRY].button_flag = BUTTON_ENABLE;
		if (point_button(status, i_status) == BUTTON_RETRY)
		{
			status->soundflag = 1;
			status->GameStatus = GAME_SETTING;
			status->phase = PHASE_INPUT_INIT;
			status->Button[BUTTON_RETRY].button_flag = BUTTON_INVISIBLE;
			status->init_ok_flag = false;
		}
		break;
	}
	}
}

void operate_time(int* minute, int* second, int* m_second)
{
	*second += *m_second / 1000;
	if (*m_second < 0)
	{
		*m_second += 1000;
		(*second)--;
	}
	*m_second %= 1000;
	*minute += *second / 60;
	if (*second < 0)
	{
		*second += 60;
		(*minute)--;
	}
	*second %= 60;
}

/* 描画 */
void game_draw(g_status status, ip_status i_status)
{
	int fontwidth;
	unsigned int color[2] = { GetColor(255,255,255),GetColor(255,255,0) };
	unsigned int time_color[2] = { GetColor(255,255,255),GetColor(255,255,0) };
	int blackturn=0, whiteturn=0;
	int playernum;
	char message[BUTTON_NUM][100] = {
	"未定義",
	"設定画面に戻ります" ,
	"CPUを黒に設定します。" ,
	"CPUを白に設定します。",
	"CPUを使用しません。" ,
	"制限時間を設定しません。",
	"一手の制限時間を設定します(10～60秒)。時間切れになると負けです。",
	"時間を10秒増やします。",
	"時間を1秒増やします。",
	"時間を10秒減らします。",
	"時間を1秒減らします。",
	"待ったを無効にします。",
	"待ったを指定回数有効にします。",
	"待ったを無制限で有効にします。",
	"待ったの使用可能回数を1回増やします。",
	"待ったの使用可能回数を1回減らします。",
	"ゲームを開始します。",
	"ゲーム設定画面に移ります",
	"ゲームを続きからプレイします。",
	"リプレイを閲覧します。",
	"タイトル画面に戻ります。",
	"終了します。"
	};

	DrawGraph(0, 0, status.background, TRUE);

	if (status.GameStatus == GAME_TITLE)
	{
		int titlewidth, fontwidth2;
		titlewidth = GetDrawFormatStringWidthToHandle(status.FontData1, "Reversi");
		DrawFormatStringToHandle(640 - titlewidth / 2, 180 - FONTSIZE1 / 2, GetColor(255, 255, 255), status.FontData1, "Reversi");
		fontwidth2 = GetDrawFormatStringWidthToHandle(status.FontData2, "Version %s", GAMEVERSION);
		DrawFormatStringToHandle(1280 - fontwidth2, 720 - FONTSIZE2, GetColor(255, 255, 0), status.FontData2, "Version %s", GAMEVERSION);
	}

	if ((status.GameStatus != GAME_SETTING) && (status.GameStatus != GAME_TITLE))
	{
		DrawGraph(0, 360 - 165, status.black_status_area, TRUE);
		DrawGraph(1280 - 325, 360 - 165, status.white_status_area, TRUE);
		for (int y = 0;y < 8;++y)
		{
			for (int x = 0;x < 8;++x)
			{
				DrawGraph(64 * (x + 6), 64 * (y + 1), status.mapImage, TRUE);
				if (status.init_ok_flag == true)
				{
					if (status.map[y][x] == 1)
					{
						DrawGraph(64 * (x + 6), 64 * (y + 1), status.blackImage, TRUE);
						if ((status.prevputX == x) && (status.prevputY == y))
						{
							DrawGraph(64 * (x + 6), 64 * (y + 1), status.blackPutimage, TRUE);
						}
					}
					else if (status.map[y][x] == -1)
					{
						DrawGraph(64 * (x + 6), 64 * (y + 1), status.whiteImage, TRUE);
						if ((status.prevputX == x) && (status.prevputY == y))
						{
							DrawGraph(64 * (x + 6), 64 * (y + 1), status.whitePutimage, TRUE);
						}
					}
					if ((status.map[y][x] == 0) && (status.flipmap[y][x] > 0))
					{
						switch (status.player)
						{
						case 1:
						{
							DrawGraph(64 * (x + 6), 64 * (y + 1), status.blackGimage, TRUE);
							break;
						}
						case -1:
						{
							DrawGraph(64 * (x + 6), 64 * (y + 1), status.whiteGimage, TRUE);
							break;
						}
						}
					}
				}
				if (status.GameStatus == GAME_PLAYING)
				{
					if ((status.clickX == x) && (status.clickY == y) && (i_status.click == 1))
					{
						DrawExtendGraph(64 * (x + 6), 64 * (y + 1), 64 * (x + 7), 64 * (y + 2), status.clickedImage, TRUE);
					}
					else if ((status.mouseX == x) && (status.mouseY == y) && (i_status.click == 0))
					{
						DrawExtendGraph(64 * (x + 6) + 4, 64 * (y + 1) + 4, 64 * (x + 7) - 4, 64 * (y + 2) - 4, status.selectedImage, TRUE);
					}
				}
			}
			DrawExtendGraph(384 - 2, 608 - 2, 896 + 2, 688 + 2, status.messagebox, TRUE);
			int count_msg = 0;
			char turn[10];
			if (status.GameStatus == GAME_PLAYING)
			{
				switch (status.player)
				{
				case BLACK:
				{
					strcpy(turn, "黒");
					break;
				}
				case WHITE:
				{
					strcpy(turn, "白");
					break;
				}
				}
				switch (status.cannotputflag)
				{
				case PUT_PASS:
				{
					fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "置ける場所がないためパスしました。");
					DrawFormatStringToHandle(640 - fontwidth / 2, 608 + (16 * count_msg), GetColor(255, 0, 0), status.FontData2, "置ける場所がないためパスしました。");
					count_msg++;
					break;
				}
				case PUT_FAILURE:
				{
					fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "その場所には置けません。");
					DrawFormatStringToHandle(640 - fontwidth / 2, 608 + (16 * count_msg), GetColor(255, 0, 0), status.FontData2, "その場所には置けません。");
					count_msg++;
					break;
				}
				}
				fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "%sの番です。", turn);
				DrawFormatStringToHandle(640 - fontwidth / 2, 608 + (16 * count_msg), GetColor(255, 255, 255), status.FontData2, "%sの番です。", turn);
				count_msg++;
			}
			else if (status.GameStatus == GAME_END)
			{
				if (status.TimeupFlag == 1)
				{
					fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "時間切れです。");
					DrawFormatStringToHandle(640 - fontwidth / 2, 608 + (16 * count_msg), GetColor(255, 0, 0), status.FontData2, "時間切れです。");
					count_msg++;
				}
				switch (status.Winner)
				{
				case BLACK:
				{
					strcpy(turn, "黒");
					break;
				}
				case WHITE:
				{
					strcpy(turn, "白");
					break;
				}
				}
				if ((status.Winner == BLACK) || (status.Winner == WHITE))
				{
					fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "%sの勝利。", turn);
					DrawFormatStringToHandle(640 - fontwidth / 2, 608 + (16 * count_msg), GetColor(255, 255, 255), status.FontData2, "%sの勝利。", turn);
				}
				else
				{
					fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "引き分け。");
					DrawFormatStringToHandle(640 - fontwidth / 2, 608 + (16 * count_msg), GetColor(255, 255, 255), status.FontData2, "引き分け。");
				}
				count_msg++;

			}
		}
	}
	if ((status.GameStatus == GAME_INITIALIZE) && (status.init_ok_flag == false))
	{
		int fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "準備中");
		DrawExtendGraph(640 - fontwidth / 2 - 16, 360 - FONTSIZE1 / 2 - 16, 640 + fontwidth / 2 + 16, 360 + FONTSIZE1 / 2 + 16, status.loadingImage, TRUE);
		DrawFormatStringToHandle(640 - fontwidth / 2, 360 - FONTSIZE1 / 2, GetColor(255, 255, 255), status.FontData1, "準備中");
	}
	if ((status.GameStatus == GAME_PLAYING) && (status.phase == PHASE_INPUT) && (status.player == status.cpu))
	{
		int fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "思考中");
		DrawExtendGraph(640 - fontwidth / 2 - 16, 360 - FONTSIZE1 / 2 - 16, 640 + fontwidth / 2 + 16, 360 + FONTSIZE1 / 2 + 16, status.loadingImage, TRUE);
		DrawFormatStringToHandle(640 - fontwidth / 2, 360 - FONTSIZE1 / 2, GetColor(255, 255, 255), status.FontData1, "思考中");
	}
	if (status.init_ok_flag == true)
	{
		if (status.player == 1)
		{
			blackturn = 1;
			whiteturn = 0;
			playernum = 0;
		}
		else if (status.player == -1)
		{
			blackturn = 0;
			whiteturn = 1;
			playernum = 1;
		}
		if (status.GameStatus == GAME_END)
		{
			if (status.Winner == BLACK)
			{
				blackturn = 1;
				whiteturn = 0;
			}
			else if (status.Winner == WHITE)
			{
				blackturn = 0;
				whiteturn = 1;
			}
			else
			{
				blackturn = 0;
				whiteturn = 0;
			}
		}
		if (status.GameStatus == GAME_PLAYING || status.GameStatus == GAME_END)
		{
			char Black[10], White[10];
			if (status.cpu == 0)
			{
				strcpy(Black, "Player1");
				strcpy(White, "Player2");
			}
			else if (status.cpu == 1)
			{
				strcpy(Black, "CPU");
				strcpy(White, "Player");
			}
			if (status.cpu == -1)
			{
				strcpy(Black, "Player");
				strcpy(White, "CPU");
			}
			if ((status.GameStatus != GAME_END) && (status.use_timer == 0) && (status.time_rest[playernum] <= 10) && (status.time_rest[playernum] > 0))
			{
				double a = fmod(status.time_rest[playernum], 1);
				time_color[1] = GetColor(255, (int)((255 / 2) * cos(2 * PI * a)) + 255 / 2, 0);
				if (CheckSoundMem(status.sound6) == 0)
				{
					PlaySoundMem(status.sound6, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if ((status.use_timer == 0) &&(status.time_rest[playernum] <= 0))
			{
				time_color[0] = GetColor(255, 0, 0);
			}
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%s", Black);
			DrawFormatStringToHandle((320 - fontwidth) / 2, 360 + FONTSIZE1 * (-2), color[blackturn], status.FontData1, "%s", Black);
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%d", status.komaA);
			DrawFormatStringToHandle((320 - fontwidth) / 2, 360 + FONTSIZE1 * (-1), color[blackturn], status.FontData1, "%d", status.komaA);
			if (status.use_timer == 0)
			{
				fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%.0lfsec", status.time_rest[0] + 0.5);
				DrawFormatStringToHandle((320 - fontwidth) / 2, 360 + FONTSIZE1 * 0, time_color[blackturn], status.FontData1, "%.0lfsec", status.time_rest[0] + 0.5);
			}
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%.3lfsec", status.time_used_prev[0]);
			DrawFormatStringToHandle((320 - fontwidth) / 2, 360 + FONTSIZE1 * 1, color[blackturn], status.FontData1, "%.3lfsec", status.time_used_prev[0]);
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData4, "%d:%02d.%03d", status.time_used_total[0][0], status.time_used_total[0][1], status.time_used_total[0][2]);
			DrawFormatStringToHandle((320 - fontwidth) / 2, 360 + FONTSIZE1 * 2, color[blackturn], status.FontData4, "%d:%02d.%03d",
				status.time_used_total[0][0],
				status.time_used_total[0][1],
				status.time_used_total[0][2]
			);

			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%s", White);
			DrawFormatStringToHandle(1280 - (320 + fontwidth) / 2, 360 + FONTSIZE1 * (-2), color[whiteturn], status.FontData1, "%s", White);
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%d", status.komaB);
			DrawFormatStringToHandle(1280 - (320 + fontwidth) / 2, 360 + FONTSIZE1 * (-1), color[whiteturn], status.FontData1, "%d", status.komaB);
			if (status.use_timer == 0)
			{
				fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%.0lfsec", status.time_rest[1] + 0.5);
				DrawFormatStringToHandle(1280 - (320 + fontwidth) / 2, 360 + FONTSIZE1 * 0, time_color[whiteturn], status.FontData1, "%.0lfsec", status.time_rest[1] + 0.5);
			}
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData1, "%.3lfsec", status.time_used_prev[1]);
			DrawFormatStringToHandle(1280 - (320 + fontwidth) / 2, 360 + FONTSIZE1 * 1, color[whiteturn], status.FontData1, "%.3lfsec", status.time_used_prev[1]);
			fontwidth = GetDrawFormatStringWidthToHandle(status.FontData4, "%d:%02d.%03d", status.time_used_total[0][0], status.time_used_total[0][1], status.time_used_total[0][2]);
			DrawFormatStringToHandle(1280 - (320 + fontwidth) / 2, 360 + FONTSIZE1 * 2, color[whiteturn], status.FontData4, "%d:%02d.%03d",
				status.time_used_total[1][0],
				status.time_used_total[1][1],
				status.time_used_total[1][2]
			);
		}
	}

	if (status.GameStatus == GAME_SETTING)
	{
		unsigned int color;
		int fontwidth;
		DrawBox(480, 260, 1120, 420, GetColor(255, 255, 255), TRUE);
		DrawBox(480, 480, 820, 620, GetColor(255, 255, 255), TRUE);
		if (status.use_timer == -1)
		{
			color = GetColor(150, 150, 150);
		}
		else
		{
			color = GetColor(0, 0, 0);
		}
		fontwidth = GetDrawFormatStringWidthToHandle(status.FontData3, "%d", status.time_setting);
		DrawFormatStringToHandle(1120 - fontwidth, 420 - FONTSIZE3, color, status.FontData3, "%d", status.time_setting);
		if ((status.matta_enable == -1) || (status.matta_enable == 1))
		{
			color = GetColor(150, 150, 150);
		}
		else
		{
			color = GetColor(0, 0, 0);
		}
		fontwidth = GetDrawFormatStringWidthToHandle(status.FontData3, "%d", status.matta_setting);
		DrawFormatStringToHandle(820 - fontwidth, 620 - FONTSIZE3, color, status.FontData3, "%d", status.matta_setting);

		DrawExtendGraph(160, 660, 1120, 700, status.messagebox, TRUE);
		int mouseonbutton;
		if ((status.b_mouseon >= 0) || (status.b_clickon >= 0))
		{
			if (status.clickflag == CLICK_CLICKING)
			{
				mouseonbutton = status.b_clickon;
			}
			else
			{
				mouseonbutton = status.b_mouseon;
			}
			if (mouseonbutton >= BUTTON_NUM || mouseonbutton < 0)
			{
				mouseonbutton = -1;
			}
			if (mouseonbutton != -1)
			{
				fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "%s", message[mouseonbutton]);
			}
			int x;
			if (fontwidth > 1120 - 160)
			{
				x = 1120 - 160 / fontwidth;
			}
			else
			{
				x = 1;
			}
			if (mouseonbutton != -1)
			{
				DrawExtendFormatStringToHandle((1120 + 160 - fontwidth) / 2, (700 + 660 - FONTSIZE2) / 2, x, 1, GetColor(255, 255, 255), status.FontData2, "%s", message[mouseonbutton]);
			}
		}
	}

	button_draw(status);

	if (status.DebugMode == DEBUG_ON)
	{
		debug_draw(status, i_status);
	}

}

void button_draw(g_status status)
{
	for (int i = 0;i < BUTTON_NUM;++i)
	{
		int use_img;
		int outline = 0;
		int fontwidth;
		if (status.Button[i].button_flag != BUTTON_INVISIBLE)
		{
			if (status.Button[i].button_flag == BUTTON_DISABLE)
			{
				use_img = status.Button[i].img_disable;
			}
			else
			{
				use_img = status.Button[i].img;
			}
			DrawExtendGraph(status.Button[i].x1, status.Button[i].y1, status.Button[i].x2 - 1, status.Button[i].y2 - 1, use_img, TRUE);
			if (status.Button[i].icon == TRUE)
			{
				DrawExtendGraph(status.Button[i].x1, status.Button[i].y1, status.Button[i].x2 - 1, status.Button[i].y2 - 1, status.Button[i].button_icon, TRUE);
			}
			else
			{
				fontwidth = GetDrawFormatStringWidthToHandle(status.Button[i].font, "%s", &status.Button[i].text);
				DrawFormatStringToHandle((status.Button[i].x2 + status.Button[i].x1) / 2 - fontwidth / 2, status.Button[i].y1, GetColor(255, 255, 255), status.Button[i].font, "%s", &status.Button[i].text);
			}
		}
	}
	for (int i = 0; i < BUTTON_NUM; ++i)
	{
		int outline = 0;
		int mouseonflag = 0;
		unsigned int outcolor;
		if (status.b_clickon == i)
		{
			outline = 1;
			mouseonflag = 1;
			outcolor = GetColor(255, 0, 0);
		}
		else if ((status.b_mouseon == i) && ((status.b_clickon == status.b_mouseon)||(status.b_clickon == -1)))
		{
			outline = 1;
			mouseonflag = 1;
			outcolor = GetColor(255, 255, 0);
		}
		if (((status.cpu == 0) && (i == BUTTON_CPU_NONE)) ||
			((status.cpu == 1) && (i == BUTTON_CPU_BLACK)) ||
			((status.cpu == -1) && (i == BUTTON_CPU_WHITE)) ||
			((status.use_timer == 0) && (i == BUTTON_TIME_TYPEA)) ||
			((status.use_timer == -1) && (i == BUTTON_TIME_NONE)) ||
			((status.matta_enable == -1) && (i == BUTTON_MATTA_OFF)) ||
			((status.matta_enable == 0) && (i == BUTTON_MATTA_ON)) ||
			((status.matta_enable == 1) && (i == BUTTON_MATTA_INF)))
		{
			outline = 1;
			outcolor = GetColor(0, 0, 255);
		}
		if ((outline == 1) && (status.Button[i].button_flag == BUTTON_ENABLE))
		{
			DrawBox(status.Button[i].x1 - 2, status.Button[i].y1 - 2, status.Button[i].x1 + 2 - 1, status.Button[i].y2 + 2 - 1, outcolor, TRUE);	/* x1y1:x1y2 */
			DrawBox(status.Button[i].x1 - 2, status.Button[i].y1 - 2, status.Button[i].x2 + 2 - 1, status.Button[i].y1 + 2 - 1, outcolor, TRUE);	/* x1y1:x2y1 */
			DrawBox(status.Button[i].x2 - 2, status.Button[i].y1 - 2, status.Button[i].x2 + 2 - 1, status.Button[i].y2 + 2 - 1, outcolor, TRUE);	/* x2y1:x2y2 */
			DrawBox(status.Button[i].x1 - 2, status.Button[i].y2 - 2, status.Button[i].x2 + 2 - 1, status.Button[i].y2 + 2 - 1, outcolor, TRUE);	/* x1y2:x2y2 */

			int fontwidth;
			int centerX, centerY;
			int dispX;
			if (mouseonflag == 1 && status.Button[i].icon == TRUE)
			{
				fontwidth = GetDrawFormatStringWidthToHandle(status.FontData2, "%s", status.Button[i].text);
				centerY = (status.Button[i].y1 + status.Button[i].y2) / 2;
				centerX = (status.Button[i].x1 + status.Button[i].x2) / 2;
				dispX = centerX - fontwidth / 2;
				if (dispX < 0)
				{
					dispX = 0;
				}
				if (dispX + fontwidth > 1280)
				{
					dispX = 1280 - fontwidth;
				}
				DrawExtendFormatStringToHandle(dispX, centerY - FONTSIZE2 / 2, 1, 1, GetColor(255, 255, 255), status.FontData2, "%s", status.Button[i].text);
			}
		}
	}
}

/* 終了処理 */
void game_finalize(g_status* status)
{
	DeleteGraph(status->blackImage);
	DeleteGraph(status->whiteImage);
	DeleteGraph(status->blackGimage);
	DeleteGraph(status->whiteGimage);
	DeleteGraph(status->mapImage);
	DeleteGraph(status->selectedImage);
	DeleteGraph(status->clickedImage);
	DeleteGraph(status->black_status_area);
	DeleteGraph(status->white_status_area);
	DeleteGraph(status->messagebox);
	DeleteFontToHandle(status->FontData1);
	DeleteGraph(status->background);
	DeleteFontToHandle(status->FontData2);
	DeleteFontToHandle(status->FontData3);
	DeleteGraph(status->blackPutimage);
	DeleteGraph(status->whitePutimage);
	DeleteSoundMem(status->sound1);
	DeleteSoundMem(status->sound2);
	DeleteSoundMem(status->sound3);
	DeleteSoundMem(status->sound4);
	DeleteSoundMem(status->sound5);
	DeleteSoundMem(status->sound6);
	for (int i = 0;i < BUTTON_NUM;++i)
	{
		DeleteGraph(status->Button[i].img);
		if (status->Button[i].icon == TRUE)
		{
			DeleteGraph(status->Button[i].img_disable);
			DeleteGraph(status->Button[i].button_icon);
		}
		else
		{
			DeleteFontToHandle(status->Button[i].font);
		}
	}
	RemoveFontFile(status->FontFile);
}