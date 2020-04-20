//1751533 计3 宋子帆
#include"90-b4.h"

void QRcode(char *content)
{
	/*记录二维码图形深浅像素*/
	char pattern[177][177] = { 0 };

	/*记录字符串长度*/
	int len = 0;

	/*将字符串转换为UTF8格式*/
	char *UTF8 = G2U(content, len);
	/*除去尾零*/
	len = len - 1;

	/*记录数据字节总数*/
	int content_byte = 0;

	/*获取版本号*/
	int version_num = Version(UTF8, len, content_byte);

	/*存储数据区*/
	char data[8192] = { 0 };

	/*填充编码*/
	Coded_data(data, UTF8, content, content_byte, version_num);	

	/*填充纠错码*/
	Error_correction_code_data(data, version_num);

	/*根据版本号打印基准内容，包括边框/位置探测图形/校正图形*/
	Basic_Print(pattern,version_num);

    /*填充版本信息（当版本号大于等于7时）*/
	if (version_num > 6)
		Version_information(pattern, version_num);

	/*填充格式信息*/
	Format_information(pattern, version_num);

	/*根据获得的编码和纠错码涂深浅色块*/
	Code_Print(pattern, data, version_num);

	/*进行掩码*/
	Mask_OFF_Code(pattern, version_num);
	
	/*打印全图形*/
	Pattern_Print(pattern, version_num);

	setcolor();
	gotoxy(0, 30 + 4 * (version_num - 1));
}

