/* リバーシの処理 */

#pragma warning(disable: 4996)
#include "DxLib.h"
#include "sub.h"

/* MAPファイル読み込み */
int read_map_file(const char* fname, char map[8][8])
{
	int x, y;
	char buf[81];
	FILE *fp;

	if((fp = fopen(fname, "r")) == NULL) return 0;
	fgets(buf, sizeof(buf), fp);
	for (y = 0;y < 8;++y)
	{
		fgets(buf, sizeof(buf), fp);
		map[y][0] = atoi(strtok(buf, ","));
		for (x = 1;x < 8;++x)
		{
			map[y][x] = atoi(strtok(NULL, ","));
		}
	}
	fclose(fp);
	return 1;
}

/* AI用マップ読み込み */
int read_aimap_file(const char* fname, int map[8][8])
{
	int x, y;
	char buf[81];
	FILE *fp;

	if ((fp = fopen(fname, "r")) == NULL) return 0;
	fgets(buf, sizeof(buf), fp);
	for (y = 0;y < 8;++y)
	{
		fgets(buf, sizeof(buf), fp);
		map[y][0] = atoi(strtok(buf, ","));
		for (x = 1;x < 8;++x)
		{
			map[y][x] = atoi(strtok(NULL, ","));
		}
	}
	fclose(fp);
	return 1;
}

/* 設定ファイル読み込み */
/* DxLib限定 */
/* =で区切る */
void setting_file_load(const char* fname, int* setting_time, int* timer_enable, int* setting_matta, int* matta_enable)
{
	char buf[100];
	char* tp = NULL;
	FILE* fp = NULL;

	if ((fp = fopen(fname, "r")) != NULL)
	{
		while (fgets(buf, sizeof(buf), fp) != NULL)
		{
			if (buf[0] == '#') continue;
			tp = strtok(buf, "=");
			if (strcmp(tp, "Time") == 0)
			{
				*setting_time = atoi(strtok(NULL, "\0"));
			}
			else if (strcmp(tp, "Enable Timer") == 0)
			{
				*timer_enable = atoi(strtok(NULL, "\0"));
			}
			else if (strcmp(tp, "Matta") == 0)
			{
				*setting_matta = atoi(strtok(NULL, "\0"));
			}
			else if (strcmp(tp, "Enable Matta") == 0)
			{
				*matta_enable = atoi(strtok(NULL, "\0"));
			}
		}
		fclose(fp);
		setting_file_save(fname, setting_time, timer_enable, setting_matta, matta_enable);
	}
	else
	{
		setting_file_save(fname, setting_time, timer_enable, setting_matta, matta_enable);
	}
}

void setting_file_save(const char* fname, int* setting_time, int* timer_enable, int* setting_matta, int* matta_enable)
{
	FILE* fp = NULL;

	fp = fopen(fname, "w");

	fprintf(fp, "# [ゲーム設定]\n");
	fprintf(fp, "# 時間設定(秒)\n");
	fprintf(fp, "Time=%d\n", *setting_time);
	fprintf(fp, "# タイマーの有無 -1:時間制限なし 0:一手の時間制限\n");
	fprintf(fp, "Enable Timer=%d\n", *timer_enable);
	fprintf(fp, "# 待った回数設定(回)\n");
	fprintf(fp, "Matta=%d\n", *setting_matta);
	fprintf(fp, "# 待ったの有無 -1:待ったなし 0:待ったあり(制限あり) 1:待った無制限\n");
	fprintf(fp, "Enable Matta=%d\n", *matta_enable);

	fclose(fp);
}


/* コマ設置可能チェック(戻り値:裏返し枚数) */
int can_put_koma(int player, int x, int y, char map[8][8])
{
	int dx, dy, count = 0, countA;
	int sx[8] = { 1,1,0,-1,-1,-1,0,1 };
	int sy[8] = { 0,1,1,1,0,-1,-1,-1 };

	if (map[y][x] != 0) return 0;

	for (int i = 0;i < 8;++i)
	{
		dx = x + sx[i];
		dy = y + sy[i];
		countA = 0;
		while ((dx >= 0) && (dx < 8) && (dy >= 0) && (dy < 8))
		{
			if (map[dy][dx] == player * (-1))
			{
				countA++;
			}
			else
			{
				if (map[dy][dx] == player)
				{
					count += countA;
				}
				break;
			}
			dx += sx[i];
			dy += sy[i];
		}
	}
	return count;
}

/* 全体のコマ設置可能チェック */
int all_can_put_koma(int player, char map[8][8], int flipmap[8][8])
{
	int x, y, count = 0, koma;
	for (y = 0;y < 8;++y)
	{
		for (x = 0;x < 8;++x)
		{
			koma = can_put_koma(player, x, y, map);
			count += koma;
			flipmap[y][x] = koma;
		}
	}
	return count;
}

