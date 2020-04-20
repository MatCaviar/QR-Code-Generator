/*1751533 ��3 ���ӷ�*/
#include"../90-b0/90-b0.h"
#include"../90-b1/90-b1.h"
/******************************************************************************************
�������ƣ��˵����ӡ��ѡ����
��    �ܣ��˵����ӡ��ѡ��
���������char content:�˵�������
		  int xiangshu:�˵������ݵ�����
		  char char_select[]:�˵�ѡ���ַ�
		  char char_exit:�˳��ַ�
�� �� ֵ��������ַ���0-9,A-Z,a-z��
******************************************************************************************/
char Menu(char *content[], int xiangshu, char char_select[], char char_exit) {
	char select;
	int str_max = 0, i = 0;

	cls();
	setconsoleborder(100, 30, 100, 30);
	setcursor(10);
	setcolor(0, 15);
	for (i = 0; i < xiangshu; i++) //����˵�����󳤶�
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

	for (;;) {//ѡ��˵���
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
�������ƣ�����ָ������λ�ü���Χ��ͬ���(�ݹ鷽ʽ)
��    �ܣ�����ָ������λ�ü���Χ��ͬ��
���������int(*p)[10]���ڲ�����һ
		  char(*mark)[10]����Ϊ��ǵ��ڲ������
		  int hang��������
		  int lie��������
		  int HANG��������
		  int LIE��������
�� �� ֵ��void
******************************************************************************************/
void Recursive_MARK_S(int(*p)[10], char(*mark)[10], int hang, int lie, int HANG, int LIE) {
	int flag, H, L;
	for (H = 0; H < 8; H++)//��ʼ��������飬������ֵΪ'0'������������
		for (L = 0; L < 10; L++) {
			mark[H][L] = '0';
		}
	mark[HANG][LIE] = '*';
	for (flag = 1; flag > 0;)
		for (flag = 0, HANG = 0; HANG < hang; HANG++)
			for (LIE = 0; LIE < lie; LIE++) {
				if ('*' == mark[HANG][LIE]) {
					if (HANG - 1 >= 0 && p[HANG - 1][LIE] == p[HANG][LIE] && mark[HANG - 1][LIE] != '*'&&p[HANG - 1][LIE])//�ȶ�����Ԫ��
						flag++, mark[HANG - 1][LIE] = '*';
					if (HANG + 1 < hang && p[HANG + 1][LIE] == p[HANG][LIE] && mark[HANG + 1][LIE] != '*'&& p[HANG + 1][LIE])//�ȶ�����Ԫ��
						flag++, mark[HANG + 1][LIE] = '*';
					if (LIE - 1 >= 0 && p[HANG][LIE - 1] == p[HANG][LIE] && mark[HANG][LIE - 1] != '*'&&p[HANG][LIE - 1])//�ȶ�����Ԫ��
						flag++, mark[HANG][LIE - 1] = '*';
					if (LIE + 1 < lie && p[HANG][LIE + 1] == p[HANG][LIE] && mark[HANG][LIE + 1] != '*'&&p[HANG][LIE + 1])//�ȶ�����Ԫ��
						flag++, mark[HANG][LIE + 1] = '*';
				}
			}
}

/******************************************************************************************
�������ƣ���Ϸ���ϳ�ʮ��/���أ��������ǣ������жϺ���
��    �ܣ��ж�������Ϸ���ϳ�ʮ��/���أ��������ǣ��Ƿ����
���������int(*p)[10]���ڲ�����һ
		  int hang��������
		  int lie��������
�� �� ֵ��1��δ����
		  0������
******************************************************************************************/
int judge_FALSE(int(*p)[10], int hang, int lie) {
	int flag = 0, H, L;
	for (H = 0, flag = 0; H < hang; H++)
		for (L = 0; L < lie; L++) {
			if (H - 1 >= 0 && p[H - 1][L] == p[H][L]&& p[H - 1][L])//�ȶ�����Ԫ��
				flag++;
			if (H + 1 < hang && p[H + 1][L] == p[H][L]&& p[H + 1][L])//�ȶ�����Ԫ��
				flag++;
			if (L - 1 >= 0 && p[H][L - 1] == p[H][L]&& p[H][L - 1])//�ȶ�����Ԫ��
				flag++;
			if (L + 1 < lie && p[H][L + 1] == p[H][L]&& p[H][L + 1])//�ȶ�����Ԫ��
				flag++;
		}
	if (flag > 0)
		return 1;
	else
		return 0;

}

/******************************************************************************************
�������ƣ��ڲ�����λ���ƶ�����
��    �ܣ����ڲ��������λ���ƶ�
���������int(*p)[10]���ڲ�����һ
		  int move_mode���ƶ���ʽѡ��  1  �������µ��������
									   2  �������ϵ��������
									   3  �������ҵ��������
									   4  �������󵥷������
									   5  �������������ƶ���ȫ����
		  int hang��������
		  int lie��������
�� �� ֵ��void
******************************************************************************************/
void Array_MOVE(int(*p)[10], int move_mode, int hang, int lie) {
	int H = 0, L = 0, n = 0,flag=0,MARK_L;
	if (1 == move_mode)
		for (n = 0; n < 10; n++)//�������µ��������
			for (H = 0; H < hang - 1; H++)
				for (L = 0; L < lie; L++)
					if (0 == p[H + 1][L]) {
						p[H + 1][L] = p[H][L];
						p[H][L] = 0;
					}
	if (2 == move_mode)
		for (n = 0; n < 10; n++)//�������ϵ��������
			for (H = 1; H < hang; H++)
				for (L = 0; L < lie; L++)
					if (0 == p[H - 1][L]) {
						p[H - 1][L] = p[H][L];
						p[H][L] = 0;
					}
	if (3 == move_mode)
		for (n = 0; n < 10; n++)//�������ҵ��������
			for (H = 0; H < hang; H++)
				for (L = 0; L < lie - 1; L++)
					if (0 == p[H][L + 1]) {
						p[H][L + 1] = p[H][L];
						p[H][L] = 0;
					}
	if (4 == move_mode)
		for (n = 0; n < 10; n++)//�������󵥷������
			for (H = 0; H < hang; H++)
				for (L = 1; L < lie; L++)
					if (0 == p[H][L - 1]) {
						p[H][L - 1] = p[H][L];
						p[H][L] = 0;
					}
	if(5 == move_mode) {//�������������ƶ���ȫ����
		for (n = 0; n<10; n++)
			for (L = 0; L < lie; L++) {
				for (H = 0, flag = 0; H < hang; H++)
					if (p[H][L] == 0)
						flag++;
				if (flag == hang)//��ĳ��ȫΪ��
					for (MARK_L = L; MARK_L < lie - 1; MARK_L++)
						for (H = 0; H < hang; H++)
							p[H][MARK_L] = p[H][MARK_L + 1], p[H][MARK_L + 1] = 0;
			}
	}

}

/******************************************************************************************
�������ƣ��ڲ������ӡ
��    �ܣ��������´�ӡ���飨�������ԡ�����λ�ñ�ע�ȣ�
���������int(*p)[10]���ڲ�����һ
		  char(*mark)[10]����Ϊ��ǵ��ڲ������
		  int hang��������
		  int lie��������
		  int output_MODE��ѡ���ӡ��ʽ  0  ��0�Բ�ͬ��ɫ�������ӡ����
										 1  ȱʡ��ӡ
										 2  ������ǵ�λ���ò�ͬ��ɫ�������ӡ����
		  char *sentence��������Ҫͬʱ��ӡ�����ݣ���Ϊ0�򲻴�ӡ��
�� �� ֵ��void
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
�������ƣ�״̬����ʾ������������
��    �ܣ���ʾ״̬������
���������int tempscore�����κϳɵ÷�
		  int *allscore���ܵ÷�
		  int target��Ŀ�����
�� �� ֵ��void
******************************************************************************************/
void Status_bar_TOP(int tempscore, int *allscore, int target) {
	gotoxy(0, 0);
	setcolor(BG_COLOR, 15);
	cout << "                                                   ";
	gotoxy(0, 0);
	cout << "���ε÷֣�" << tempscore << " �ܵ÷֣�" << *allscore<< " ����Ŀ�꣺" << target;
}

/******************************************************************************************
�������ƣ�״̬����ʾ�������ײ���
��    �ܣ���ʾ״̬������
���������
�� �� ֵ��void
******************************************************************************************/
void Status_bar_BOTTLE(int(*p)[10],int H,int L,int print_mode,int X,int Y, char*ch, int remain, int extra_score, int allscore) {
	gotoxy(X, Y);
	setcolor(BG_COLOR, 15);
	cout << "                                         ";
	gotoxy(X, Y);
	if (print_mode)
		cout << "���ؽ�������ʣ��" << remain << "������,����ӷ֣�" << extra_score << "�ܵ÷֣�" << allscore << endl;
	else
		cout << ch;

}

/******************************************************************************************
�������ƣ���ʼ���ܴ�ӡ����
��    �ܣ���ʾ��ʼ����
���������int hang��������
		  int lie��������
		  int line_NUM���Ƿ���ʾ�б��б�  0  ����ʾ
										  1   ��ʾ
		  int Solid_Hollow������/ʵ�ı߿���  0  ����
											 1  ʵ��
		  int Divider���Ƿ񺬷ָ���  0  ����
									 1   ��
		  int Extra_RIGHT���Ҳ��Ƿ��ж�����  0     ��
		                                       ����  �����ܵĿ��
�� �� ֵ��void
******************************************************************************************/
void Border_PRINT(int hang, int lie, int line_NUM, int Solid_Hollow, int Divider,int Extra_RIGHT) {
	int H = 0, L = 0, X = 0, Y = 0, n = 0;
	cls();
	setconsoleborder((6 + Divider * 2) * lie + 5 + line_NUM * 2+ Extra_RIGHT, (3 + Divider) * hang + 7 + line_NUM, (6 + Divider * 2) * lie + 5 + line_NUM * 2 + Extra_RIGHT, (3 + Divider) * hang + 7 + line_NUM);
	cout << "��Ļ��ǰ����Ϊ��" << (3 + Divider) * hang + 7 + line_NUM << "��" << (6 + Divider * 2) * lie + 5 + line_NUM * 2 + Extra_RIGHT << "��" << endl;
	setcursor(CURSOR_VISIBLE_FULL);
	setcolor(15, 2);
	if (line_NUM) {//��ӡ�б��б�
		for (n = 0; n < lie; n++)
			showch((6 + 2 * Divider)*n + 6, 1, n + '0', 0 + n, 7 + n, 1);
		for (n = 0; n < hang; n++)
			showch(0, (3 + Divider)*n + 4, n + 'A', 0 + n, 7 + n, 1);
	}
	if (Solid_Hollow) {//ʵ����
		if (Divider) {//�зָ���
			setcolor(15, 2);
			cout << "��������";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "�ש�����";
			}
			cout << "��" << endl;
			for (n = 0; n < 3; n++) {
				Sleep(2);
				cout << "��";
				setcolor(15, 2);
				cout << "      ";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "��";
					setcolor(15, 2);
					cout << "      ";
				}
				cout << "��" << endl;
			}
			for (H = 1; H < hang; H++) {
				Sleep(2);
				cout << "�ǩ�����";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "�贈����";
				}
				cout << "��" << endl;
				for (n = 0; n < 3; n++) {
					Sleep(2);
					cout << "��";
					setcolor(15, 2);
					cout << "      ";
					for (L = 1; L < lie; L++) {
						Sleep(2);
						cout << "��";
						setcolor(15, 2);
						cout << "      ";
					}
					cout << "��" << endl;
				}
			}
			cout << "��������";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "�ߩ�����";
			}
			cout << "��" << endl;
		}
		else {
			setcolor(15, 2);
			cout << "��";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "��";
			}
			cout << "��";
			for (H = 0; H < hang; H++) {
				setcolor(15, 2);
				gotoxy(0, 3 * H + 2);
				cout << "��";
				gotoxy(0, 3 * H + 3);
				cout << "��";
				gotoxy(0, 3 * H + 4);
				cout << "��";
				gotoxy(2, 3 * H + 2);
				gotoxy(6 * lie + 2, 3 * H + 2);
				cout << "��";
				gotoxy(6 * lie + 2, 3 * H + 3);
				cout << "��";
				gotoxy(6 * lie + 2, 3 * H + 4);
				cout << "��";
			}
			gotoxy(0, 3 * hang + 2);
			cout << "��";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "��";
			}
			cout << "��" << endl;
		}
	}
	else {//������
		if (Divider) {//�зָ���
			setcolor(15, 2);
			gotoxy(0 + 2 * line_NUM, 1 + line_NUM);
			cout << "�X�T�T�T";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "�j�T�T�T";
			}
			cout << "�[";
			for (n = 0; n < 3; n++) {
				Sleep(2);
				gotoxy(0 + 2 * line_NUM, 2 + line_NUM + n);
				cout << "�U";
				setcolor(15, 2);
				cout << "      ";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "�U";
					cout << "      ";
				}
				cout << "�U" ;
			}
			for (H = 1; H < hang; H++) {
				Sleep(2);
				gotoxy(0 + 2 * line_NUM, 5 + line_NUM + 4*(H-1));
				cout << "�d�T�T�T";
				for (L = 1; L < lie; L++) {
					Sleep(2);
					cout << "�p�T�T�T";
				}
				cout << "�g" ;
				getxy(X, Y);
				for (n = 0; n < 3; n++) {
					gotoxy(0 + 2 * line_NUM, Y + 1 + n);
					Sleep(2);
					cout << "�U";
					setcolor(15, 2);
					cout << "      ";
					for (L = 1; L < lie; L++) {
						Sleep(2);
						cout << "�U";
						cout << "      ";
					}
					cout << "�U" ;
				}
			}
			gotoxy(0 + 2 * line_NUM, 1+line_NUM + 4 * hang);
			cout << "�^�T�T�T";
			for (n = 1; n < lie; n++) {
				Sleep(2);
				cout << "�m�T�T�T";
			}
			cout << "�a" ;
		}
		else {
	
			setcolor(15, 2);
			gotoxy(0 + 2 * line_NUM, 1 + line_NUM);
			cout << "�X";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "�T";
			}
			cout << "�[";
			for (H = 0; H < hang; H++) {
				setcolor(15, 2);
				gotoxy(0+2*line_NUM, 3 * H + 2+ line_NUM);
				cout << "�U";
				gotoxy(0 + 2 * line_NUM, 3 * H + 3+ line_NUM);
				cout << "�U";
				gotoxy(0 + 2 * line_NUM, 3 * H + 4+ line_NUM);
				cout << "�U";
				gotoxy(6 * lie + 2 + 2 * line_NUM, 3 * H + 2 + line_NUM);
				cout << "�U";
				gotoxy(6 * lie + 2 + 2 * line_NUM, 3 * H + 3 + line_NUM);
				cout << "�U";
				gotoxy(6 * lie + 2 + 2 * line_NUM, 3 * H + 4 + line_NUM);
				cout << "�U";
			}
			gotoxy(0 + 2 * line_NUM, 3 * hang + 2 + line_NUM);
			cout << "�^";
			for (n = 0; n < 3 * lie; n++) {
				Sleep(5);
				cout << "�T";
			}
			cout << "�a" ;
		}	
		
	}
	setcolor();
	setcursor(10);
}

