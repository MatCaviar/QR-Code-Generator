/*1751533 计3 宋子帆*/
#include"../90-b0/90-b0.h"
#include"../90-b1/90-b1.h"

/******************************************************************************************
函数名称：行列数输入函数
功    能：输入行列数
输入参数：int *phang：指向总行数的指针
		  int *plie：指向总列数的指针
		  int hang_MIN：本游戏最小行数
		  int hang_MAX：本游戏最大行数
		  int lie_MIN：本游戏最小列数
		  int lie_MAX：本游戏最大列 数
返 回 值：void
******************************************************************************************/
void input(int *phang, int *plie, int hang_MIN, int hang_MAX, int lie_MIN, int lie_MAX) {
	cls();
	for (;;) {//获取行数及其输入错误处理
		gotoxy(0, 0);
		cout << "请输入行数(" << hang_MIN << "-" << hang_MAX << ")：";
		cin >> *phang;
		if (!cin.good() || *phang < hang_MIN || *phang>hang_MAX) {
			cin.clear();
			cin.ignore(1024, '\n');
			gotoxy(16, 0);
			cout << "                             ";
			continue;
		}
		break;
	}
	for (;;) {//获取列数及其输入错误处理
		gotoxy(0, 1);
		cout << "请输入列数(" << lie_MIN << "-" << lie_MAX << ")：";
		cin >> *plie;
		if (!cin.good() || *plie < lie_MIN || *plie>lie_MAX) {
			cin.clear();
			cin.ignore(1024, '\n');
			gotoxy(17, 1);
			cout << "                             ";
			continue;
		}
		break;
	}
}

/******************************************************************************************
函数名称：END函数
功    能：输入end以继续
输入参数：void
返 回 值：void
******************************************************************************************/
void getEND(void) {
	int X = 0, Y = 0;
	char e, n, d;
	cout << endl << endl << "本小题结束，请输入End继续...";
	getxy(X, Y);
	for (;;) {
		cin.clear();
		cin.ignore(1024, '\n');
		gotoxy(X, Y);
		cout << "                                                     ";
		gotoxy(X, Y);
		cin >> e >> n >> d;
		if ((e == 'e' || e == 'E') && (n == 'n' || n == 'N') && (d == 'd' || d == 'D'))
			break;
		cout << endl << "输入错误，请重请输入";
		Sleep(800);
		gotoxy(0, Y + 2);
		cout << "                                                    ";
	}
}

/******************************************************************************************
函数名称：鼠标及键盘选择函数
功    能：实现鼠标及键盘选择
输入参数：
返 回 值：
******************************************************************************************/
void keyboard_mouse_MOVE(int(*p)[10], char(*mark)[10], int hang, int lie, char select, int &H, int &L, int &allscore, int &target, int &flag_continue, int extra) {
	int X, Y;
	int ret, maction;
	int keycode1, keycode2;
	int temp_H = 0, temp_L = 0, flag_mouse = 0;

	ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);//读取

	if (ret == CCT_MOUSE_EVENT) {//鼠标事件
		for (temp_H = 0, flag_mouse = 0; temp_H < hang; temp_H++)
			for (temp_L = 0; temp_L < lie; temp_L++) {
				if (X >= (8 - extra * 2) * temp_L + 2 + extra * 2 && X <= (8 - extra * 2) * temp_L + 7 + extra * 2 && Y >= (4 - extra) * temp_H + 2 + extra && Y <= (4 - extra) * temp_H + 4 + extra)
					H = temp_H, L = temp_L, flag_mouse = 1;
			}
		setcolor(BG_COLOR, 15);
		gotoxy(0 + extra * 2, (4 - extra) * hang + 5 - extra);
		cout << "                                ";
		gotoxy(0 + extra * 2, (4 - extra) * hang + 5 - extra);
		if (flag_mouse&&p[H][L])
			cout << "[当前位置] " << setw(2) << char('A' + H) << " 行" << setw(2) << L << "列";//打印当前位置
		else
			cout << "[当前位置] 鼠标位置非法";
		switch (maction) {
		case MOUSE_LEFT_BUTTON_CLICK:			//按下左键选择
			Block_Select(p, mark, hang, lie, H, L, &allscore, target, &flag_continue, select);
			break;
		} //end of switch(maction)
	} //end of if (CCT_MOUSE_EVENT)
	else if (ret == CCT_KEYBOARD_EVENT) {//键盘事件
		switch (keycode1) {
		case '\r':	//回车
			Block_Select(p, mark, hang, lie, H, L, &allscore, target, &flag_continue, select);
			break;
		case 224:
			if (extra&&p[H][L])
				SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_CH, 0, "★", 0);
			else if(extra==0)
				SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, p[H][L] + 2, 15, p[H][L], 0, 1);
			switch (keycode2) {
			case KB_ARROW_UP://上
				H = H - 1;
				if (H < 0 || p[H][L] == 0)
					H = hang - 1;
				break;
			case KB_ARROW_DOWN://下
				H = H + 1;
				if (H > hang - 1) {
					for (H = 0;; H++)
						if (p[H][L])
							break;
				}
				break;
			case KB_ARROW_LEFT://左
				L = L - 1;
				if (L < 0)
					for (L = lie - 1;; L--)
						if (p[H][L])
							break;
				if(p[H][L]==0)
					for (;; L--)
						if (p[H][L])
							break;
				break;
			case KB_ARROW_RIGHT://右
				L = L + 1;
				if (L > lie - 1)
					for(L=0;;L++)
						if (p[H][L])
							break;
				if (p[H][L] == 0)
					for (;; L++)
						if (p[H][L])
							break;
				break;
			}//end of swicth(keycode1)

		}
	}

}