/* 裏返し処理 */
int flip_koma(int player, char map[8][8], int flipdir[8], int x, int y)
{
	int dx, dy, count, flip, flipflag;
	int fliped = 0;
	int sx[8] = { 1,1,0,-1,-1,-1,0,1 };
	int sy[8] = { 0,1,1,1,0,-1,-1,-1 };
	if (map[y][x] != 0) return -1;

	for (int i = 0;i < 8;++i)
	{
		map[y][x] = player;
		dx = x + sx[i];
		dy = y + sy[i];
		count = 0;
		flipflag = 0;
		while ((dx >= 0) && (dx <= 7) && (dy >= 0) && (dy <= 7))
		{
			if (map[dy][dx] == player * (-1))
			{
				count++;
			}
			else
			{
				if (map[dy][dx] == player)
				{
					flipflag = 1;
				}
				break;
			}
			dx += sx[i];
			dy += sy[i];
		}
		dx = x;
		dy = y;
		if (flipflag == 1)
		{
			for (flip = 0; flip < count; flip++)
			{
				dx += sx[i];
				dy += sy[i];
				map[dy][dx] *= -1;
			}
			flipdir[i] = count;
			fliped = 1;
		}
		else
		{
			flipdir[i] = 0;
		}
	}
	if (fliped == 1) return 0;
	else return -1;
}

/* コマカウント */
void count_koma(int* black, int* white, char map[8][8])
{
	int x, y;
	int black1 = 0, white1 = 0;
	for (x = 0;x < 8;++x)
	{
		for (y = 0;y < 8;++y)
		{
			if (map[y][x] == 1)
			{
				black1++;
			}
			else if (map[y][x] == -1)
			{
				white1++;
			}
		}
	}
	*black = black1;
	*white = white1;
}


int game_backup(
	const char* fname,
	char startmap[8][8],
	char map[8][8],
	int history[REV_DATASIZE][11],
	double time[REV_DATASIZE],
	int* com,
	int* tekazu,
	int* time_setting,
	int* time_enable,
	int* matta,
	int* matta_enable,
	DATEDATA* start,
	int* matta_rest
)
{
	int x, y;
	int a = 8, b = 8;
	char str[16];
	FILE* fp;
	if ((fp = fopen(fname, "wb")) == NULL) return -1;
	fwrite("DETAIL", sizeof(char) * 6, 1, fp);
	sprintf(str, "%-10s", GAMEVERSION);
	fwrite(str, sizeof(char) * 10, 1, fp);
	fwrite(&start->Year, sizeof(int), 1, fp);
	fwrite(&start->Mon, sizeof(int), 1, fp);
	fwrite(&start->Day, sizeof(int), 1, fp);
	fwrite(&start->Hour, sizeof(int), 1, fp);
	fwrite(&start->Min, sizeof(int), 1, fp);
	fwrite(&start->Sec, sizeof(int), 1, fp);
	fwrite(com, sizeof(int), 1, fp);
	fwrite(time_enable, sizeof(int), 1, fp);
	fwrite(time_setting, sizeof(int), 1, fp);
	fwrite(matta_enable, sizeof(int), 1, fp);
	fwrite(matta, sizeof(int), 1, fp);
	fwrite(matta_rest, sizeof(int), 1, fp);
	fwrite(tekazu, sizeof(int), 1, fp);
	fwrite(&a, sizeof(int), 2, fp);	//盤面サイズ

	fwrite("mapS", sizeof(char) * 4, 1, fp);

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fwrite(&startmap[y][x], sizeof(char), 1, fp);
		}
	}

	fwrite("History", sizeof(char) * 7, 1, fp);

	for (int j = 0; j < (*tekazu) + 1; ++j)
	{
		for (int i = 0; i < 11; ++i)
		{
			fwrite(&history[j][i], sizeof(int), 1, fp);
		}
		fwrite(&time[j], sizeof(double), 1, fp);
	}

	fwrite("mapN", sizeof(char) * 4, 1, fp);

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fwrite(&map[y][x], sizeof(char), 1, fp);
		}
	}
	fclose(fp);
	return 0;
}


int game_backup_load(
	const char* fname,
	char startmap[8][8],
	char map[8][8],
	int history[REV_DATASIZE][11],
	double time[REV_DATASIZE],
	int* com,
	int* tekazu,
	int* time_setting,
	int* time_enable,
	int* matta,
	int* matta_enable,
	int* player,
	DATEDATA* start,
	int* matta_rest
)
{
	int x, y;
	int a = 8, b = 8;
	FILE* fp;
	char str[16];
	char str1[16];
	if ((fp = fopen(fname, "rb")) == NULL) return -1;
	fread(str, sizeof(char) * 6, 1, fp);
	if (memcmp("DETAIL", str, sizeof(char) * 6) != 0) return -1;
	fread(str, sizeof(char) * 10, 1, fp);
	sprintf(str1, "%-10s", GAMEVERSION);
	if (memcmp(str1, str, sizeof(char) * 10) != 0) return -1;
	fread(&start->Year, sizeof(int), 1, fp);
	fread(&start->Mon, sizeof(int), 1, fp);
	fread(&start->Day, sizeof(int), 1, fp);
	fread(&start->Hour, sizeof(int), 1, fp);
	fread(&start->Min, sizeof(int), 1, fp);
	fread(&start->Sec, sizeof(int), 1, fp);
	fread(com, sizeof(int), 1, fp);
	fread(time_enable, sizeof(int), 1, fp);
	fread(time_setting, sizeof(int), 1, fp);
	fread(matta_enable, sizeof(int), 1, fp);
	fread(matta, sizeof(int), 1, fp);
	fread(matta_rest, sizeof(int), 1, fp);
	fread(tekazu, sizeof(int), 1, fp);
	fread(str, sizeof(int), 2, fp);	//盤面サイズ

	fread(str, sizeof(char) * 4, 1, fp);
	if (memcmp("mapS", str, sizeof(char) * 4) != 0) return -1;

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fread(&startmap[y][x], sizeof(char), 1, fp);
		}
	}

	fread(str, sizeof(char) * 7, 1, fp);
	if (memcmp("History", str, sizeof(char) * 7) != 0) return -1;

	for (int j = 0; j < (*tekazu) + 1; ++j)
	{
		for (int i = 0; i < 11; ++i)
		{
			fread(&history[j][i], sizeof(int), 1, fp);
		}
		fread(&time[j], sizeof(double), 1, fp);
		*player = -history[j][0];
	}

	fread(str, sizeof(char) * 4, 1, fp);
	if (memcmp("mapN", str, sizeof(char) * 4) != 0) return -1;

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fread(&map[y][x], sizeof(char), 1, fp);
		}
	}
	(*tekazu)++;
	fclose(fp);
	return 0;
}

