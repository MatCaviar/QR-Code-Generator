/*1751533 计3 宋子帆*/
#include"../90-b0/90-b0.h"
#include"../90-b1/90-b1.h"
/******************************************************************************************
函数名称：菜单项打印及选择函数
功    能：菜单项打印及选择
输入参数：char content:菜单项内容
		  int xiangshu:菜单项内容的行数
		  char char_select[]:菜单选择字符
		  char char_exit:退出字符
返 回 值：输入的字符（0-9,A-Z,a-z）
******************************************************************************************/
char Menu(char *content[], int xiangshu, char char_select[], char char_exit) {
	char select;
	int str_max = 0, i = 0;

	cls();
	setconsoleborder(100, 30, 100, 30);
	setcursor(10);
	setcolor(0, 15);
	for (i = 0; i < xiangshu; i++) //计算菜单项最大长度
		if (strlen(content[i]) > unsigned(str_max))
			str_max = int(strlen(content[i]));
	for (i = 0; i < str_max; i++)
		cout << "-";
	cout << endl;
	for (i = 0; i < xiangshu - 1; i++)
		cout << content[i] << endl;
	for (i = 0; i < str_max; i++)
		cout << "-";
	cout << endl;
	cout << content[xiangshu - 1] << endl;

	for (;;) {//选择菜单项
		select = _getch();
		if (select >= 'a'&&select <= 'z')
			select = select - 'a' + 'A';
		for (i = 0; char_select[i] != 0; i++) 
			if (select == char_select[i])
				return select;		
		if (select == char_exit)
			return select;
		if ((char_exit >= 'A'&&char_exit <= 'Z') && select == (char_exit + 'a' - 'A'))
			return select;
	}

	return select;
}

/******************************************************************************************
函数名称：查找指定坐标位置及周围相同项函数(递归方式)
功    能：查找指定坐标位置及周围相同项
输入参数：int(*p)[10]：内部数组一
		  char(*mark)[10]：作为标记的内部数组二
		  int hang：总行数
		  int lie：总列数
		  int HANG：行坐标
		  int LIE：列坐标
返 回 值：void
******************************************************************************************/
void Recursive_MARK_S(int(*p)[10], char(*mark)[10], int hang, int lie, int HANG, int LIE) {
	int flag, H, L;
	for (H = 0; H < 8; H++)//初始化标记数组，（均赋值为'0'），便于区分
		for (L = 0; L < 10; L++) {
			mark[H][L] = '0';
		}
	mark[HANG][LIE] = '*';
	for (flag = 1; flag > 0;)
		for (flag = 0, HANG = 0; HANG < hang; HANG++)
			for (LIE = 0; LIE < lie; LIE++) {
				if ('*' == mark[HANG][LIE]) {
					if (HANG - 1 >= 0 && p[HANG - 1][LIE] == p[HANG][LIE] && mark[HANG - 1][LIE] != '*'&&p[HANG - 1][LIE])//比对上面元素
						flag++, mark[HANG - 1][LIE] = '*';
					if (HANG + 1 < hang && p[HANG + 1][LIE] == p[HANG][LIE] && mark[HANG + 1][LIE] != '*'&& p[HANG + 1][LIE])//比对下面元素
						flag++, mark[HANG + 1][LIE] = '*';
					if (LIE - 1 >= 0 && p[HANG][LIE - 1] == p[HANG][LIE] && mark[HANG][LIE - 1] != '*'&&p[HANG][LIE - 1])//比对左面元素
						flag++, mark[HANG][LIE - 1] = '*';
					if (LIE + 1 < lie && p[HANG][LIE + 1] == p[HANG][LIE] && mark[HANG][LIE + 1] != '*'&&p[HANG][LIE + 1])//比对右面元素
						flag++, mark[HANG][LIE + 1] = '*';
				}
			}
}

