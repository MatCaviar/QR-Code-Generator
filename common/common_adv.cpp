/*1751533 ��3 ���ӷ�*/
#include"../90-b0/90-b0.h"
#include"../90-b1/90-b1.h"

/******************************************************************************************
�������ƣ����������뺯��
��    �ܣ�����������
���������int *phang��ָ����������ָ��
		  int *plie��ָ����������ָ��
		  int hang_MIN������Ϸ��С����
		  int hang_MAX������Ϸ�������
		  int lie_MIN������Ϸ��С����
		  int lie_MAX������Ϸ����� ��
�� �� ֵ��void
******************************************************************************************/
void input(int *phang, int *plie, int hang_MIN, int hang_MAX, int lie_MIN, int lie_MAX) {
	cls();
	for (;;) {//��ȡ�����������������
		gotoxy(0, 0);
		cout << "����������(" << hang_MIN << "-" << hang_MAX << ")��";
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
	for (;;) {//��ȡ�����������������
		gotoxy(0, 1);
		cout << "����������(" << lie_MIN << "-" << lie_MAX << ")��";
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
�������ƣ�END����
��    �ܣ�����end�Լ���
���������void
�� �� ֵ��void
******************************************************************************************/
void getEND(void) {
	int X = 0, Y = 0;
	char e, n, d;
	cout << endl << endl << "��С�������������End����...";
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
		cout << endl << "�����������������";
		Sleep(800);
		gotoxy(0, Y + 2);
		cout << "                                                    ";
	}
}

/******************************************************************************************
�������ƣ���꼰����ѡ����
��    �ܣ�ʵ����꼰����ѡ��
���������
�� �� ֵ��
******************************************************************************************/
void keyboard_mouse_MOVE(int(*p)[10], char(*mark)[10], int hang, int lie, char select, int &H, int &L, int &allscore, int &target, int &flag_continue, int extra) {
	int X, Y;
	int ret, maction;
	int keycode1, keycode2;
	int temp_H = 0, temp_L = 0, flag_mouse = 0;

	ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);//��ȡ

	if (ret == CCT_MOUSE_EVENT) {//����¼�
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
			cout << "[��ǰλ��] " << setw(2) << char('A' + H) << " ��" << setw(2) << L << "��";//��ӡ��ǰλ��
		else
			cout << "[��ǰλ��] ���λ�÷Ƿ�";
		switch (maction) {
		case MOUSE_LEFT_BUTTON_CLICK:			//�������ѡ��
			Block_Select(p, mark, hang, lie, H, L, &allscore, target, &flag_continue, select);
			break;
		} //end of switch(maction)
	} //end of if (CCT_MOUSE_EVENT)
	else if (ret == CCT_KEYBOARD_EVENT) {//�����¼�
		switch (keycode1) {
		case '\r':	//�س�
			Block_Select(p, mark, hang, lie, H, L, &allscore, target, &flag_continue, select);
			break;
		case 224:
			if (extra&&p[H][L])
				SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_CH, 0, "��", 0);
			else if(extra==0)
				SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, p[H][L] + 2, 15, p[H][L], 0, 1);
			switch (keycode2) {
			case KB_ARROW_UP://��
				H = H - 1;
				if (H < 0 || p[H][L] == 0)
					H = hang - 1;
				break;
			case KB_ARROW_DOWN://��
				H = H + 1;
				if (H > hang - 1) {
					for (H = 0;; H++)
						if (p[H][L])
							break;
				}
				break;
			case KB_ARROW_LEFT://��
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
			case KB_ARROW_RIGHT://��
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
�������ƣ���꼰����ȡ��ѡ����
��    �ܣ�ʵ��ѡ�������꼰����ȡ��ѡ��
���������
�� �� ֵ��
******************************************************************************************/
void keyboard_mouse_SELECT(int(*p)[10], char(*mark)[10], int hang, int lie, char select, int &H, int &L, int &allscore, int &target, int &flag_continue, int extra) {
	int X, Y;
	int ret, maction;
	int keycode1, keycode2;
	int temp_H = 0, temp_L = 0, flag_mouse = 0;

	for (;;) {
		ret = read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);//��ȡ

		if (ret == CCT_MOUSE_EVENT) {//����¼�
			for (temp_H = 0, flag_mouse = 0; temp_H < hang; temp_H++)
				for (temp_L = 0; temp_L < lie; temp_L++)
					if (X >= (8 - extra * 2) * temp_L + 2 + extra * 2 && X <= (8 - extra * 2) * temp_L + 7 + extra * 2 && Y >= (4 - extra) * temp_H + 2 + extra && Y <= (4 - extra) * temp_H + 4 + extra)
						if (temp_H != H || temp_L != L) {
							redrew(p, mark, hang, lie, extra);//�����ϳ�
							return;
						}
			if (maction == MOUSE_LEFT_BUTTON_CLICK) { //��������ϳ�
				CLEAR_ASEMBLE_mark(p, mark, hang, lie, H, L, &allscore, target);
				return;
			}

		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {//�����¼�
			switch (keycode1) {
			case '\r':	//���»س��ϳ�
				CLEAR_ASEMBLE_mark(p, mark, hang, lie, H, L, &allscore, target);
				return;
			case 224:
				if (extra)
					SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_CH, 0, "��", 0);
				else
					SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, p[H][L] + 2, 15, p[H][L], 0, 1);				switch (keycode2) {
					case KB_ARROW_UP://��
						redrew(p, mark, hang, lie, extra);//�����ϳ�
						H = H - 1;
						if (H < 0)
							H = hang - 1;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "��", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);
						return;
					case KB_ARROW_DOWN://��
						redrew(p, mark, hang, lie, extra);//�����ϳ�
						H = H + 1;
						if (H > hang - 1)
							H = 0;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "��", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);					return;
					case KB_ARROW_LEFT://��
						redrew(p, mark, hang, lie, extra);//�����ϳ�
						L = L - 1;
						if (L < 0)
							L = lie - 1;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "��", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);					return;
					case KB_ARROW_RIGHT://��
						redrew(p, mark, hang, lie, extra);//�����ϳ�
						L = L + 1;
						if (L > lie - 1)
							L = 0;
						if (extra)
							SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_SELECT, 0, "��", 0);
						else
							SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, 14, 2, p[H][L], 0, 1);					return;
				}//end of swicth(keycode1)

			}
		}
	}
}

/******************************************************************************************
�������ƣ��ػ溯����������ȡ��ѡ��
��    �ܣ��ػ汻��ǵ�����
���������
�� �� ֵ��
******************************************************************************************/
void redrew(int(*p)[10], char(*mark)[10], int hang, int lie, int extra) {
	int H = 0, L = 0;
	for (H = 0; H < hang; H++)
		for (L = 0; L < lie; L++)
			if (mark[H][L] == '*') {
				if (extra)
					SingleBlock(3, 6, 6 * L + 4, 3 * H + 3, p[H][L] + COLOR, COLOR_CH, 0, "��", 0);
				else
					SingleBlock(3, 6, 8 * L + 2, 4 * H + 2, p[H][L] + 2, 15, p[H][L], 0, 1);
			}
}

/******************************************************************************************
�������ƣ�����������ú���
��    �ܣ����ñ������
���������
�� �� ֵ��
******************************************************************************************/
void MARK_CLEAR(char(*mark)[10], int hang, int lie) {
	int H = 0, L = 0;
	for (H = 0; H < hang; H++)
		for (L = 0; L < lie; L++)
			mark[H][L] = '0';
}