int backup_check(char startmap[8][8], char map[8][8], int history[REV_DATASIZE][11], int tekazu)
{
	int x, y;
	char nowmap[8][8] = { 0 };
	int flipmap[8][8] = { 0 };
	for (y = 0; y < 8; y++)
	{
		for (x = 0; x < 8; x++)
		{
			nowmap[y][x] = startmap[y][x];
		}
	}
	all_can_put_koma(history[0][0], map, flipmap);
	for (int i = 0; i < tekazu; i++)
	{
		if (next_map(history[i][0], history[i][1], history[i][2], nowmap, nowmap, flipmap, flipmap) != 0) return -1;
	}
	for (y = 0; y < 8; y++)
	{
		for (x = 0; x < 8; x++)
		{
			if (nowmap[y][x] != map[y][x]) return -1;
		}
	}
	return 0;
}

int replay_create(
	const char* fname,
	char startmap[8][8],
	char map[8][8],
	int history[REV_DATASIZE][11],
	double time[REV_DATASIZE],
	int* com,
	int* tekazu,
	int* time_setting,
	int* time_enable,
	int* matta,
	int* matta_enable,
	int* player,
	DATEDATA* start,
	DATEDATA* end,
	int* matta_rest
)
{
	int x, y;
	int a = 8, b = 8;
	char str[16];
	FILE* fp;
	if ((fp = fopen(fname, "wb")) == NULL) return -1;
	fwrite("DETAIL", sizeof(char) * 6, 1, fp);
	sprintf(str, "%-10s", GAMEVERSION);
	fwrite(str, sizeof(char) * 10, 1, fp);
	fwrite(&start->Year, sizeof(int), 1, fp);
	fwrite(&start->Mon, sizeof(int), 1, fp);
	fwrite(&start->Day, sizeof(int), 1, fp);
	fwrite(&start->Hour, sizeof(int), 1, fp);
	fwrite(&start->Min, sizeof(int), 1, fp);
	fwrite(&start->Sec, sizeof(int), 1, fp);
	fwrite(&end->Year, sizeof(int), 1, fp);
	fwrite(&end->Mon, sizeof(int), 1, fp);
	fwrite(&end->Day, sizeof(int), 1, fp);
	fwrite(&end->Hour, sizeof(int), 1, fp);
	fwrite(&end->Min, sizeof(int), 1, fp);
	fwrite(&end->Sec, sizeof(int), 1, fp);
	fwrite(com, sizeof(int), 1, fp);
	fwrite(time_enable, sizeof(int), 1, fp);
	fwrite(time_setting, sizeof(int), 1, fp);
	fwrite(matta_enable, sizeof(int), 1, fp);
	fwrite(matta, sizeof(int), 1, fp);
	fwrite(matta_rest, sizeof(int), 1, fp);
	fwrite(tekazu, sizeof(int), 1, fp);
	fwrite(&a, sizeof(int), 2, fp);	//盤面サイズ

	fwrite("mapS", sizeof(char) * 4, 1, fp);

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fwrite(&startmap[y][x], sizeof(char), 1, fp);
		}
	}

	fwrite("History", sizeof(char) * 7, 1, fp);

	for (int j = 0; j < *tekazu; ++j)
	{
		for (int i = 0; i < 11; ++i)
		{
			fwrite(&history[j][i], sizeof(int), 1, fp);
		}
		fwrite(&time[j], sizeof(double), 1, fp);
	}

	fwrite("mapF", sizeof(char) * 4, 1, fp);

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fwrite(&map[y][x], sizeof(char), 1, fp);
		}
	}
	fclose(fp);
	return 0;
}