/******************************************************************************************
函数名称：游戏（合成十）/本关（消灭星星）结束判断函数
功    能：判断整个游戏（合成十）/本关（消灭星星）是否结束
输入参数：int(*p)[10]：内部数组一
		  int hang：总行数
		  int lie：总列数
返 回 值：1：未结束
		  0：结束
******************************************************************************************/
int judge_FALSE(int(*p)[10], int hang, int lie) {
	int flag = 0, H, L;
	for (H = 0, flag = 0; H < hang; H++)
		for (L = 0; L < lie; L++) {
			if (H - 1 >= 0 && p[H - 1][L] == p[H][L]&& p[H - 1][L])//比对上面元素
				flag++;
			if (H + 1 < hang && p[H + 1][L] == p[H][L]&& p[H + 1][L])//比对下面元素
				flag++;
			if (L - 1 >= 0 && p[H][L - 1] == p[H][L]&& p[H][L - 1])//比对左面元素
				flag++;
			if (L + 1 < lie && p[H][L + 1] == p[H][L]&& p[H][L + 1])//比对右面元素
				flag++;
		}
	if (flag > 0)
		return 1;
	else
		return 0;

}

/******************************************************************************************
函数名称：内部数组位置移动函数
功    能：对内部数组进行位置移动
输入参数：int(*p)[10]：内部数组一
		  int move_mode：移动方式选择  1  从上至下单非零填充
									   2  从下至上单非零填充
									   3  从左至右单非零填充
									   4  从右至左单非零填充
									   5  从右至左整列移动到全零列
		  int hang：总行数
		  int lie：总列数
返 回 值：void
******************************************************************************************/
void Array_MOVE(int(*p)[10], int move_mode, int hang, int lie) {
	int H = 0, L = 0, n = 0,flag=0,MARK_L;
	if (1 == move_mode)
		for (n = 0; n < 10; n++)//从上至下单非零填充
			for (H = 0; H < hang - 1; H++)
				for (L = 0; L < lie; L++)
					if (0 == p[H + 1][L]) {
						p[H + 1][L] = p[H][L];
						p[H][L] = 0;
					}
	if (2 == move_mode)
		for (n = 0; n < 10; n++)//从下至上单非零填充
			for (H = 1; H < hang; H++)
				for (L = 0; L < lie; L++)
					if (0 == p[H - 1][L]) {
						p[H - 1][L] = p[H][L];
						p[H][L] = 0;
					}
	if (3 == move_mode)
		for (n = 0; n < 10; n++)//从左至右单非零填充
			for (H = 0; H < hang; H++)
				for (L = 0; L < lie - 1; L++)
					if (0 == p[H][L + 1]) {
						p[H][L + 1] = p[H][L];
						p[H][L] = 0;
					}
	if (4 == move_mode)
		for (n = 0; n < 10; n++)//从右至左单非零填充
			for (H = 0; H < hang; H++)
				for (L = 1; L < lie; L++)
					if (0 == p[H][L - 1]) {
						p[H][L - 1] = p[H][L];
						p[H][L] = 0;
					}
	if(5 == move_mode) {//从右至左整列移动到全零列
		for (n = 0; n<10; n++)
			for (L = 0; L < lie; L++) {
				for (H = 0, flag = 0; H < hang; H++)
					if (p[H][L] == 0)
						flag++;
				if (flag == hang)//若某列全为零
					for (MARK_L = L; MARK_L < lie - 1; MARK_L++)
						for (H = 0; H < hang; H++)
							p[H][MARK_L] = p[H][MARK_L + 1], p[H][MARK_L + 1] = 0;
			}
	}

}

/******************************************************************************************
函数名称：内部数组打印
功    能：命令行下打印数组（包括反显、特殊位置标注等）
输入参数：int(*p)[10]：内部数组一
		  char(*mark)[10]：作为标记的内部数组二
		  int hang：总行数
		  int lie：总列数
		  int output_MODE：选择打印方式  0  将0以不同颜色标出并打印数组
										 1  缺省打印
										 2  将被标记的位置用不同颜色标出并打印数组
		  char *sentence：可能需要同时打印的内容（若为0则不打印）
返 回 值：void
******************************************************************************************/
void array_output(int(*p)[10], char(*mark)[10], int hang, int lie, int output_MODE, char *sentence) {
	int H, L;
	if (sentence != 0)
		cout << endl << endl << sentence << endl;
	cout << "  |";
	for (L = 0; L < lie; L++)
		cout << "  " << L;
	cout << endl << "--+";
	for (H = 0; H < 3 * lie + 1; H++)
		cout << "-";
	for (H = 0; H < hang; H++) {
		cout << endl << char('A' + H) << " |";
		for (L = 0; L < lie; L++) {
			if (output_MODE == 1)
				cout << "  " << p[H][L];

			if (output_MODE == 0) {
				if (0 == p[H][L]) {
					mark[H][L] = '*';
					setcolor(0, 11);
					cout << "  " << p[H][L];
					setcolor(0, 7);
				}
				else {
					cout << "  " << p[H][L];
				}
			}

			if (output_MODE == 2) {
				if ('*' == mark[H][L]) {
					setcolor(0, 11);
					cout << "  " << p[H][L];
					setcolor(0, 7);
				}
				else {
					cout << "  " << p[H][L];
				}
			}
		}
	}
}

