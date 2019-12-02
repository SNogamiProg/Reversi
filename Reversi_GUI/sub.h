#if !defined SUB_H
#define SUB_H

#define PI 3.14159265

/* ゲームの状態 */
#define GAME_TITLE 0	/* タイトル */
#define GAME_SETTING 1	/* ゲーム設定 */
#define GAME_REPLAYLIST 2	/* リプレイリスト */
#define GAME_OPTION 3	/* オプション */
#define GAME_INITIALIZE 4	/* ゲーム初期設定 */
#define GAME_PLAYING 5	/* ゲーム中 */
#define GAME_END 6	/* ゲーム終了 */
#define GAME_REPLAY_INIT 7	/* リプレイ初期設定 */
#define GAME_REPLAY 8	/* リプレイ再生 */

/* フェーズ */
#define PHASE_TITLE_INIT 0
#define PHASE_TITLE 1

#define PHASE_SETTING_INIT 0
#define PHASE_SETTING 1

#define PHASE_NONE	-1	/* 該当なし */
#define PHASE_INPUT_INIT 0	/* 最初の処理 */
#define PHASE_INPUT 1	/* 入力待ち状態 */
#define PHASE_FLIP 2	/* 裏返し処理 */
#define PHASE_SUCCESSFUL 3	/* 設置成功 */
#define PHASE_FAILURE 4	/* 設置失敗 */

/* クリックフラグ */
#define CLICK_NOTCLICK 0	/* クリックしてない */
#define CLICK_CLICKING 1	/* クリック中 */
#define MOUSE_NOTON 0	/* マウスを乗せていない */
#define MOUSE_ON 1	/* マウスを乗せている */

/* 設置失敗 */
#define PUT_SUCCESSFUL 0	/* 設置成功 */
#define PUT_PASS 1	/* パス */
#define PUT_FAILURE 2	/* 設置失敗 */

/* コマの色 */
#define BLACK 1
#define WHITE -1

/* 残り時間デフォルト */
#define TIME_DEFAULT 30.0

#define MATTA_DEFAULT 1

#define REV_DATASIZE 60

#define FONTSIZE 16

#define FONTSIZE1 48
#define FONTSIZE2 16
#define FONTSIZE3 36
#define FONTSIZE4 36

#define BUTTON_PASS 0	/* パスしたときのOKボタン */
#define BUTTON_RETRY 1	/* リトライ */

#define BUTTON_CPU_BLACK 2	/* CPU黒 */
#define BUTTON_CPU_WHITE 3	/* CPU白 */
#define BUTTON_CPU_NONE 4	/* CPU無し */
#define BUTTON_TIME_NONE 5	/* 制限時間なし */
#define BUTTON_TIME_TYPEA 6	/* 一手の時間制限 */
#define BUTTON_TIME_10UP 7
#define BUTTON_TIME_1UP 8
#define BUTTON_TIME_10DOWN 9
#define BUTTON_TIME_1DOWN 10
#define BUTTON_MATTA_OFF 11
#define BUTTON_MATTA_ON 12
#define BUTTON_MATTA_INF 13
#define BUTTON_MATTA_1UP 14
#define BUTTON_MATTA_1DOWN 15
#define BUTTON_GAME_START 16

#define BUTTON_GO_GAMESETTING 17	/* タイトル画面　はじめから */
#define BUTTON_CONTINUE 18	/* タイトル画面　続きから */
#define BUTTON_REPLAY 19	/* タイトル画面　リプレイ */

#define BUTTON_BACK_SCENE 20	/* 戻るボタン */
#define BUTTON_PREV_MAP 21

#define BUTTON_EXIT 22
#define BUTTON_NUM 22+1

#define BUTTON_INVISIBLE -1
#define BUTTON_DISABLE 0
#define BUTTON_ENABLE 1

/* デバッグモード */
#define DEBUG_OFF 0
#define DEBUG_ON 1

/* 先読みの手数 */
#define DEPTH 6