int replay_load(
	const char* fname,
	char startmap[8][8],
	char map[8][8],
	int history[REV_DATASIZE][11],
	double time[REV_DATASIZE],
	int* com,
	int* tekazu,
	int* time_setting,
	int* time_enable,
	int* matta,
	int* matta_enable,
	int* player,
	DATEDATA* start,
	DATEDATA* end,
	int* matta_rest
)
{
	int x, y;
	int a = 8, b = 8;
	FILE* fp;
	char str[16];
	char str1[16];
	if ((fp = fopen(fname, "rb")) == NULL) return -1;
	fread(str, sizeof(char) * 6, 1, fp);
	if (memcmp("DETAIL", str, sizeof(char) * 6) != 0) return -1;
	fread(str, sizeof(char) * 10, 1, fp);
	sprintf(str1, "%-10s", GAMEVERSION);
	if (memcmp(str1, str, sizeof(char) * 10) != 0) return -1;
	fread(&start->Year, sizeof(int), 1, fp);
	fread(&start->Mon, sizeof(int), 1, fp);
	fread(&start->Day, sizeof(int), 1, fp);
	fread(&start->Hour, sizeof(int), 1, fp);
	fread(&start->Min, sizeof(int), 1, fp);
	fread(&start->Sec, sizeof(int), 1, fp);
	fread(&end->Year, sizeof(int), 1, fp);
	fread(&end->Mon, sizeof(int), 1, fp);
	fread(&end->Day, sizeof(int), 1, fp);
	fread(&end->Hour, sizeof(int), 1, fp);
	fread(&end->Min, sizeof(int), 1, fp);
	fread(&end->Sec, sizeof(int), 1, fp);
	fread(com, sizeof(int), 1, fp);
	fread(time_enable, sizeof(int), 1, fp);
	fread(time_setting, sizeof(int), 1, fp);
	fread(matta_enable, sizeof(int), 1, fp);
	fread(matta, sizeof(int), 1, fp);
	fread(matta_rest, sizeof(int), 1, fp);
	fread(tekazu, sizeof(int), 1, fp);
	fread(str, sizeof(int), 2, fp);	//盤面サイズ

	fread(str, sizeof(char) * 4, 1, fp);
	if (memcmp("mapS", str, sizeof(char) * 4) != 0) return -1;

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fread(&startmap[y][x], sizeof(char), 1, fp);
		}
	}

	fread(str, sizeof(char) * 7, 1, fp);
	if (memcmp("History", str, sizeof(char) * 7) != 0) return -1;

	for (int j = 0; j < *tekazu; ++j)
	{
		for (int i = 0; i < 11; ++i)
		{
			fread(&history[j][i], sizeof(int), 1, fp);
		}
		fread(&time[j], sizeof(double), 1, fp);
		*player = history[j][0];
	}

	fread(str, sizeof(char) * 4, 1, fp);
	if (memcmp("mapF", str, sizeof(char) * 4) != 0) return -1;

	for (y = 0; y < 8; ++y)
	{
		for (x = 0; x < 8; ++x)
		{
			fread(&map[y][x], sizeof(char), 1, fp);
		}
	}
	fclose(fp);
	return 0;
}

int replay_check(char startmap[8][8], char map[8][8], int history[REV_DATASIZE][11], int tekazu)
{
	int x, y;
	char nowmap[8][8] = { 0 };
	int flipmap[8][8] = { 0 };
	for (y = 0; y < 8; y++)
	{
		for (x = 0; x < 8; x++)
		{
			nowmap[y][x] = startmap[y][x];
		}
	}
	all_can_put_koma(history[0][0], map, flipmap);
	for (int i = 0; i < tekazu; i++)
	{
		if (next_map(history[i][0], history[i][1], history[i][2], nowmap, nowmap, flipmap, flipmap) != 0) return -1;
	}
	for (y = 0; y < 8; y++)
	{
		for (x = 0; x < 8; x++)
		{
			if (nowmap[y][x] != map[y][x]) return -1;
		}
	}
	return 0;
}