/******************************************************************************************
函数名称：状态栏显示函数（顶部）
功    能：显示状态栏内容
输入参数：int tempscore：本次合成得分
		  int *allscore：总得分
		  int target：目标分数
返 回 值：void
******************************************************************************************/
void Status_bar_TOP(int tempscore, int *allscore, int target) {
	gotoxy(0, 0);
	setcolor(BG_COLOR, 15);
	cout << "                                                   ";
	gotoxy(0, 0);
	cout << "本次得分：" << tempscore << " 总得分：" << *allscore<< " 本关目标：" << target;
}

/******************************************************************************************
函数名称：状态栏显示函数（底部）
功    能：显示状态栏内容
输入参数：
返 回 值：void
******************************************************************************************/
void Status_bar_BOTTLE(int(*p)[10],int H,int L,int print_mode,int X,int Y, char*ch, int remain, int extra_score, int allscore) {
	gotoxy(X, Y);
	setcolor(BG_COLOR, 15);
	cout << "                                         ";
	gotoxy(X, Y);
	if (print_mode)
		cout << "本关结束，还剩下" << remain << "个星星,额外加分：" << extra_score << "总得分：" << allscore << endl;
	else
		cout << ch;

}

/******************************************************************************************
函数名称：初始外框架打印函数
功    能：显示初始外框架
输入参数：int hang：总行数
		  int lie：总列数
		  int line_NUM：是否显示行标列标  0  不显示
										  1   显示
		  int Solid_Hollow：空心/实心边框线  0  空心
											 1  实心
		  int Divider：是否含分割线  0  不含
									 1   含
		  int Extra_RIGHT：右侧是否有额外框架  0     无
		                                       数字  额外框架的宽度
返 回 值：void
******************************************************************************************/
void Border_PRINT(int hang, int lie, int line_NUM, int Solid_Hollow, int Divider,int Extra_RIGHT) {
	int H = 0, L = 0, X = 0, Y = 0, n = 0;
	cls();
	setconsoleborder((6 + Divider * 2) * lie + 5 + line_NUM * 2+ Extra_RIGHT, (3 + Divider) * hang + 7 + line_NUM, (6 + Divider * 2) * lie + 5 + line_NUM * 2 + Extra_RIGHT, (3 + Divider) * hang + 7 + line_NUM);
	cout << "屏幕当前设置为：" << (3 + Divider) * hang + 7 + line_NUM << "行" << (6 + Divider * 2) * lie + 5 + line_NUM * 2 + Extra_RIGHT << "列" << endl;
	setcursor(CURSOR_VISIBLE_FULL);
	setcolor(15, 2);
	if (line_NUM) {//打印行标列标
		for (n = 0; n < lie; n++)
			showch((6 + 2 * Divider)*n + 6, 1, n + '0', 0 + n, 7 + n, 1);
		for (n = 0; n < hang; n++)
			showch(0, (3 + Divider)*n + 4, n + 'A', 0 + n, 7 + n, 1);
	}
	if (Solid_Hollow) {//实心线
		if (Divider) {//有分割线
			setcolor(15, 2);
			cout << "┏━━━";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "┳━━━";
			}
			cout << "┓" << endl;
			for (n = 0; n < 3; n++) {
				Sleep(2);
				cout << "┃";
				setcolor(15, 2);
				cout << "      ";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "┃";
					setcolor(15, 2);
					cout << "      ";
				}
				cout << "┃" << endl;
			}
			for (H = 1; H < hang; H++) {
				Sleep(2);
				cout << "┣━━━";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "╋━━━";
				}
				cout << "┫" << endl;
				for (n = 0; n < 3; n++) {
					Sleep(2);
					cout << "┃";
					setcolor(15, 2);
					cout << "      ";
					for (L = 1; L < lie; L++) {
						Sleep(2);
						cout << "┃";
						setcolor(15, 2);
						cout << "      ";
					}
					cout << "┃" << endl;
				}
			}
			cout << "┗━━━";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "┻━━━";
			}
			cout << "┛" << endl;
		}
		else {
			setcolor(15, 2);
			cout << "┏";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "━";
			}
			cout << "┓";
			for (H = 0; H < hang; H++) {
				setcolor(15, 2);
				gotoxy(0, 3 * H + 2);
				cout << "┃";
				gotoxy(0, 3 * H + 3);
				cout << "┃";
				gotoxy(0, 3 * H + 4);
				cout << "┃";
				gotoxy(2, 3 * H + 2);
				gotoxy(6 * lie + 2, 3 * H + 2);
				cout << "┃";
				gotoxy(6 * lie + 2, 3 * H + 3);
				cout << "┃";
				gotoxy(6 * lie + 2, 3 * H + 4);
				cout << "┃";
			}
			gotoxy(0, 3 * hang + 2);
			cout << "┗";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "━";
			}
			cout << "┛" << endl;
		}
	}
	else {//空心线
		if (Divider) {//有分割线
			setcolor(15, 2);
			gotoxy(0 + 2 * line_NUM, 1 + line_NUM);
			cout << "╔═══";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "╦═══";
			}
			cout << "╗";
			for (n = 0; n < 3; n++) {
				Sleep(2);
				gotoxy(0 + 2 * line_NUM, 2 + line_NUM + n);
				cout << "║";
				setcolor(15, 2);
				cout << "      ";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "║";
					cout << "      ";
				}
				cout << "║" ;
			}
			for (H = 1; H < hang; H++) {
				Sleep(2);
				gotoxy(0 + 2 * line_NUM, 5 + line_NUM + 4*(H-1));
				cout << "╠═══";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "╬═══";
				}
				cout << "╣" ;
				getxy(X, Y);
				for (n = 0; n < 3; n++) {
					gotoxy(0 + 2 * line_NUM, Y + 1 + n);
					Sleep(2);
					cout << "║";
					setcolor(15, 2);
					cout << "      ";
					for (L = 1; L < lie; L++) {
						Sleep(2);
						cout << "║";
						cout << "      ";
					}
					cout << "║" ;
				}
			}
			gotoxy(0 + 2 * line_NUM, 1+line_NUM + 4 * hang);
			cout << "╚═══";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "╩═══";
			}
			cout << "╝" ;
		}
		else {
	
			setcolor(15, 2);
			gotoxy(0 + 2 * line_NUM, 1 + line_NUM);
			cout << "╔";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "═";
			}
			cout << "╗";
			for (H = 0; H < hang; H++) {
				setcolor(15, 2);
				gotoxy(0+2*line_NUM, 3 * H + 2+ line_NUM);
				cout << "║";
				gotoxy(0 + 2 * line_NUM, 3 * H + 3+ line_NUM);
				cout << "║";
				gotoxy(0 + 2 * line_NUM, 3 * H + 4+ line_NUM);
				cout << "║";
				gotoxy(6 * lie + 2 + 2 * line_NUM, 3 * H + 2 + line_NUM);
				cout << "║";
				gotoxy(6 * lie + 2 + 2 * line_NUM, 3 * H + 3 + line_NUM);
				cout << "║";
				gotoxy(6 * lie + 2 + 2 * line_NUM, 3 * H + 4 + line_NUM);
				cout << "║";
			}
			gotoxy(0 + 2 * line_NUM, 3 * hang + 2 + line_NUM);
			cout << "╚";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "═";
			}
			cout << "╝" ;
		}	
		
	}
	setcolor();
	setcursor(10);
}

