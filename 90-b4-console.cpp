//1751533 ��3 ���ӷ�
#include"90-b4.h"

/*��ӡ����λ��̽��ͼ��*/
void Single_Position_detection_pattern_Print(char(*pattern)[177], int hang, int lie)
{
	/*��һ��*/
	for (int i = 0; i < 7; i++)
		pattern[hang][lie + i] = DEEP_COLOR;

	/*�ڶ���*/
	pattern[hang + 1][lie] = DEEP_COLOR;
	for (int i = 0; i < 5; i++)
		pattern[hang + 1][lie + 1 + i] = LIGHT_COLOR;
	pattern[hang + 1][lie + 6] = DEEP_COLOR;

	/*����-����*/
	for (int i = 0; i < 3; i++)
	{
		pattern[hang + 2 + i][lie] = DEEP_COLOR;
		pattern[hang + 2 + i][lie + 1] = LIGHT_COLOR;
		pattern[hang + 2 + i][lie + 2] = DEEP_COLOR;
		pattern[hang + 2 + i][lie + 3] = DEEP_COLOR;
		pattern[hang + 2 + i][lie + 4] = DEEP_COLOR;
		pattern[hang + 2 + i][lie + 5] = LIGHT_COLOR;
		pattern[hang + 2 + i][lie + 6] = DEEP_COLOR;
	}

	/*������*/
	pattern[hang + 5][lie] = DEEP_COLOR;
	for (int i = 0; i < 5; i++)
		pattern[hang + 5][lie + 1 + i] = LIGHT_COLOR;
	pattern[hang + 5][lie + 6] = DEEP_COLOR;

	/*���һ��*/
	for (int i = 0; i < 7; i++)
		pattern[hang + 6][lie + i] = DEEP_COLOR;
}

/*��ӡλ��̽��ͼ�ε�һ���ر߿�*/
void Position_detection_pattern_Border(char(*pattern)[177], int version_num)
{
	/*���Ͻ�*/
	for (int i = 0; i < 8; i++)
		pattern[0 + i][7] = LIGHT_COLOR;
	for (int i = 0; i < 7; i++)
		pattern[7][0 + i] = LIGHT_COLOR;

	/*���½�*/
	for (int i = 0; i < 8; i++)
		pattern[21 + 4 * (version_num - 1) - 8 + i][7] = LIGHT_COLOR;
	for (int i = 0; i < 7; i++)
		pattern[21 + 4 * (version_num - 1) - 8][0 + i] = LIGHT_COLOR;

	/*���Ͻ�*/
	for (int i = 0; i < 8; i++)
		pattern[0 + i][21 + 4 * (version_num - 1) - 8] = LIGHT_COLOR;
	for (int i = 0; i < 8; i++)
		pattern[7][21 + 4 * (version_num - 1) - 8 + i] = LIGHT_COLOR;
}

/*��ӡ��λͼ��*/
void Timming_Pattern_Print(char(*pattern)[177], int version_num)
{
	/*��ӡ����*/
	for (int i = 1, j = 0; j < 21 + 4 * (version_num - 1) - 16; j++)
	{
		if (i)
		{
			pattern[6][8 + j] = DEEP_COLOR;
			i--;
		}
		else
		{

			pattern[6][8 + j] = LIGHT_COLOR;
			i++;
		}
	}

	/*��ӡ����*/
	for (int i = 1, j = 0; j < 21 + 4 * (version_num - 1) - 16; j++)
	{
		if (i)
		{
			pattern[8 + j][6] = DEEP_COLOR;
			i--;
		}
		else
		{
			pattern[8 + j][6] = LIGHT_COLOR;
			i++;
		}
	}

}

/*��ӡ���½ǵ�����ɫ���ؿ�*/
void Single_DEEP_COLOR_Block(char(*pattern)[177], int version_num)
{
	pattern[13 + 4 * (version_num - 1)][8] = DEEP_COLOR;
}

/*��ӡ��ά��߿�*/
void Border_Print(int x, int y, int length, int color)
{
	/*��ӡ�ϱ߿�*/
	setcolor(color, color);
	gotoxy(x, y);
	for (int i = 0; i < 2 * length; i++)
		cout << " ";

	/*��ӡ�±߿�*/
	gotoxy(x, y + length - 1);
	for (int i = 0; i < 2 * length; i++)
		cout << " ";

	/*��ӡ��߿�*/
	for (int i = 0; i < length - 2; i++)
	{
		gotoxy(x, y + 1 + i);
		cout << "  ";
	}

	/*��ӡ�ұ߿�*/
	for (int i = 0; i < length - 2; i++)
	{
		gotoxy(x + 2 * (length - 1), y + 1 + i);
		cout << "  ";
	}

}