/* 無限 */
#define INF 1000

/* ゲームバージョン */
#define GAMEVERSION "2.00"

typedef struct but_status
{
	int x1;
	int y1;
	int x2;
	int y2;
	int button_flag = BUTTON_INVISIBLE;
	int img;
	int img_disable;
	char text[90];
	int font;
	int button_icon;
	bool icon;
}Button_status;

typedef struct g_status
{
	char map[8][8];	/* マップ */
	char startmap[8][8];	/* 初期マップ */
	int flipmap[8][8] = { 0 };	/* 裏返し可能枚数 */
	int flipdir[8] = { 0 };
	int kakutei[8][8] = { 0 };	/* 確定石 0:未確定 1:置けば確定 2:確定石*/
	int aimap[8][8] = { 0 };
	int mapscore[8][8] = { 0 };	/* シミュレーション結果 */
	int history[REV_DATASIZE][11] = { 0 };	/* バックアップデータ */
	double timehistory[REV_DATASIZE] = { 0 };	/* 時間データの格納 */
	int passflag = 1;	/* パスフラグ */
	int player = 1;	/* 現在プレイヤー */
	int cpu = 0;
	int ai_flag;	/* CPUモード 0:序盤 1:中盤 2:終盤1 3:終盤2 */
	int komaA;	/* 黒枚数 */
	int komaB;	/* 白枚数 */
	int tekazu = 0;	/* 手数 */
	int max_tekazu = 0;	/* リプレイ使用時の最大手数 */
	int putX;
	int putY;
	int prevputX;
	int prevputY;
	int mouseX;
	int mouseY;
	int clickX = -1;
	int clickY = -1;
	int cannotputflag = PUT_SUCCESSFUL;	/* 設置失敗のフラグ */
	int phase = PHASE_TITLE_INIT;	/* フェーズ */
	int blackImage;
	int whiteImage;
	int mapImage;
	int blackGimage;
	int whiteGimage;
	int blackPutimage;
	int whitePutimage;
	int black_status_area;
	int white_status_area;
	int background;
	int messagebox;
	int selectedImage;
	int clickedImage;
	int clickflag = CLICK_NOTCLICK;
	int gonextflag = 0;
	int mouseonflag = MOUSE_NOTON;
	int loadingImage;

	int GameStatus = GAME_TITLE;	/* ゲームの状態 */

	HANDLE FontFile;

	int FontData1;
	int FontData2;
	int FontData3;
	int FontData4;

	int sound1;
	int sound2;
	int sound3;
	int sound4;
	int sound5;
	int sound6;

	int soundflag = 1;

	struct but_status Button[BUTTON_NUM];

	int b_mouseon = -1;
	int b_clickon = -1;
	int b_conf = -1;	/* 確定 */

	int DebugMode = DEBUG_OFF;

	int com_count = 0;

	double time_rest[2] = { 0 };
	double time_max = TIME_DEFAULT;
	double time_used[2] = { 0 };
	double time_used_prev[2] = { 0 };	/* 先程の手の思考時間 */
	int time_used_total[2][3] = { 0 };	/* 思考時間合計 (分:秒.ミリ秒) */
	int time_num;	/* 稼働中のタイマー */
	int StartTime;
	int TimeupFlag = 0;
	int matta_rest = MATTA_DEFAULT;
	int Winner = 0;	/* 勝者 */
	DATEDATA StartTimeData;
	DATEDATA EndTimeData;

	/* 設定 */
	int time_setting = (int)TIME_DEFAULT;
	int matta_setting = MATTA_DEFAULT;
	int use_timer = 0;	/* -1:時間制限なし 0:一手の時間制限 */
	int matta_enable = -1;	/* -1:待ったなし 0:待ったあり（制限あり） 1:待った無制限 */
	int time_max_setting = 60;
	int time_min_setting = 10;
	int matta_max_setting = 10;
	int matta_min_setting = 1;

	bool init_ok_flag = false;

	int continueflag = 0;
	int exitflag = 0;
}Game_status;