/******************************************************************************************
函数名称：单个色块打印函数
功    能：画一个色块
输入参数：int G：色块高度
		  int K：色块宽度
		  int X：坐标X
		  int Y：坐标Y
		  int bg：背景色
		  int fg：前景色
		  int NUM：色块中央打印的内容（数字，若为0则不打印）
		  char CH：色块中央打印的内容（字符，若为0则不打印）
		  int Solid_Hollow：空心/实心边框线  0  空心
											 1  实心
返 回 值：void
******************************************************************************************/
void SingleBlock(int G, int K, int X, int Y, int bg, int fg, int NUM, char *CH, int Solid_Hollow) {
	int i;
	gotoxy(X, Y);
	setcolor(bg, fg);

	if (Solid_Hollow == 0) {//空心线
		cout << "╔";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "═";
		cout << "╗";

		for (i = 0; i < G - 2; i++) {
			gotoxy(X, Y + 1 + i);
			cout << "║";
		}

		gotoxy(X + 2 + (K - 6) / 2, Y + (G - 1) / 2);
		if (NUM)
			cout  <<setw(2)<< NUM;
		if (CH)
			cout << CH;
		for (i = 0; i < G - 2; i++) {
			gotoxy(X + K - 2, Y + 1 + i);
			cout << "║";//中间;
		}

		gotoxy(X, Y + G - 1);
		cout << "╚";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "═";
		cout << "╝";
	}
	else {//实心线
		cout << "┏";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "━";
		cout << "┓";

		for (i = 0; i < G - 2; i++) {
			gotoxy(X, Y + 1 + i);
			cout << "┃";
		}

		gotoxy(X + 2 + (K - 6) / 2, Y + (G - 1) / 2);
		if (NUM)
			cout << setw(2) << NUM;
		if (CH)
			cout << CH;
		for (i = 0; i < G - 2; i++) {
			gotoxy(X + K - 2, Y + 1 + i);
			cout << "┃" ;//中间
		}

		gotoxy(X, Y + G - 1);
		cout << "┗";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "━";
		cout << "┛";
	}

}