/******************************************************************************************
函数名称：鼠标及键盘取消选择函数
功    能：实现选择后用鼠标及键盘取消选择
输入参数：
返 回 值：
******************************************************************************************/
void keyboard_mouse_SELECT(int(*p)[10], char(*mark)[10], int hang, int lie, char select, int &H, int &L, int &allscore, int &target, int &flag_continue, int extra) {
	int X, Y;
	int ret, maction;
	int keycode1, keycode2;
	int temp_H = 0, temp_L = 0, flag_mouse = 0;

	for (;;) {
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);//读取

		if (ret == CCT_MOUSE_EVENT) {//鼠标事件
			for (temp_H = 0, flag_mouse = 0; temp_H < hang; temp_H++)
				for (temp_L = 0; temp_L < lie; temp_L++)
					if (X >= (8 - extra * 2) * temp_L + 2 + extra * 2 && X <= (8 - extra * 2) * temp_L + 7 + extra * 2 && Y >= (4 - extra) * temp_H + 2 + extra && Y <= (4 - extra) * temp_H + 4 + extra)
						if (temp_H != H || temp_L != L) {
							redrew(p, mark, hang, lie, extra);//放弃合成
							return;
						}
			if (maction == MOUSE_LEFT_BUTTON_CLICK) { //按下左键合成
				CLEAR_ASEMBLE_mark(p, mark, hang, lie, H, L, &allscore, target);
				return;
			}

		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {//键盘事件
			switch (keycode1) {
			case '\r':	//按下回车合成
				CLEAR_ASEMBLE_mark(p, mark, hang, lie, H, L, &allscore, target);
				return;
			case 224:
				if (extra)
					SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_CH, 0, "★", 0);
				else
					SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, p[H][L] + 2, 15, p[H][L], 0, 1);				switch (keycode2) {
					case KB_ARROW_UP://上
						redrew(p, mark, hang, lie, extra);//放弃合成
						H = H - 1;
						if (H < 0)
							H = hang - 1;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "★", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);
						return;
					case KB_ARROW_DOWN://下
						redrew(p, mark, hang, lie, extra);//放弃合成
						H = H + 1;
						if (H > hang - 1)
							H = 0;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "★", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);					return;
					case KB_ARROW_LEFT://左
						redrew(p, mark, hang, lie, extra);//放弃合成
						L = L - 1;
						if (L < 0)
							L = lie - 1;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "★", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);					return;
					case KB_ARROW_RIGHT://右
						redrew(p, mark, hang, lie, extra);//放弃合成
						L = L + 1;
						if (L > lie - 1)
							L = 0;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "★", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);					return;
				}//end of swicth(keycode1)

			}
		}
	}
}

/******************************************************************************************
函数名称：重绘函数（适用于取消选择）
功    能：重绘被标记的星星
输入参数：
返 回 值：
******************************************************************************************/
void redrew(int(*p)[10], char(*mark)[10], int hang, int lie, int extra) {
	int H = 0, L = 0;
	for (H = 0; H < hang; H++)
		for (L = 0; L < lie; L++)
			if (mark[H][L] == '*') {
				if (extra)
					SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_CH, 0, "★", 0);
				else
					SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, p[H][L] + 2, 15, p[H][L], 0, 1);
			}
}

/******************************************************************************************
函数名称：标记数组重置函数
功    能：重置标记数组
输入参数：
返 回 值：
******************************************************************************************/
void MARK_CLEAR(char(*mark)[10], int hang, int lie) {
	int H = 0, L = 0;
	for (H = 0; H < hang; H++)
		for (L = 0; L < lie; L++)
			mark[H][L] = '0';
}









