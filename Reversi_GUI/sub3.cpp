/* 操作（キーボード、マウス）の処理 */

#pragma warning(disable: 4996)
#include "DxLib.h"
#include "sub.h"

/* マウス座標 */
void mouse_update(ip_status* i_status)
{
	i_status->wheel = 0;
	GetMousePoint(&i_status->MouseX, &i_status->MouseY);
	if ((GetMouseInput()&MOUSE_INPUT_LEFT) != 0)
	{
		i_status->click = 1;
	}
	else if ((GetMouseInput()&MOUSE_INPUT_RIGHT) != 0)
	{
		i_status->click = 2;
	}
	else
	{
		i_status->click = 0;
	}
	i_status->wheel += GetMouseWheelRotVol();
}

/* マウス情報描画（デバッグ用） */
void mouse_status_draw(ip_status i_status)
{
	unsigned int color = GetColor(0, 0, 0);
	switch (i_status.click)
	{
	case 1: color = GetColor(255, 0, 0); break;
	case 2: color = GetColor(0, 255, 0); break;
	default: break;
	}
	if ((i_status.click <= 2)&& (i_status.click > 0)) {
		DrawCircle(i_status.MouseX, i_status.MouseY, 16, color, TRUE);
	}
	DrawFormatString(i_status.MouseX, i_status.MouseY - 16, GetColor(255, 255, 0), "%d", i_status.wheel);
}