/******************************************************************************************
�������ƣ�����ɫ���ӡ����
��    �ܣ���һ��ɫ��
���������int G��ɫ��߶�
		  int K��ɫ����
		  int X������X
		  int Y������Y
		  int bg������ɫ
		  int fg��ǰ��ɫ
		  int NUM��ɫ�������ӡ�����ݣ����֣���Ϊ0�򲻴�ӡ��
		  char CH��ɫ�������ӡ�����ݣ��ַ�����Ϊ0�򲻴�ӡ��
		  int Solid_Hollow������/ʵ�ı߿���  0  ����
											 1  ʵ��
�� �� ֵ��void
******************************************************************************************/
void SingleBlock(int G, int K, int X, int Y, int bg, int fg, int NUM, char *CH, int Solid_Hollow) {
	int i;
	gotoxy(X, Y);
	setcolor(bg, fg);

	if (Solid_Hollow == 0) {//������
		cout << "�X";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "�T";
		cout << "�[";

		for (i = 0; i < G - 2; i++) {
			gotoxy(X, Y + 1 + i);
			cout << "�U";
		}

		gotoxy(X + 2 + (K - 6) / 2, Y + (G - 1) / 2);
		if (NUM)
			cout  <<setw(2)<< NUM;
		if (CH)
			cout << CH;
		for (i = 0; i < G - 2; i++) {
			gotoxy(X + K - 2, Y + 1 + i);
			cout << "�U";//�м�;
		}

		gotoxy(X, Y + G - 1);
		cout << "�^";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "�T";
		cout << "�a";
	}
	else {//ʵ����
		cout << "��";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "��";
		cout << "��";

		for (i = 0; i < G - 2; i++) {
			gotoxy(X, Y + 1 + i);
			cout << "��";
		}

		gotoxy(X + 2 + (K - 6) / 2, Y + (G - 1) / 2);
		if (NUM)
			cout << setw(2) << NUM;
		if (CH)
			cout << CH;
		for (i = 0; i < G - 2; i++) {
			gotoxy(X + K - 2, Y + 1 + i);
			cout << "��" ;//�м�
		}

		gotoxy(X, Y + G - 1);
		cout << "��";
		for (i = 0; i < (K - 4) / 2; i++)
			cout << "��";
		cout << "��";
	}

}