int ai_put_koma(int com, int* putX, int* putY, char map[8][8], int flipmap[8][8], int* flag, int tekazu, int aimap[8][8], int pass, int* op_count, int mapscore[8][8])
{
	int max = -1, min = INF-1;
	int max_count = 0, min_count = 0;
	int i = 0, j = 0;
	int chance = 0;
	int kohoX = -1, kohoY = -1;
	int kadoB = 0, kadoW = 0;

	for (int k = 0; k < 8; k++)
	{
		for (int l = 0; l < 8; l++)
		{
			mapscore[k][l] = -INF-1;
		}
	}
/*
	if (tekazu>=44)
	{
		*flag = 1;
	}
//	check_kakuteiseki(com*(-1), map, flipmap, kakutei);
//	chance = check_kakuteiseki(com, map, flipmap, kakutei);
	if (chance > 0)
	{
		j = GetRand(chance - 1);
		i = 0;
		for (int y = 0;y < 8;++y)
		{
			for (int x = 0;x < 8;++x)
			{
				if (flipmap[y][x] > 0)
				{
					i++;
					if (i >= j)
					{
						*putX = x;
						*putY = y;
						return 0;
					}
				}
			}
		}
	}
	else
	{
		for (int y = 0;y < 8;++y)
		{
			for (int x = 0;x < 8;++x)
			{
				if (*flag == 0)
				{
					if ((flipmap[y][x] < min) && (flipmap[y][x] > 0))
					{
						min = flipmap[y][x];
						min_count = 1;
					}
					else if (flipmap[y][x] == min)
					{
						min_count++;
					}
				}
				else if (*flag == 1)
				{
					if (flipmap[y][x] > max)
					{
						max = flipmap[y][x];
						max_count = 1;
					}
					else if (flipmap[y][x] == max)
					{
						max_count++;
					}
				}
			}
		}
		if (*flag == 0)
		{
			j = GetRand(min_count - 1);
			i = 0;
			for (int y = 0;y < 8;++y)
			{
				for (int x = 0;x < 8;++x)
				{
					if (flipmap[y][x] == min)
					{
						i++;
						if (i >= j)
						{
							*putX = x;
							*putY = y;
							return 0;
						}
					}
				}
			}
		}
		if (*flag == 1)
		{
			j = GetRand(max_count - 1);
			i = 0;
			for (int y = 0;y < 8;++y)
			{
				for (int x = 0;x < 8;++x)
				{
					if (flipmap[y][x] == max)
					{
						i++;
						if (i >= j)
						{
							*putX = x;
							*putY = y;
							return 0;
						}
					}
				}
			}
		}
	}
	return -1;
	*/
if (tekazu>48)
{
	(*op_count)++;
	ai_simulate_end(&kohoX, &kohoY, com, com, DEPTH, map, aimap, flipmap, pass, op_count,mapscore);
	*putX = kohoX;
	*putY = kohoY;
}
else
{
	(*op_count)++;
	ai_simulate_mid(&kohoX, &kohoY, com, com, DEPTH, map, aimap, flipmap, -INF-1, INF, pass, op_count,mapscore);
	*putX = kohoX;
	*putY = kohoY;
}
if ((putX >= 0) && (putY >= 0))
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/*
int check_kakuteiseki(int player, char map[8][8], int flipmap[8][8], int kakutei[8][8])
{
	/* 左上から右、右上から下、右下から左、左下から上 */
	/* 2回目は逆周り */
/*	int x, y;
	int dx, dy;
	int search[8] = { 0,0,0,0,0,0,0,0 };
	int sx[8] = { 1,1,0,-1,-1,-1,0,1 };
	int sy[8] = { 0,1,1,1,0,-1,-1,-1 };
	int dir[4][8] = {	/* (判定方向*4)*2 */
/*		{4,5,6,7,3,4,5,6},
		{5,6,7,0,6,7,0,1},
		{0,1,2,3,7,0,1,2},
		{1,2,3,4,2,3,4,5}
	};
	int koma;
	int corner[4][2] = {
		{0,0},
		{0,7},
		{7,7},
		{7,0}
	};
	int chance = 0;
	int dx2, dy2;

	for (int a = 0;a < 4;++a)
	{
		if (a == 0)
		{
			for (int hen = 0;hen < 4;hen++)
			{
				koma = 0;
				x = corner[hen][0];
				y = corner[hen][1];
				for (int i = 0;i < 8;i++)
				{
					dx = x + sx[hen * 2] * i;
					dy = y + sy[hen * 2] * i;
					if (map[dy][dx] != 0)
					{
						koma++;
					}
				}
				if (koma >= 8)
				{
					for (int i = 0;i < 8;i++)
					{
						dx = x + sx[hen * 2] * i;
						dy = y + sy[hen * 2] * i;

						kakutei[dy][dx] = 2;
					}
				}
			}
		}
		for (int i = 0;i < 4;i++)
		{
			if (map[corner[i][1]][corner[i][0]] != 0)
			{
				kakutei[corner[i][1]][corner[i][0]] = 2;
			}
			else
			{
				kakutei[corner[i][1]][corner[i][0]] = 1;
			}
		}


		for (int i = 0;i < 8;++i)
		{
			search[i] = 0;
		}
		
		for (int d = 0;d < 2;++d)
		{
			for (int b = 0;b < 4;++b)
			{
				dx = corner[b][0];
				dy = corner[b][1];
				for (int i = 0;i < 8;++i)
				{
					dx2 = dx + sx[b * 2] * i;
					dy2 = dy + sy[b * 2] * i;
					if (a == 0)
					{
						if (i > 0)
						{
							if (((map[a][dx] == map[a][dx2])) && (kakutei[a][dx2] == 2))
							{
								if ((map[a][dx] != 0)||(kakutei[a][dx2] == 2))
								{
									kakutei[a][dx2] = 2;
								}
								if ((map[a][dx] == player * (-1)) && (kakutei[a][dx2] != 2))
								{
									kakutei[a][dx2] = 1;
								}
							}

							if ((map[dy][a] == map[dy2][a]) && (kakutei[dy2][a] == 2))
							{
								if ((map[dy][a] != 0)|| (kakutei[dy][a] == 2))
								{
									kakutei[dy2][a] = 2;
								}
								if ((map[dy][a] == player * (-1)) && (kakutei[a][dx2] != 2))
								{
									kakutei[dy2][a] = 1;
								}
							}
						}
					}
					else
					{

					}
				}
			}
		}

		for (int b = 0;b < 4;++b)
		{
			x = corner[b][0];
			y = corner[b][1];
			int flag1 = 0;
			int flag2 = 0;

			for (int i = 0;i < 8 - 2 * a;++i)
			{
				int x1, y1, x2, y2;
				int count1 = 0, count2 = 0;
				int cx, cy;

				cx = x + a * sx[b * 2 + 1];
				cy = y + a * sy[b * 2 + 1];

				dx = cx + (sx[b * 2 + 1] * i);
				dy = cy + (sy[b * 2 + 1] * i);
				for (int j = 0;j < 4;++j)
				{
					if (flag1 == 0)
					{
						x1 = dx + sx[dir[b][j]];
						y1 = cy + sx[dir[b][j]];
						if ((x1 < 0) || (x1 >= 8) || (y1 < 0) || (y1 > 8))
						{
							count1++;
						}
						else if ((map[y1][x1] == player) && (kakutei[y1][x1] == 2))
						{
							count1++;
						}
					}

					if (flag2 == 0)
					{
						x2 = cx + sx[dir[b][j + 4]];
						y2 = dy + sy[dir[b][j + 4]];

						if ((x2 < 0) || (x2 >= 8) || (y2 < 0) || (y2 >= 8))
						{
							count2++;
						}
						else if ((map[y2][x2] == player) && (kakutei[y2][x2] == 2))
						{
							count2++;
						}
					}
				}
				if (count1 >= 4)
				{
					if (map[cy][dx] == 0)
					{
						flag1 = 1;
						kakutei[cy][dx] = 1;
						if (flipmap[cy][dx] > 0)
						{
							chance++;
						}
					}
					else
					{
						kakutei[cy][dx] = 2;
					}
				}
				else
				{
					flag1 = 1;
				}
				if (count2 >= 4)
				{
					if (map[dy][cx] == 0)
					{
						flag2 = 1;
						kakutei[dy][cx] = 1;
						if (flipmap[dy][cx] > 0)
						{
							chance++;
						}
					}
					else
					{
						kakutei[dy][cx] = 2;
					}
				}
				else
				{
					flag2 = 1;
				}
			}
		}
	}
	return chance;
}
*/

int ai_simulate_mid(int* x, int* y, int com, int player, int depth, char map[8][8], int aimap[8][8], int flipmap[8][8], int alpha, int beta, int pass, int* op_count, int mapscore[8][8])
{
	int value = -INF - 1;
	int max = -INF - 1;
	char nmap[8][8] = { 0 };
	int nflipmap[8][8] = { 0 };
//	int flipcount;
	int m, n;
	int koholist[60][2] = { 0 };
	int kohocount = 0;
	int random;
	int komaB, komaW;

	if (depth <= 0)
	{
		return map_score(com, map, aimap, flipmap);
	}

	if (all_can_put_koma(player, map, flipmap) == 0)
	{
		if (pass <= 0)
		{
			(*op_count)++;
			value = ai_simulate_mid(&m, &n, com, -player, depth-1, map, aimap, flipmap,alpha,beta, pass + 1, op_count,mapscore);
			return value;
		}
		else if (pass >= 1)
		{
			count_koma(&komaB, &komaW, map);
			if (com == 1)
			{
				if (komaB > komaW)
				{
					return INF;
				}
				else if (komaB < komaW)
				{
					return -INF;
				}
			}
			else if (com == -1)
			{
				if (komaB > komaW)
				{
					return -INF;
				}
				else if (komaB < komaW)
				{
					return INF;
				}
			}
			if (komaB == komaW)
			{
				return 0;
			}
		}
	}

	for (int j = 0; (alpha < beta) && (j < 8); j++)
	{
		for (int i = 0; (alpha < beta) && (i < 8); i++)
		{
			if (flipmap[j][i] > 0)
			{
				for (int l = 0; l < 8; l++)
				{
					for (int k = 0; k < 8; k++)
					{
						nmap[k][l] = map[k][l];
						nflipmap[k][l] = flipmap[k][l];
					}
				}
				next_map(player, i, j, map, nmap, flipmap, nflipmap);

				m = i;
				n = j;

				(*op_count)++;
				value = -ai_simulate_mid(&m, &n, com, -player, depth - 1, nmap, aimap, nflipmap, -beta, -alpha, pass, op_count, mapscore);
				if (depth == DEPTH)
				{
					mapscore[j][i] = value;
				}
				if (value > max)
				{
					max = value;
					kohocount = 0;
					koholist[kohocount][0] = i;
					koholist[kohocount][1] = j;
					kohocount++;
				}
				else if (value == max)
				{
					koholist[kohocount][0] = i;
					koholist[kohocount][1] = j;
					kohocount++;
				}
				if (alpha < max)
				{
					alpha = max;
				}
				if (max >= beta)
				{
					return max;
				}
			}
		}
	}
	if (depth == DEPTH)
	{
		/* ここからDxLib.h依存 */
		SRand(GetNowCount());
		random = GetRand(kohocount - 1);
		/* ここまで */

		*x = koholist[random][0];
		*y = koholist[random][1];
	}

	return value;
}

int ai_simulate_end(int* x, int* y, int com, int player, int depth, char map[8][8], int aimap[8][8], int flipmap[8][8], int pass, int* op_count, int mapscore[8][8])
{
	int value, max = -INF-1;
	char nmap[8][8] = { 0 };
	int nflipmap[8][8] = { 0 };
//	int flipcount;
	int m, n;
	int koholist[60][2] = { 0 };
	int kohocount = 0;
	int random;
	int komaB, komaW;

	if (depth <= 0)
	{
		count_koma(&komaB, &komaW, map);
		return (com == 1 ? komaB - komaW : komaW - komaB);
	}

	if (all_can_put_koma(player, map, flipmap) == 0)
	{
		if (pass <= 0)
		{
			(*op_count)++;
			value = -ai_simulate_end(&m, &n, com, -player, depth-1, map, aimap, flipmap, pass + 1, op_count,mapscore);
			return value;
		}
		else if (pass >= 1)
		{
			count_koma(&komaB, &komaW, map);
			return (com == 1 ? komaB - komaW : komaW - komaB);
		}
	}

	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (flipmap[j][i] > 0)
			{
				for (int l = 0; l < 8; l++)
				{
					for (int k = 0; k < 8; k++)
					{
						nmap[k][l] = map[k][l];
						nflipmap[k][l] = flipmap[k][l];
					}
				}
				next_map(player, i, j, map, nmap, flipmap, nflipmap);

				m = i;
				n = j;

				(*op_count)++;
				value = -ai_simulate_end(&m, &n, com, -player, depth - 1, nmap, aimap, nflipmap, pass, op_count,mapscore);
				if (depth == DEPTH)
				{
					mapscore[j][i] = value;
				}
				if (value > max)
				{
					kohocount = 0;
					max = value;
					koholist[kohocount][0] = i;
					koholist[kohocount][1] = j;
					kohocount++;
				}
				else if (value == max)
				{
					koholist[kohocount][0] = i;
					koholist[kohocount][1] = j;
					kohocount++;
				}
			}
		}
	}
	/* ここからDxLib.h依存 */
	SRand(GetNowCount());
	random = GetRand(kohocount - 1);
	/* ここまで */

	*x = koholist[random][0];
	*y = koholist[random][1];

	return value;
}