//GB2312到UTF-8的转换
char* G2U(const char* gb2312, int &len)
{
	len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

/*十进制转换为二进制*/
void Dec_to_Binary(int dec, int &single_bit, int binary_data[])
{
	int i = 0;
	single_bit = 0;
	i = dec;
	while (i)
	{
		binary_data[single_bit] = i % 2;
		i /= 2;
		single_bit++;
	}
}

/*确定版本号*/
int Version(char *UTF8, int len, int &content_byte)
{
	for (int i = 0; i < len; i++)
	{
		/*记录单个字符的bit数*/
		int single_bit = 0;
		/*记录单个字符转为UTF8格式后的byte数*/
		int utf8_byte = 0;
		/*记录二进制数据*/
		int binary_data[1000] = { 0 };

		/*计算单个字符的bit数*/
		Dec_to_Binary(int(*(UTF8 + i)), single_bit, binary_data);

		/*根据bit数得出UTF8格式下的byte数*/
		if (single_bit >= 0 && single_bit <= 7)
			utf8_byte = 1;
		else if (single_bit >= 8 && single_bit <= 11)
			utf8_byte = 2;
		else if (single_bit >= 12 && single_bit <= 16)
			utf8_byte = 3;
		else if (single_bit >= 17 && single_bit <= 21)
			utf8_byte = 4;
		else if (single_bit >= 22 && single_bit <= 26)
			utf8_byte = 5;
		else if (single_bit >= 27 && single_bit <= 31)
			utf8_byte = 6;

		content_byte += utf8_byte;
	}

	/*记录版本号*/
	int version_num = 1;
	for (; version_num <= 40; version_num++)
		/*一旦版本容量超过数据大小，则选择该版本*/
		if (EClevel_M_Version_Capacity[version_num] > unsigned(content_byte))
			break;

	return version_num;
}

/*处理编码数据*/
void Coded_data(char *data, char *UTF8, char *content, int content_byte, int version_num)
{
	/*填充模式指示符*/
	strcat(data, "0100");

	/*填充字符计数符*/
	/*计算字符个数*/
	int content_length = content_byte;

	if (version_num < 10)
		binary_print(content_length, data, 8);
	else
		binary_print(content_length, data, 16);


	/*填充编码后原串*/
	for (int i = 0; i < content_byte; i++)
		binary_print(*(UTF8 + i), data, 8);

	/*填充补位*/
	/*先补至长度能被8整除*/
	strcat(data, "0000");
	/*交替补"11101100"与"00010001"直到填满容量*/
	for (int i = 1; strlen(data) < EClevel_M_Version_Capacity_total[version_num] * 8;)
	{
		if (i)
		{
			strcat(data, "11101100");
			i--;
		}
		else
		{
			strcat(data, "00010001");
			i++;
		}
	}
}

/*处理纠错码数据*/
void Error_correction_code_data(char *data, int version_num)
{
	/*原码的十进制方式*/
	unsigned char data_dec[8192 / 8] = { 0 };
	unsigned char cor_data[1024] = { 0 };

	/*将原码8bit一组转为十进制输入到数组中*/
	data_to_dec(data, data_dec);

	/*若版本号大于5，则分块处理*/
	if (version_num > 5)
	{
		/*储存分块原码数据（十进制）*/
		unsigned char data_1[8192 / 8] = { 0 };
		unsigned char data_2[8192 / 8] = { 0 };

		/*储存分块纠错码数据（十进制）*/
		unsigned char cor_1[8192 / 8] = { 0 };
		unsigned char cor_2[8192 / 8] = { 0 };

		/*将十进制原码分块*/
		int i = 0;
		for (; i < data_log[version_num]; i++)
			data_1[i] = data_dec[i];
		for (i = 0; i < data_log[version_num]; i++)
			data_2[i] = data_dec[i + data_log[version_num]];

		/*分块生成纠错码*/
		GenerErrorCorCode(data_1,data_log[version_num], cor_1,cor_log[version_num]);
		GenerErrorCorCode(data_2, data_log[version_num], cor_2, cor_log[version_num]);

		/*清空data*/
		strcpy(data, "\0");
		
		/*按顺序将分块的原码重排，输入到data中*/
		for (int i = 0; i < data_log[version_num]; i++)
			binary_print(data_1[i], data, 8), binary_print(data_2[i], data, 8);

		/*按顺序将分块的纠错码码重排，输入到data中*/
		for (int i = 0; i < cor_log[version_num]; i++)
			binary_print(cor_1[i], data, 8), binary_print(cor_2[i], data, 8);
	}

	/*版本号小于6时不分块*/
	else {
		/*计算得到纠错码*/
		GenerErrorCorCode(data_dec, strlen(data) / 8, cor_data, Level_M_correction_code_num[version_num]);

		/*将纠错码追加在原码末尾*/
		for (unsigned int i = 0; i < Level_M_correction_code_num[version_num]; i++)
			binary_print(*(cor_data + i), data, 8);
	}
}

/*将二进制的原码转换为十进制*/
void data_to_dec(char *data, unsigned char *data_dec)
{
	for (unsigned int i = 0; i < strlen(data) / 8; i++)
	{
		data_dec[i] = 128 * (data[i * 8] - '0') + 64 * (data[i * 8 + 1] - '0') + 32 * (data[i * 8 + 2] - '0') + 16 * (data[i * 8 + 3] - '0') + 8 * (data[i * 8 + 4] - '0') + 4 * (data[i * 8 + 5] - '0') + 2 * (data[i * 8 + 6] - '0') + (data[i * 8 + 7] - '0');
	}
}

/*转换为二进制并输入到数组中*/
void binary_print(const unsigned int &v, char *data, int size)
{
	int i;
	int bit;

	for (i = 0; i < size; i++)
	{
		bit = v&(1 << (size - i - 1));//提取出第i位的值
		if (bit == 0)
			strcat(data, "0");
		else
			strcat(data, "1");
	}
}

/*	*********************************
函数功能：生成一块纠错码
Mes：数据码首地址
MesNum：一块数据码字节数
result：纠错码结果首地址
ErrCorCodeNum：纠错码字节数
*/
void GenerErrorCorCode(unsigned char *Mes, int MesNum, unsigned  char *result, int ErrCorCodeNum)
{
	int pGenPoly;
	for (pGenPoly = 0; pGenPoly < GenerTableSize&&pGenerPolyTable[pGenPoly] != ErrCorCodeNum; pGenPoly++)
		;
	if (pGenPoly >= GenerTableSize)
	{
		cout << "纠错码字数有误" << endl;
		return;
	}

	const int PolyNum = MesNum + ErrCorCodeNum;
	unsigned char *MesPoly = new unsigned char[PolyNum];//开辟内存
	unsigned char *GenPoly = new unsigned char[PolyNum];
	unsigned char *tem = new unsigned char[PolyNum];

	int i, j;
	for (i = 0; i < PolyNum; i++)
	{
		MesPoly[i] = (i < MesNum ? Mes[i] : '\0');
		GenPoly[i] = (i <= ErrCorCodeNum ? GenerPolyTable[pGenPoly][ErrCorCodeNum - i] : '\0');
	}

	unsigned char CurCeof;
	for (i = 0; i < MesNum; i++)
	{
		CurCeof = AntiLog[MesPoly[i]];
		for (j = 0; j <= ErrCorCodeNum; j++)
		{
			tem[j] = (GenPoly[j] + CurCeof) % 255;
			tem[j] = Log[tem[j]];
			MesPoly[i + j] ^= tem[j];
		}
	}

	for (j = 0; i < PolyNum; i++)
		result[j++] = MesPoly[i];

	delete MesPoly;//释放内存
	delete GenPoly;
	delete tem;

	return;
}



