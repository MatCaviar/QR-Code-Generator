//1751533 计3 宋子帆
#include"90-b4.h"

/*打印单个位置探测图形*/
void Single_Position_detection_pattern_Print(char(*pattern)[177], int hang, int lie)
{
	/*第一行*/
	for (int i = 0; i < 7; i++)
		pattern[hang][lie + i] = DEEP_COLOR;

	/*第二行*/
	pattern[hang + 1][lie] = DEEP_COLOR;
	for (int i = 0; i < 5; i++)
		pattern[hang + 1][lie + 1 + i] = LIGHT_COLOR;
	pattern[hang + 1][lie + 6] = DEEP_COLOR;

	/*第三-五行*/
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

	/*第六行*/
	pattern[hang + 5][lie] = DEEP_COLOR;
	for (int i = 0; i < 5; i++)
		pattern[hang + 5][lie + 1 + i] = LIGHT_COLOR;
	pattern[hang + 5][lie + 6] = DEEP_COLOR;

	/*最后一行*/
	for (int i = 0; i < 7; i++)
		pattern[hang + 6][lie + i] = DEEP_COLOR;
}

/*打印位置探测图形的一像素边框*/
void Position_detection_pattern_Border(char(*pattern)[177], int version_num)
{
	/*左上角*/
	for (int i = 0; i < 8; i++)
		pattern[0 + i][7] = LIGHT_COLOR;
	for (int i = 0; i < 7; i++)
		pattern[7][0 + i] = LIGHT_COLOR;

	/*左下角*/
	for (int i = 0; i < 8; i++)
		pattern[21 + 4 * (version_num - 1) - 8 + i][7] = LIGHT_COLOR;
	for (int i = 0; i < 7; i++)
		pattern[21 + 4 * (version_num - 1) - 8][0 + i] = LIGHT_COLOR;

	/*右上角*/
	for (int i = 0; i < 8; i++)
		pattern[0 + i][21 + 4 * (version_num - 1) - 8] = LIGHT_COLOR;
	for (int i = 0; i < 8; i++)
		pattern[7][21 + 4 * (version_num - 1) - 8 + i] = LIGHT_COLOR;
}

