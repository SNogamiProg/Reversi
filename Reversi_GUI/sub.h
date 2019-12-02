#if !defined SUB_H
#define SUB_H

#define PI 3.14159265

/* �Q�[���̏�� */
#define GAME_TITLE 0	/* �^�C�g�� */
#define GAME_SETTING 1	/* �Q�[���ݒ� */
#define GAME_REPLAYLIST 2	/* ���v���C���X�g */
#define GAME_OPTION 3	/* �I�v�V���� */
#define GAME_INITIALIZE 4	/* �Q�[�������ݒ� */
#define GAME_PLAYING 5	/* �Q�[���� */
#define GAME_END 6	/* �Q�[���I�� */
#define GAME_REPLAY_INIT 7	/* ���v���C�����ݒ� */
#define GAME_REPLAY 8	/* ���v���C�Đ� */

/* �t�F�[�Y */
#define PHASE_TITLE_INIT 0
#define PHASE_TITLE 1

#define PHASE_SETTING_INIT 0
#define PHASE_SETTING 1

#define PHASE_NONE	-1	/* �Y���Ȃ� */
#define PHASE_INPUT_INIT 0	/* �ŏ��̏��� */
#define PHASE_INPUT 1	/* ���͑҂���� */
#define PHASE_FLIP 2	/* ���Ԃ����� */
#define PHASE_SUCCESSFUL 3	/* �ݒu���� */
#define PHASE_FAILURE 4	/* �ݒu���s */

/* �N���b�N�t���O */
#define CLICK_NOTCLICK 0	/* �N���b�N���ĂȂ� */
#define CLICK_CLICKING 1	/* �N���b�N�� */
#define MOUSE_NOTON 0	/* �}�E�X���悹�Ă��Ȃ� */
#define MOUSE_ON 1	/* �}�E�X���悹�Ă��� */

/* �ݒu���s */
#define PUT_SUCCESSFUL 0	/* �ݒu���� */
#define PUT_PASS 1	/* �p�X */
#define PUT_FAILURE 2	/* �ݒu���s */

/* �R�}�̐F */
#define BLACK 1
#define WHITE -1

/* �c�莞�ԃf�t�H���g */
#define TIME_DEFAULT 30.0

#define MATTA_DEFAULT 1

#define REV_DATASIZE 60

#define FONTSIZE 16

#define FONTSIZE1 48
#define FONTSIZE2 16
#define FONTSIZE3 36
#define FONTSIZE4 36

#define BUTTON_PASS 0	/* �p�X�����Ƃ���OK�{�^�� */
#define BUTTON_RETRY 1	/* ���g���C */

#define BUTTON_CPU_BLACK 2	/* CPU�� */
#define BUTTON_CPU_WHITE 3	/* CPU�� */
#define BUTTON_CPU_NONE 4	/* CPU���� */
#define BUTTON_TIME_NONE 5	/* �������ԂȂ� */
#define BUTTON_TIME_TYPEA 6	/* ���̎��Ԑ��� */
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

#define BUTTON_GO_GAMESETTING 17	/* �^�C�g����ʁ@�͂��߂��� */
#define BUTTON_CONTINUE 18	/* �^�C�g����ʁ@�������� */
#define BUTTON_REPLAY 19	/* �^�C�g����ʁ@���v���C */

#define BUTTON_BACK_SCENE 20	/* �߂�{�^�� */
#define BUTTON_PREV_MAP 21

#define BUTTON_EXIT 22
#define BUTTON_NUM 22+1

#define BUTTON_INVISIBLE -1
#define BUTTON_DISABLE 0
#define BUTTON_ENABLE 1

/* �f�o�b�O���[�h */
#define DEBUG_OFF 0
#define DEBUG_ON 1

/* ��ǂ݂̎萔 */
#define DEPTH 6

/* ���� */
#define INF 1000