typedef struct ip_status
{
	int MouseX = 0;
	int MouseY = 0;
	int click = 0;
	int wheel = 0;
	int clickedflag = 0;
}Input_status;

/* 初期化 */
int game_initialize(g_status* status);

/* 計算 */
void game_update(g_status* status, ip_status i_status);

/* 描画 */
void game_draw(g_status status, ip_status i_status);

/* 終了処理 */
void game_finalize(g_status* status);

/* MAPファイル読み込み */
int read_map_file(const char* fname, char map[8][8]);

/* AI用マップ読み込み */
int read_aimap_file(const char* fname, int map[8][8]);


void setting_file_load(const char* fname, int* setting_time, int* timer_enable, int* setting_matta, int* matta_enable);


void setting_file_save(const char* fname, int* setting_time, int* timer_enable, int* setting_matta, int* matta_enable);

/* コマ設置可能チェック(戻り値:裏返し枚数) */
int can_put_koma(int player, int x, int y, char map[8][8]);

/* 全体のコマ設置可能チェック */
int all_can_put_koma(int player, char map[8][8], int flipmap[8][8]);

/* 裏返し処理 */
int flip_koma(int player, char map[8][8], int flipdir[8], int x, int y);

/* コマカウント */
void count_koma(int *black, int *white, char map[8][8]);


/* バックアップ */

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
);

/* バックアップ復元 */

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
);

int backup_check(char startmap[8][8], char map[8][8], int history[REV_DATASIZE][11], int tekazu);


/* リプレイデータ作成 */
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
);

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
);

int replay_check(char startmap[8][8], char map[8][8], int history[REV_DATASIZE][11], int tekazu);

/* ボタン初期設定 */
void button_init(but_status* button, int x1, int y1, int x2, int y2, const char* image, const char* text, int fontsize);

/* ボタン初期設定（アイコン付き） */
void button_init_with_icon(but_status* button, int x1, int y1, int x2, int y2, const char* image, const char* icon, const char* text);

/* ボタン描画 */
void button_draw(g_status status);

/* マップクリック */
int point_map(g_status* status, ip_status i_status);

/* ボタンクリック */
int point_button(g_status* status, ip_status i_status);

/* AIコマ設置 */
int ai_put_koma(int com, int* putX, int* putY, char map[8][8], int flipmap[8][8], int* flag, int tekazu, int aimap[8][8], int pass, int* op_count, int mapscore[8][8]);


int ai_simulate_mid(int* x, int* y, int com, int player, int depth, char map[8][8], int aimap[8][8], int flipmap[8][8], int alpha, int beta, int pass, int* op_count, int mapscore[8][8]);


int ai_simulate_end(int* x, int* y, int com, int player, int depth, char map[8][8], int aimap[8][8], int flipmap[8][8], int pass, int* op_count, int mapscore[8][8]);


int map_score(int com, char map[8][8], int aimap[8][8], int flipmap[8][8]);


int next_map(int player, int x, int y, char map[8][8], char nextmap[8][8], int flipmap[8][8], int nextfmap[8][8]);


int prev_map(int player, int x, int y, char map[8][8], char prevmap[8][8], int flipmap[8][8], int prevfmap[8][8], int flipdir[8]);


int Matta(int* player, int com, int history[REV_DATASIZE][11], double time_history[REV_DATASIZE], char map[8][8], int flipmap[8][8], int* tekazu, int* matta_count, double time_used_prev[2], int time_used_total[2][3]);

/* 時間計算 */
void operate_time(int* minute, int* second, int* m_second);

/* マウス座標 */
void mouse_update(ip_status* i_status);

/* マウス情報描画（デバッグ用） */
void mouse_status_draw(ip_status i_status);

/* デバッグ描画 */
void debug_draw(g_status status, ip_status i_status);

/* FPS安定 */
bool FPSUpdate();

void FPSDraw();

void FPSWait();

#endif