int map_score(int com, char map[8][8], int aimap[8][8], int flipmap[8][8])
{
	int scoreB = 0, scoreW = 0, score = 0;
	int naimap[8][8] = { 0 };

	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			naimap[j][i] = aimap[j][i];
		}
	}


	if (map[0][0] != 0)
	{
		for (int i = 1; i < 8; ++i)
		{
			if ((map[0][i] == 0) || (map[0][i] == com))
			{
				naimap[0][i] = naimap[0][i - 1];
				if (map[0][i] == 0)
				{
					break;
				}
			}
			else if (map[0][i] == -com)
			{
				naimap[0][i] = -naimap[0][i - 1];
			}
		}
		for (int i = 1; i < 8; ++i)
		{
			if ((map[i][0] == 0) || (map[i][0] == com))
			{
				naimap[i][0] = naimap[i - 1][0];
				if (map[i][0] == 0)
				{
					break;
				}
			}
			else if (map[i][0] == -com)
			{
				naimap[i][0] = -naimap[i - 1][0];
			}
		}
	}
	if (map[0][7] != 0)
	{
		for (int i = 1; i < 8; ++i)
		{
			if ((map[0][7 - i] == 0) || (map[0][7 - i] == com))
			{
				naimap[0][7 - i] = naimap[0][7 - (i - 1)];
				if (map[0][7 - i] == 0)
				{
					break;
				}
			}
			else if (map[0][i] == -com)
			{
				naimap[0][i] = -naimap[0][7 - (i - 1)];
			}
		}
		for (int i = 1; i < 8; ++i)
		{
			if ((map[i][7] == 0) || (map[i][7] == com))
			{
				naimap[i][7] = naimap[i - 1][7];
				if (map[i][7] == 0)
				{
					break;
				}
			}
			else if (map[i][0] == -com)
			{
				naimap[i][7] = -naimap[i - 1][7];
			}
		}
	}
	if (map[7][0] != 0)
	{
		for (int i = 1; i < 8; ++i)
		{
			if ((map[7][i] == 0) || (map[7][i] == com))
			{
				naimap[7][i] = naimap[7][i - 1];
				if (map[7][i] == 0)
				{
					break;
				}
			}
			else if (map[0][i] == -com)
			{
				naimap[7][i] = -naimap[7][i - 1];
			}
		}
		for (int i = 1; i < 8; ++i)
		{
			if ((map[7 - i][0] == 0) || (map[7 - i][0] == com))
			{
				naimap[7 - i][0] = naimap[7 - (i - 1)][0];
				if (map[7 - i][0] == 0)
				{
					break;
				}
			}
			else if (map[i][0] == -com)
			{
				naimap[7 - i][0] = -naimap[7 - (i - 1)][0];
			}
		}
	}
	if (map[7][7] != 0)
	{
		for (int i = 1; i < 8; ++i)
		{
			if ((map[7][7 - i] == 0) || (map[7][7 - i] == com))
			{
				naimap[7][7 - i] = naimap[7][7 - (i - 1)];
				if (map[7][7 - i] == 0)
				{
					break;
				}
			}
			else if (map[0][i] == -com)
			{
				naimap[7][7 - i] = -naimap[7][7 - (i - 1)];
			}
		}
		for (int i = 1; i < 8; ++i)
		{
			if ((map[7 - i][7] == 0) || (map[7 - i][7] == com))
			{
				naimap[7 - i][7] = naimap[7 - (i - 1)][7];
				if (map[7 - i][7] == 0)
				{
					break;
				}
			}
			else if (map[i][0] == -com)
			{
				naimap[7 - i][7] = -naimap[7 - (i - 1)][7];
			}
		}
	}

	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (map[j][i] == 1)
			{
				scoreB += aimap[j][i];
			}
			else if (map[j][i] == -1)
			{
				scoreW += aimap[j][i];
			}
		}
	}
	if (com == 1)
	{
		score = scoreB - scoreW;
	}
	else if (com == -1)
	{
		score = scoreW - scoreB;
	}
	return score;
}