/*��ӡ����У��ͼ��*/
void Single_Alignment_Pattern_Print(char(*pattern)[177], int hang, int lie)
{
	/*��һ��*/
	for (int i = 0; i < 5; i++)
		pattern[hang][lie + i] = DEEP_COLOR;

	/*�ڶ�-����*/
	for (int i = 0; i < 3; i++)
	{
		pattern[hang + 1 + i][lie] = DEEP_COLOR;
		pattern[hang + 1 + i][lie + 1] = LIGHT_COLOR;
		if (i == 1)
			pattern[hang + 1 + i][lie + 2] = DEEP_COLOR;
		else
			pattern[hang + 1 + i][lie + 2] = LIGHT_COLOR;
		pattern[hang + 1 + i][lie + 3] = LIGHT_COLOR;
		pattern[hang + 1 + i][lie + 4] = DEEP_COLOR;
	}

	/*���һ��*/
	for (int i = 0; i < 5; i++)
		pattern[hang + 4][lie + i] = DEEP_COLOR;
}

/*���ݼ�¼��ά��ͼ����ǳ���ص��ַ������ӡȫͼ��*/
void Pattern_Print(char(*pattern)[177], int version_num)
{
	gotoxy(2 + X, 1 + Y);

	/*����ǳɫ��ӡ��ά��*/
	for (int hang = 0; hang < 21 + 4 * (version_num - 1); hang++)
	{
		gotoxy(2 + X, 1 + hang + Y);
		for (int lie = 0; lie < 21 + 4 * (version_num - 1); lie++)
		{
			if (pattern[hang][lie] == DEEP_COLOR|| pattern[hang][lie] == DEEP_COLOR_DATA)
				setcolor(DEEP_COLOR_PRINT, DEEP_COLOR_PRINT);
			else if (pattern[hang][lie] == LIGHT_COLOR|| pattern[hang][lie] == LIGHT_COLOR_DATA)
				setcolor(LIGHT_COLOR_PRINT, LIGHT_COLOR_PRINT);
			else
				setcolor(3, 3);

			cout << "  ";
		}
	}

}

/*���ݰ汾�Ŵ�ӡ��׼���ݣ������߿�/λ��̽��ͼ��/У��ͼ��*/
void Basic_Print(char(*pattern)[177], int version_num)
{
	/*��ӡ�߿�*/
	Border_Print(0 + X, 0 + Y, 23 + 4 * (version_num - 1), 15);

	/*��ӡλ��̽��ͼ��*/
	Single_Position_detection_pattern_Print(pattern, 0, 0);
	Single_Position_detection_pattern_Print(pattern, 14 + 4 * (version_num - 1), 0);
	Single_Position_detection_pattern_Print(pattern, 0, 14 + 4 * (version_num - 1));
	Position_detection_pattern_Border(pattern, version_num);

	/*��ӡ��λͼ��*/
	Timming_Pattern_Print(pattern, version_num);

	/*��ӡ���½ǵ�����ɫ���ؿ�*/
	Single_DEEP_COLOR_Block(pattern, version_num);

	/*��ӡУ��ͼ��*/
	switch (version_num)
	{
	case 1:
		break;
	case 2:
		Single_Alignment_Pattern_Print(pattern, 16, 16);
		break;
	case 3:
		Single_Alignment_Pattern_Print(pattern, 20, 20);
		break;
	case 4:
		Single_Alignment_Pattern_Print(pattern, 24, 24);
		break;
	case 5:
		Single_Alignment_Pattern_Print(pattern, 28, 28);
		break;
	case 6:
		Single_Alignment_Pattern_Print(pattern, 32, 32);
		break;
	case 7:
		Single_Alignment_Pattern_Print(pattern, 4, 20);
		Single_Alignment_Pattern_Print(pattern, 20, 4);
		Single_Alignment_Pattern_Print(pattern, 20, 36);
		Single_Alignment_Pattern_Print(pattern, 36, 20);
		Single_Alignment_Pattern_Print(pattern, 20, 20);
		Single_Alignment_Pattern_Print(pattern, 36, 36);
		break;
	case 8:
		Single_Alignment_Pattern_Print(pattern, 4, 22);
		Single_Alignment_Pattern_Print(pattern, 22, 4);
		Single_Alignment_Pattern_Print(pattern, 22, 40);
		Single_Alignment_Pattern_Print(pattern, 40, 22);
		Single_Alignment_Pattern_Print(pattern, 22, 22);
		Single_Alignment_Pattern_Print(pattern, 40, 40);
		break;
	case 9:
		Single_Alignment_Pattern_Print(pattern, 4, 24);
		Single_Alignment_Pattern_Print(pattern, 24, 4);
		Single_Alignment_Pattern_Print(pattern, 24, 44);
		Single_Alignment_Pattern_Print(pattern, 44, 24);
		Single_Alignment_Pattern_Print(pattern, 24, 24);
		Single_Alignment_Pattern_Print(pattern, 44, 44);
		break;
	}
}