/* �Q�[���o�[�W���� */
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
	char map[8][8];	/* �}�b�v */
	char startmap[8][8];	/* �����}�b�v */
	int flipmap[8][8] = { 0 };	/* ���Ԃ��\���� */
	int flipdir[8] = { 0 };
	int kakutei[8][8] = { 0 };	/* �m��� 0:���m�� 1:�u���Ίm�� 2:�m���*/
	int aimap[8][8] = { 0 };
	int mapscore[8][8] = { 0 };	/* �V�~�����[�V�������� */
	int history[REV_DATASIZE][11] = { 0 };	/* �o�b�N�A�b�v�f�[�^ */
	double timehistory[REV_DATASIZE] = { 0 };	/* ���ԃf�[�^�̊i�[ */
	int passflag = 1;	/* �p�X�t���O */
	int player = 1;	/* ���݃v���C���[ */
	int cpu = 0;
	int ai_flag;	/* CPU���[�h 0:���� 1:���� 2:�I��1 3:�I��2 */
	int komaA;	/* ������ */
	int komaB;	/* ������ */
	int tekazu = 0;	/* �萔 */
	int max_tekazu = 0;	/* ���v���C�g�p���̍ő�萔 */
	int putX;
	int putY;
	int prevputX;
	int prevputY;
	int mouseX;
	int mouseY;
	int clickX = -1;
	int clickY = -1;
	int cannotputflag = PUT_SUCCESSFUL;	/* �ݒu���s�̃t���O */
	int phase = PHASE_TITLE_INIT;	/* �t�F�[�Y */
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

	int GameStatus = GAME_TITLE;	/* �Q�[���̏�� */

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
	int b_conf = -1;	/* �m�� */

	int DebugMode = DEBUG_OFF;

	int com_count = 0;

	double time_rest[2] = { 0 };
	double time_max = TIME_DEFAULT;
	double time_used[2] = { 0 };
	double time_used_prev[2] = { 0 };	/* ����̎�̎v�l���� */
	int time_used_total[2][3] = { 0 };	/* �v�l���ԍ��v (��:�b.�~���b) */
	int time_num;	/* �ғ����̃^�C�}�[ */
	int StartTime;
	int TimeupFlag = 0;
	int matta_rest = MATTA_DEFAULT;
	int Winner = 0;	/* ���� */
	DATEDATA StartTimeData;
	DATEDATA EndTimeData;

	/* �ݒ� */
	int time_setting = (int)TIME_DEFAULT;
	int matta_setting = MATTA_DEFAULT;
	int use_timer = 0;	/* -1:���Ԑ����Ȃ� 0:���̎��Ԑ��� */
	int matta_enable = -1;	/* -1:�҂����Ȃ� 0:�҂�������i��������j 1:�҂��������� */
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

/* ������ */
int game_initialize(g_status* status);

/* �v�Z */
void game_update(g_status* status, ip_status i_status);

/* �`�� */
void game_draw(g_status status, ip_status i_status);

/* �I������ */
void game_finalize(g_status* status);

/* MAP�t�@�C���ǂݍ��� */
int read_map_file(const char* fname, char map[8][8]);

/* AI�p�}�b�v�ǂݍ��� */
int read_aimap_file(const char* fname, int map[8][8]);


void setting_file_load(const char* fname, int* setting_time, int* timer_enable, int* setting_matta, int* matta_enable);


void setting_file_save(const char* fname, int* setting_time, int* timer_enable, int* setting_matta, int* matta_enable);

/* �R�}�ݒu�\�`�F�b�N(�߂�l:���Ԃ�����) */
int can_put_koma(int player, int x, int y, char map[8][8]);

/* �S�̂̃R�}�ݒu�\�`�F�b�N */
int all_can_put_koma(int player, char map[8][8], int flipmap[8][8]);

/* ���Ԃ����� */
int flip_koma(int player, char map[8][8], int flipdir[8], int x, int y);

/* �R�}�J�E���g */
void count_koma(int *black, int *white, char map[8][8]);


/* �o�b�N�A�b�v */

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

/* �o�b�N�A�b�v���� */

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


/* ���v���C�f�[�^�쐬 */
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

/* �{�^�������ݒ� */
void button_init(but_status* button, int x1, int y1, int x2, int y2, const char* image, const char* text, int fontsize);

/* �{�^�������ݒ�i�A�C�R���t���j */
void button_init_with_icon(but_status* button, int x1, int y1, int x2, int y2, const char* image, const char* icon, const char* text);

/* �{�^���`�� */
void button_draw(g_status status);

/* �}�b�v�N���b�N */
int point_map(g_status* status, ip_status i_status);

/* �{�^���N���b�N */
int point_button(g_status* status, ip_status i_status);

/* AI�R�}�ݒu */
int ai_put_koma(int com, int* putX, int* putY, char map[8][8], int flipmap[8][8], int* flag, int tekazu, int aimap[8][8], int pass, int* op_count, int mapscore[8][8]);


int ai_simulate_mid(int* x, int* y, int com, int player, int depth, char map[8][8], int aimap[8][8], int flipmap[8][8], int alpha, int beta, int pass, int* op_count, int mapscore[8][8]);


int ai_simulate_end(int* x, int* y, int com, int player, int depth, char map[8][8], int aimap[8][8], int flipmap[8][8], int pass, int* op_count, int mapscore[8][8]);


int map_score(int com, char map[8][8], int aimap[8][8], int flipmap[8][8]);


int next_map(int player, int x, int y, char map[8][8], char nextmap[8][8], int flipmap[8][8], int nextfmap[8][8]);


int prev_map(int player, int x, int y, char map[8][8], char prevmap[8][8], int flipmap[8][8], int prevfmap[8][8], int flipdir[8]);


int Matta(int* player, int com, int history[REV_DATASIZE][11], double time_history[REV_DATASIZE], char map[8][8], int flipmap[8][8], int* tekazu, int* matta_count, double time_used_prev[2], int time_used_total[2][3]);

/* ���Ԍv�Z */
void operate_time(int* minute, int* second, int* m_second);

/* �}�E�X���W */
void mouse_update(ip_status* i_status);

/* �}�E�X���`��i�f�o�b�O�p�j */
void mouse_status_draw(ip_status i_status);

/* �f�o�b�O�`�� */
void debug_draw(g_status status, ip_status i_status);

/* FPS���� */
bool FPSUpdate();

void FPSDraw();

void FPSWait();

#endif