int next_map(int player, int x, int y, char map[8][8], char nextmap[8][8],int flipmap[8][8],int nextfmap[8][8])
{
	int flipdir[8] = { 0 };
	if (flip_koma(player, nextmap, flipdir, x, y) != 0) return -1;
	all_can_put_koma(-player, nextmap, nextfmap);
	return 0;
}

int prev_map(int player, int x, int y, char map[8][8], char prevmap[8][8], int flipmap[8][8], int prevfmap[8][8], int flipdir[8])
{
	int dx, dy;
	int sx[8] = { 1,1,0,-1,-1,-1,0,1 };
	int sy[8] = { 0,1,1,1,0,-1,-1,-1 };
	for (int j = 0; j < 8; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			prevmap[j][i] = map[j][i];
			prevfmap[j][i] = map[j][i];
		}
	}
	if (prevmap[y][x] != player) return -1;

	if (prevmap[y][x] == player)
	{
		prevmap[y][x] = 0;
	}
	for (int dir = 0; dir < 8; dir++)
	{
		dx = x;
		dy = y;
		for (int i = 0; i < flipdir[dir]; ++i)
		{
			dx += sx[dir];
			dy += sy[dir];
			if (((dx < 0) || (dx >= 8) || (dy < 0) || (dy >= 8)) || prevmap[dy][dx] != player)
			{
				return -1;
			}
			prevmap[dy][dx] *= -1;
		}
	}
	all_can_put_koma(player, prevmap, prevfmap);
	return 0;
}