/******************************************************************************************
�������ƣ�ɫ���ƶ�����
��    �ܣ��ƶ�һ��ɫ��
���������int(*p)[10]���ڲ�����һ
          int hang��������
		  int lie��������
          int move_mode���ƶ���ʽѡ��  1  �������µ��������(�зָ������б��б�)
                                       2  �������ϵ��������
                                       3  �������ҵ��������
                                       4  �������󵥷������
                                       5  �������������ƶ���ȫ����
									   6  �������µ��������(�޷ָ������б��б�)
          int G��ɫ��߶�
	      int K��ɫ����
		  int NUM��ɫ�������ӡ�����ݣ����֣���Ϊ0�򲻴�ӡ��
		  char CH��ɫ�������ӡ�����ݣ��ַ�����Ϊ0�򲻴�ӡ��
		  int Solid_Hollow������/ʵ�ı߿���  0  ����
		                                     1  ʵ��
�� �� ֵ��void
******************************************************************************************/
void SingleBlock_MOVE(int(*p)[10], int hang, int lie, int move_mode, int G, int K,int NUM,char *CH,int Solid_Hollow,int H,int L) {
	int X=0,Y = 0,n=0,flag=0,MARK_L=0,i=0;
	if(move_mode==1)//���ϵ��µ���ɫ���ƶ�(�зָ������б��б�)
	for (Y = 4 * H + 2; Y < 4 * (H + 1) + 3; Y++) {
		Sleep(6);
		SingleBlock(G, K, 8 * L + 2, Y, p[H][L] + 2, 15, NUM, CH, Solid_Hollow);
		if (Y - 1 > 1) {
			gotoxy(8 * L + 2, Y - 1);
			setcolor(15, 2);
			cout << "      ";
		}
	}
	if (move_mode == 6)//���ϵ��µ���ɫ���ƶ�(�޷ָ������б��б�)
		for (Y = 3 * H + 3; Y <= 3 * H + 5; Y++) {
			Sleep(6);
			SingleBlock(G, K, 6 * L + 4, Y+1, p[H][L] + COLOR, COLOR_CH, NUM, CH, Solid_Hollow);
				gotoxy(6 * L + 4, Y);
				setcolor(15, 2);
				cout << "      ";
		}
	if (5 == move_mode) {//�������������ƶ���ȫ����
		for (n = 0; n<10; n++)
			for (L = 0; L < lie; L++) {
				for (H = 0, flag = 0; H < hang; H++)
					if (p[H][L] == 0)
						flag++;
				if (flag == hang)//��ĳ��ȫΪ��
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