/*���ݻ�õı���;�����Ϳ��ǳɫ��*/
void Code_Print(char(*pattern)[177], char *data, int version_num)
{
	/*�����½ǿ�ʼ���*/
	int hang = 20 + 4 * (version_num - 1), lie = 20 + 4 * (version_num - 1);
	int data_counter = 0;

	int right_left = 1,up_down = 1;
	for (;;)
	{
		/*��right_leftΪ�棬������ұ��Ǹ�����������Ǹ�*/
		if (right_left)
		{
			/*�жϸô��Ƿ��й���ͼ��*/
			if (!pattern[hang][lie])
			{
				if (data[data_counter] == '1')
					pattern[hang][lie] = DEEP_COLOR_DATA;
				if(data[data_counter] == '0')
					pattern[hang][lie] = LIGHT_COLOR_DATA;
				data_counter++;
			}
			right_left--;
		}
		else
		{
			/*�жϸô��Ƿ��й���ͼ��*/
			if (!pattern[hang][lie-1])
			{
				if (data[data_counter] == '1')
					pattern[hang][lie-1] = DEEP_COLOR_DATA;
				if (data[data_counter] == '0')
					pattern[hang][lie-1] = LIGHT_COLOR_DATA;
				data_counter++;
			}
			right_left++;
			/*��up_downΪ�棬��������䣬��������*/
			if (up_down)
				hang--;
			else
				hang++;
		}

		/*���������ʱ*/
		if (hang < 0)
		{
			hang = 0, lie -= 2, up_down--;
			if (lie == 6)
				lie -= 1;
		}

		/*������׸�ʱ*/
		if (hang > 20 + 4 * (version_num - 1))
		{
			hang = 20 + 4 * (version_num - 1), lie -= 2, up_down++;
			if (lie == 6)
				lie -= 1;
		}

		if (!data[data_counter])
			break;

	}//end of for

	/*��δ������������ǳɫ���*/
	/*for (hang = 0; hang<21 + 4 * (version_num - 1); hang++)
		for (lie = 0; lie < 21 + 4 * (version_num - 1); lie++)
		{
			if (!pattern[hang][lie])
				pattern[hang][lie] = LIGHT_COLOR_DATA;
		}*/
}

/*����ʽ��Ϣ*/
void Format_information(char(*pattern)[177],int version_num)
{
	/*�洢BCHλ*/
	char BCH[16] = { "000110001100011" };

	/*������Ͻ�*/
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		if (BCH[i] == '1')
			pattern[i][8] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[i][8] = LIGHT_COLOR;
	}
	/*���սǴ�*/
	if (BCH[6] == '1')
		pattern[7][8] = DEEP_COLOR;
	if (BCH[6] == '0')
		pattern[7][8] = LIGHT_COLOR;
	if (BCH[7] == '1')
		pattern[8][8] = DEEP_COLOR;
	if (BCH[7] == '0')
		pattern[8][8] = LIGHT_COLOR;
	if (BCH[8] == '1')
		pattern[8][7] = DEEP_COLOR;
	if (BCH[8] == '0')
		pattern[8][7] = LIGHT_COLOR;
	for (i = 9;i <15 ; i++)
	{
		if (BCH[i] == '1')
			pattern[8][14-i] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[8][14-i] = LIGHT_COLOR;
	}

	/*������½�*/
	for (i = 8; i < 15; i++)
	{
		if (BCH[i] == '1')
			pattern[6+4*(version_num-1)+i][8] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[6 + 4 * (version_num - 1) + i][8] = LIGHT_COLOR;
	}

	/*������Ͻ�*/
	for (i = 0; i < 8; i++)
	{
		if (BCH[i] == '1')
			pattern[8][20 + 4 * (version_num - 1) - i] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[8][20 + 4 * (version_num - 1) - i] = LIGHT_COLOR;
	}

}