/*打印定位图形*/
void Timming_Pattern_Print(char(*pattern)[177], int version_num)
{
	/*打印横条*/
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

	/*打印竖条*/
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

/*打印左下角单个深色像素块*/
void Single_DEEP_COLOR_Block(char(*pattern)[177], int version_num)
{
	pattern[13 + 4 * (version_num - 1)][8] = DEEP_COLOR;
}

/*打印二维码边框*/
void Border_Print(int x, int y, int length, int color)
{
	/*打印上边框*/
	setcolor(color, color);
	gotoxy(x, y);
	for (int i = 0; i < 2 * length; i++)
		cout << " ";

	/*打印下边框*/
	gotoxy(x, y + length - 1);
	for (int i = 0; i < 2 * length; i++)
		cout << " ";

	/*打印左边框*/
	for (int i = 0; i < length - 2; i++)
	{
		gotoxy(x, y + 1 + i);
		cout << "  ";
	}

	/*打印右边框*/
	for (int i = 0; i < length - 2; i++)
	{
		gotoxy(x + 2 * (length - 1), y + 1 + i);
		cout << "  ";
	}

}

/*打印单个校正图形*/
void Single_Alignment_Pattern_Print(char(*pattern)[177], int hang, int lie)
{
	/*第一行*/
	for (int i = 0; i < 5; i++)
		pattern[hang][lie + i] = DEEP_COLOR;

	/*第二-四行*/
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

	/*最后一行*/
	for (int i = 0; i < 5; i++)
		pattern[hang + 4][lie + i] = DEEP_COLOR;
}

/*根据记录二维码图形深浅像素的字符数组打印全图形*/
void Pattern_Print(char(*pattern)[177], int version_num)
{
	gotoxy(2 + X, 1 + Y);

	/*按深浅色打印二维码*/
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

/*根据版本号打印基准内容，包括边框/位置探测图形/校正图形*/
void Basic_Print(char(*pattern)[177], int version_num)
{
	/*打印边框*/
	Border_Print(0 + X, 0 + Y, 23 + 4 * (version_num - 1), 15);

	/*打印位置探测图形*/
	Single_Position_detection_pattern_Print(pattern, 0, 0);
	Single_Position_detection_pattern_Print(pattern, 14 + 4 * (version_num - 1), 0);
	Single_Position_detection_pattern_Print(pattern, 0, 14 + 4 * (version_num - 1));
	Position_detection_pattern_Border(pattern, version_num);

	/*打印定位图形*/
	Timming_Pattern_Print(pattern, version_num);

	/*打印左下角单个深色像素块*/
	Single_DEEP_COLOR_Block(pattern, version_num);

	/*打印校正图形*/
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

/*根据获得的编码和纠错码涂深浅色块*/
void Code_Print(char(*pattern)[177], char *data, int version_num)
{
	/*从右下角开始填充*/
	int hang = 20 + 4 * (version_num - 1), lie = 20 + 4 * (version_num - 1);
	int data_counter = 0;

	int right_left = 1,up_down = 1;
	for (;;)
	{
		/*若right_left为真，则填充右边那个，否则左边那个*/
		if (right_left)
		{
			/*判断该处是否有功能图形*/
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
			/*判断该处是否有功能图形*/
			if (!pattern[hang][lie-1])
			{
				if (data[data_counter] == '1')
					pattern[hang][lie-1] = DEEP_COLOR_DATA;
				if (data[data_counter] == '0')
					pattern[hang][lie-1] = LIGHT_COLOR_DATA;
				data_counter++;
			}
			right_left++;
			/*若up_down为真，则向上填充，否则向下*/
			if (up_down)
				hang--;
			else
				hang++;
		}

		/*填充至顶格时*/
		if (hang < 0)
		{
			hang = 0, lie -= 2, up_down--;
			if (lie == 6)
				lie -= 1;
		}

		/*填充至底格时*/
		if (hang > 20 + 4 * (version_num - 1))
		{
			hang = 20 + 4 * (version_num - 1), lie -= 2, up_down++;
			if (lie == 6)
				lie -= 1;
		}

		if (!data[data_counter])
			break;

	}//end of for

	/*将未填满的区域用浅色填充*/
	/*for (hang = 0; hang<21 + 4 * (version_num - 1); hang++)
		for (lie = 0; lie < 21 + 4 * (version_num - 1); lie++)
		{
			if (!pattern[hang][lie])
				pattern[hang][lie] = LIGHT_COLOR_DATA;
		}*/
}

/*填充格式信息*/
void Format_information(char(*pattern)[177],int version_num)
{
	/*存储BCH位*/
	char BCH[16] = { "000110001100011" };

	/*填充左上角*/
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		if (BCH[i] == '1')
			pattern[i][8] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[i][8] = LIGHT_COLOR;
	}
	/*填充拐角处*/
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

	/*填充左下角*/
	for (i = 8; i < 15; i++)
	{
		if (BCH[i] == '1')
			pattern[6+4*(version_num-1)+i][8] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[6 + 4 * (version_num - 1) + i][8] = LIGHT_COLOR;
	}

	/*填充右上角*/
	for (i = 0; i < 8; i++)
	{
		if (BCH[i] == '1')
			pattern[8][20 + 4 * (version_num - 1) - i] = DEEP_COLOR;
		if (BCH[i] == '0')
			pattern[8][20 + 4 * (version_num - 1) - i] = LIGHT_COLOR;
	}

}

/*填充版本信息（当版本号大于等于7时）*/
void Version_information(char(*pattern)[177], int version_num)
{
	cout << version_information[version_num][0] << endl;

	/*填充左下角部分*/
	pattern[10 + 4 * (version_num-1)][0] = version_information[version_num][0], pattern[10 + 4 * (version_num - 1)][1] = version_information[version_num][3], pattern[10 + 4 * (version_num - 1)][2] = version_information[version_num][6], pattern[10 + 4 * (version_num - 1)][3] = version_information[version_num][9], pattern[10 + 4 * (version_num - 1)][4] = version_information[version_num][12], pattern[10 + 4 * (version_num - 1)][5] = version_information[version_num][15];
	pattern[11 + 4 * (version_num-1)][0] = version_information[version_num][1], pattern[11 + 4 * (version_num - 1)][1] = version_information[version_num][4], pattern[11 + 4 * (version_num - 1)][2] = version_information[version_num][7], pattern[11 + 4 * (version_num - 1)][3] = version_information[version_num][10], pattern[11 + 4 * (version_num - 1)][4] = version_information[version_num][13], pattern[11 + 4 * (version_num - 1)][5] = version_information[version_num][16];
	pattern[12 + 4 * (version_num - 1)][0] = version_information[version_num][2], pattern[12 + 4 * (version_num - 1)][1] = version_information[version_num][5], pattern[12 + 4 * (version_num - 1)][2] = version_information[version_num][8], pattern[12 + 4 * (version_num - 1)][3] = version_information[version_num][11], pattern[12 + 4 * (version_num - 1)][4] = version_information[version_num][14], pattern[12 + 4 * (version_num - 1)][5] = version_information[version_num][17];

	/*填充右上角部分*/
	pattern[0][10 + 4 * (version_num - 1)] = version_information[version_num][0], pattern[0][11 + 4 * (version_num - 1)] = version_information[version_num][1], pattern[0][12 + 4 * (version_num - 1)] = version_information[version_num][2];
	pattern[1][10 + 4 * (version_num - 1)] = version_information[version_num][3], pattern[1][11 + 4 * (version_num - 1)] = version_information[version_num][4], pattern[1][12 + 4 * (version_num - 1)] = version_information[version_num][5];
	pattern[2][10 + 4 * (version_num - 1)] = version_information[version_num][6], pattern[2][11 + 4 * (version_num - 1)] = version_information[version_num][7], pattern[2][12 + 4 * (version_num - 1)] = version_information[version_num][8];
	pattern[3][10 + 4 * (version_num - 1)] = version_information[version_num][9], pattern[3][11 + 4 * (version_num - 1)] = version_information[version_num][10], pattern[3][12 + 4 * (version_num - 1)] = version_information[version_num][11];
	pattern[4][10 + 4 * (version_num - 1)] = version_information[version_num][12], pattern[4][11 + 4 * (version_num - 1)] = version_information[version_num][13], pattern[4][12 + 4 * (version_num - 1)] = version_information[version_num][14];
	pattern[5][10 + 4 * (version_num - 1)] = version_information[version_num][15], pattern[5][11 + 4 * (version_num - 1)] = version_information[version_num][16], pattern[5][12 + 4 * (version_num - 1)] = version_information[version_num][17];
}

/*对数据区进行掩模*/
void Mask_OFF_Code(char(*pattern)[177], int version_num)
{
	int hang = 0, lie = 0;
	/*存储掩模图型*/
	char Mask_Pattern[177][177] = { 0 };

	/*输入掩模图型(101)*/
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
			/*只对数据区进行掩模*/
			if (pattern[hang][lie] == DEEP_COLOR_DATA || pattern[hang][lie] == LIGHT_COLOR_DATA)
			{
				/*作异或运算*/
				if (pattern[hang][lie] == Mask_Pattern[hang][lie])
					pattern[hang][lie] = LIGHT_COLOR_DATA;
				else
					pattern[hang][lie] = DEEP_COLOR_DATA;
			}
		
}