int Matta(int* player, int com, int history[REV_DATASIZE][11], double time_history[REV_DATASIZE], char map[8][8], int flipmap[8][8], int* tekazu, int* matta_count, double time_used_prev[2], int time_used_total[2][3])
{
	int flipdir[8] = { 0 };
	int player_tmp, x_tmp, y_tmp;
	(*tekazu)--;
	for (int i = 0; i < 8; i++)
	{
		flipdir[i] = history[*tekazu][i + 3];
	}
	player_tmp = history[*tekazu][0];
	x_tmp = history[*tekazu][1];
	y_tmp = history[*tekazu][2];
	prev_map(player_tmp, x_tmp, y_tmp, map, map, flipmap, flipmap, flipdir);
	switch (player_tmp)
	{
	case 1:
	{
		time_used_total[0][1] -= (int)time_history[*tekazu];
		time_used_total[0][2] -= (int)(time_history[*tekazu] * 1000) % 1000;
		operate_time(&time_used_total[0][0], &time_used_total[0][1], &time_used_total[0][2]);
		break;
	}
	case -1:
	{
		time_used_total[1][1] -= (int)time_history[*tekazu];
		time_used_total[1][2] -= (int)(time_history[*tekazu] * 1000) % 1000;
		operate_time(&time_used_total[1][0], &time_used_total[1][1], &time_used_total[1][2]);
		break;
	}
	}
	while (*tekazu >= 1 && history[*tekazu][0] == com)
	{
		(*tekazu)--;
		for (int i = 0; i < 8; i++)
		{
			flipdir[i] = history[*tekazu][i + 3];
		}
		player_tmp = history[*tekazu][0];
		x_tmp = history[*tekazu][1];
		y_tmp = history[*tekazu][2];
		prev_map(player_tmp, x_tmp, y_tmp, map, map, flipmap, flipmap, flipdir);
		switch (player_tmp)
		{
		case 1:
		{
			time_used_total[0][1] -= (int)time_history[*tekazu];
			time_used_total[0][2] -= (int)(time_history[*tekazu] * 1000) % 1000;
			operate_time(&time_used_total[0][0], &time_used_total[0][1], &time_used_total[0][2]);
			break;
		}
		case -1:
		{
			time_used_total[1][1] -= (int)time_history[*tekazu];
			time_used_total[1][2] -= (int)(time_history[*tekazu] * 1000) % 1000;
			operate_time(&time_used_total[1][0], &time_used_total[1][1], &time_used_total[1][2]);
			break;
		}
		}
	}
	if (*tekazu > 0)
	{
		*player = history[*tekazu][0];
	}
	else
	{
		*player = 1;
	}
	(*matta_count)--;
	return 0;
}