/*���汾��Ϣ�����汾�Ŵ��ڵ���7ʱ��*/
void Version_information(char(*pattern)[177], int version_num)
{
	cout << version_information[version_num][0] << endl;

	/*������½ǲ���*/
	pattern[10 + 4 * (version_num-1)][0] = version_information[version_num][0], pattern[10 + 4 * (version_num - 1)][1] = version_information[version_num][3], pattern[10 + 4 * (version_num - 1)][2] = version_information[version_num][6], pattern[10 + 4 * (version_num - 1)][3] = version_information[version_num][9], pattern[10 + 4 * (version_num - 1)][4] = version_information[version_num][12], pattern[10 + 4 * (version_num - 1)][5] = version_information[version_num][15];
	pattern[11 + 4 * (version_num-1)][0] = version_information[version_num][1], pattern[11 + 4 * (version_num - 1)][1] = version_information[version_num][4], pattern[11 + 4 * (version_num - 1)][2] = version_information[version_num][7], pattern[11 + 4 * (version_num - 1)][3] = version_information[version_num][10], pattern[11 + 4 * (version_num - 1)][4] = version_information[version_num][13], pattern[11 + 4 * (version_num - 1)][5] = version_information[version_num][16];
	pattern[12 + 4 * (version_num - 1)][0] = version_information[version_num][2], pattern[12 + 4 * (version_num - 1)][1] = version_information[version_num][5], pattern[12 + 4 * (version_num - 1)][2] = version_information[version_num][8], pattern[12 + 4 * (version_num - 1)][3] = version_information[version_num][11], pattern[12 + 4 * (version_num - 1)][4] = version_information[version_num][14], pattern[12 + 4 * (version_num - 1)][5] = version_information[version_num][17];

	/*������Ͻǲ���*/
	pattern[0][10 + 4 * (version_num - 1)] = version_information[version_num][0], pattern[0][11 + 4 * (version_num - 1)] = version_information[version_num][1], pattern[0][12 + 4 * (version_num - 1)] = version_information[version_num][2];
	pattern[1][10 + 4 * (version_num - 1)] = version_information[version_num][3], pattern[1][11 + 4 * (version_num - 1)] = version_information[version_num][4], pattern[1][12 + 4 * (version_num - 1)] = version_information[version_num][5];
	pattern[2][10 + 4 * (version_num - 1)] = version_information[version_num][6], pattern[2][11 + 4 * (version_num - 1)] = version_information[version_num][7], pattern[2][12 + 4 * (version_num - 1)] = version_information[version_num][8];
	pattern[3][10 + 4 * (version_num - 1)] = version_information[version_num][9], pattern[3][11 + 4 * (version_num - 1)] = version_information[version_num][10], pattern[3][12 + 4 * (version_num - 1)] = version_information[version_num][11];
	pattern[4][10 + 4 * (version_num - 1)] = version_information[version_num][12], pattern[4][11 + 4 * (version_num - 1)] = version_information[version_num][13], pattern[4][12 + 4 * (version_num - 1)] = version_information[version_num][14];
	pattern[5][10 + 4 * (version_num - 1)] = version_information[version_num][15], pattern[5][11 + 4 * (version_num - 1)] = version_information[version_num][16], pattern[5][12 + 4 * (version_num - 1)] = version_information[version_num][17];
}

/*��������������ģ*/
void Mask_OFF_Code(char(*pattern)[177], int version_num)
{
	int hang = 0, lie = 0;
	/*�洢��ģͼ��*/
	char Mask_Pattern[177][177] = { 0 };

	/*������ģͼ��(101)*/
	for(hang=0;hang<21+4*(version_num-1);hang++)
		for (lie = 0; lie < 21 + 4 * (version_num - 1); lie++)
		{
			if ((hang*lie) % 2 + (hang*lie) % 3 == 0)
				Mask_Pattern[hang][lie] = DEEP_COLOR_DATA;
			else
				Mask_Pattern[hang][lie] = LIGHT_COLOR_DATA;
		}


	for(hang=0;hang<21+4*(version_num-1);hang++)
		for (lie = 0; lie < 21 + 4 * (version_num - 1); lie++)
			/*ֻ��������������ģ*/
			if (pattern[hang][lie] == DEEP_COLOR_DATA || pattern[hang][lie] == LIGHT_COLOR_DATA)
			{
				/*���������*/
				if (pattern[hang][lie] == Mask_Pattern[hang][lie])
					pattern[hang][lie] = LIGHT_COLOR_DATA;
				else
					pattern[hang][lie] = DEEP_COLOR_DATA;
			}
		
}