/******************************************************************************************
函数名称：色块移动函数
功    能：移动一个色块
输入参数：int(*p)[10]：内部数组一
          int hang：总行数
		  int lie：总列数
          int move_mode：移动方式选择  1  从上至下单非零填充(有分割线无行标列标)
                                       2  从下至上单非零填充
                                       3  从左至右单非零填充
                                       4  从右至左单非零填充
                                       5  从右至左整列移动到全零列
									   6  从上至下单非零填充(无分割线有行标列标)
          int G：色块高度
	      int K：色块宽度
		  int NUM：色块中央打印的内容（数字，若为0则不打印）
		  char CH：色块中央打印的内容（字符，若为0则不打印）
		  int Solid_Hollow：空心/实心边框线  0  空心
		                                     1  实心
返 回 值：void
******************************************************************************************/
void SingleBlock_MOVE(int(*p)[10], int hang, int lie, int move_mode, int G, int K,int NUM,char *CH,int Solid_Hollow,int H,int L) {
	int X=0,Y = 0,n=0,flag=0,MARK_L=0,i=0;
	if(move_mode==1)//从上到下单个色块移动(有分割线无行标列标)
	for (Y = 4 * H + 2; Y < 4 * (H + 1) + 3; Y++) {
		Sleep(6);
		SingleBlock(G, K, 8 * L + 2, Y, p[H][L] + 2, 15, NUM, CH, Solid_Hollow);
		if (Y - 1 > 1) {
			gotoxy(8 * L + 2, Y - 1);
			setcolor(15, 2);
			cout << "      ";
		}
	}
	if (move_mode == 6)//从上到下单个色块移动(无分割线有行标列标)
		for (Y = 3 * H + 3; Y <= 3 * H + 5; Y++) {
			Sleep(6);
			SingleBlock(G, K, 6 * L + 4, Y+1, p[H][L] + COLOR, COLOR_CH, NUM, CH, Solid_Hollow);
				gotoxy(6 * L + 4, Y);
				setcolor(15, 2);
				cout << "      ";
		}
	if (5 == move_mode) {//从右至左整列移动到全零列
		for (n = 0; n<10; n++)
			for (L = 0; L < lie; L++) {
				for (H = 0, flag = 0; H < hang; H++)
					if (p[H][L] == 0)
						flag++;
				if (flag == hang)//若某列全为零
					for (MARK_L = L; MARK_L < lie - 1; MARK_L++)
						for (H = 0; H < hang; H++) {
							for (i = 1; i <= 6&&p[H][MARK_L+1];i++) {
								Sleep(4);
								SingleBlock(G, K, 6*(MARK_L+1)+4-i, 3 * H + 3, p[H][MARK_L + 1] + COLOR, COLOR_CH, NUM, CH, Solid_Hollow);
								gotoxy(6*(MARK_L+1)+4-i+6,3*H+3);
								setcolor(15, 2);
								cout << " ";
								gotoxy(6 * (MARK_L + 1) + 4 - i + 6,3*H+4);
								cout << " ";
								gotoxy(6 * (MARK_L + 1) + 4 - i + 6,3*H+5);
								cout << " ";
							}				
							p[H][MARK_L] = p[H][MARK_L + 1], p[H][MARK_L + 1] = 0;

						}
			}
	}
}