//1751533 ��3 ���ӷ�
#include"90-b4.h"

void QRcode(char *content)
{
	/*��¼��ά��ͼ����ǳ����*/
	char pattern[177][177] = { 0 };

	/*��¼�ַ�������*/
	int len = 0;

	/*���ַ���ת��ΪUTF8��ʽ*/
	char *UTF8 = G2U(content, len);
	/*��ȥβ��*/
	len = len - 1;

	/*��¼�����ֽ�����*/
	int content_byte = 0;

	/*��ȡ�汾��*/
	int version_num = Version(UTF8, len, content_byte);

	/*�洢������*/
	char data[8192] = { 0 };

	/*������*/
	Coded_data(data, UTF8, content, content_byte, version_num);	

	/*��������*/
	Error_correction_code_data(data, version_num);

	/*���ݰ汾�Ŵ�ӡ��׼���ݣ������߿�/λ��̽��ͼ��/У��ͼ��*/
	Basic_Print(pattern,version_num);

    /*���汾��Ϣ�����汾�Ŵ��ڵ���7ʱ��*/
	if (version_num > 6)
		Version_information(pattern, version_num);

	/*����ʽ��Ϣ*/
	Format_information(pattern, version_num);

	/*���ݻ�õı���;�����Ϳ��ǳɫ��*/
	Code_Print(pattern, data, version_num);

	/*��������*/
	Mask_OFF_Code(pattern, version_num);
	
	/*��ӡȫͼ��*/
	Pattern_Print(pattern, version_num);

	setcolor();
	gotoxy(0, 30 + 4 * (version_num - 1));
}

//GB2312��UTF-8��ת��
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

/*ʮ����ת��Ϊ������*/
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

/*ȷ���汾��*/
int Version(char *UTF8, int len, int &content_byte)
{
	for (int i = 0; i < len; i++)
	{
		/*��¼�����ַ���bit��*/
		int single_bit = 0;
		/*��¼�����ַ�תΪUTF8��ʽ���byte��*/
		int utf8_byte = 0;
		/*��¼����������*/
		int binary_data[1000] = { 0 };

		/*���㵥���ַ���bit��*/
		Dec_to_Binary(int(*(UTF8 + i)), single_bit, binary_data);

		/*����bit���ó�UTF8��ʽ�µ�byte��*/
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

	/*��¼�汾��*/
	int version_num = 1;
	for (; version_num <= 40; version_num++)
		/*һ���汾�����������ݴ�С����ѡ��ð汾*/
		if (EClevel_M_Version_Capacity[version_num] > unsigned(content_byte))
			break;

	return version_num;
}

/*�����������*/
void Coded_data(char *data, char *UTF8, char *content, int content_byte, int version_num)
{
	/*���ģʽָʾ��*/
	strcat(data, "0100");

	/*����ַ�������*/
	/*�����ַ�����*/
	int content_length = content_byte;

	if (version_num < 10)
		binary_print(content_length, data, 8);
	else
		binary_print(content_length, data, 16);


	/*�������ԭ��*/
	for (int i = 0; i < content_byte; i++)
		binary_print(*(UTF8 + i), data, 8);

	/*��䲹λ*/
	/*�Ȳ��������ܱ�8����*/
	strcat(data, "0000");
	/*���油"11101100"��"00010001"ֱ����������*/
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

/*�������������*/
void Error_correction_code_data(char *data, int version_num)
{
	/*ԭ���ʮ���Ʒ�ʽ*/
	unsigned char data_dec[8192 / 8] = { 0 };
	unsigned char cor_data[1024] = { 0 };

	/*��ԭ��8bitһ��תΪʮ�������뵽������*/
	data_to_dec(data, data_dec);

	/*���汾�Ŵ���5����ֿ鴦��*/
	if (version_num > 5)
	{
		/*����ֿ�ԭ�����ݣ�ʮ���ƣ�*/
		unsigned char data_1[8192 / 8] = { 0 };
		unsigned char data_2[8192 / 8] = { 0 };

		/*����ֿ���������ݣ�ʮ���ƣ�*/
		unsigned char cor_1[8192 / 8] = { 0 };
		unsigned char cor_2[8192 / 8] = { 0 };

		/*��ʮ����ԭ��ֿ�*/
		int i = 0;
		for (; i < data_log[version_num]; i++)
			data_1[i] = data_dec[i];
		for (i = 0; i < data_log[version_num]; i++)
			data_2[i] = data_dec[i + data_log[version_num]];

		/*�ֿ����ɾ�����*/
		GenerErrorCorCode(data_1,data_log[version_num], cor_1,cor_log[version_num]);
		GenerErrorCorCode(data_2, data_log[version_num], cor_2, cor_log[version_num]);

		/*���data*/
		strcpy(data, "\0");
		
		/*��˳�򽫷ֿ��ԭ�����ţ����뵽data��*/
		for (int i = 0; i < data_log[version_num]; i++)
			binary_print(data_1[i], data, 8), binary_print(data_2[i], data, 8);

		/*��˳�򽫷ֿ�ľ����������ţ����뵽data��*/
		for (int i = 0; i < cor_log[version_num]; i++)
			binary_print(cor_1[i], data, 8), binary_print(cor_2[i], data, 8);
	}

	/*�汾��С��6ʱ���ֿ�*/
	else {
		/*����õ�������*/
		GenerErrorCorCode(data_dec, strlen(data) / 8, cor_data, Level_M_correction_code_num[version_num]);

		/*��������׷����ԭ��ĩβ*/
		for (unsigned int i = 0; i < Level_M_correction_code_num[version_num]; i++)
			binary_print(*(cor_data + i), data, 8);
	}
}

/*�������Ƶ�ԭ��ת��Ϊʮ����*/
void data_to_dec(char *data, unsigned char *data_dec)
{
	for (unsigned int i = 0; i < strlen(data) / 8; i++)
	{
		data_dec[i] = 128 * (data[i * 8] - '0') + 64 * (data[i * 8 + 1] - '0') + 32 * (data[i * 8 + 2] - '0') + 16 * (data[i * 8 + 3] - '0') + 8 * (data[i * 8 + 4] - '0') + 4 * (data[i * 8 + 5] - '0') + 2 * (data[i * 8 + 6] - '0') + (data[i * 8 + 7] - '0');
	}
}

/*ת��Ϊ�����Ʋ����뵽������*/
void binary_print(const unsigned int &v, char *data, int size)
{
	int i;
	int bit;

	for (i = 0; i < size; i++)
	{
		bit = v&(1 << (size - i - 1));//��ȡ����iλ��ֵ
		if (bit == 0)
			strcat(data, "0");
		else
			strcat(data, "1");
	}
}

/*	*********************************
�������ܣ�����һ�������
Mes���������׵�ַ
MesNum��һ���������ֽ���
result�����������׵�ַ
ErrCorCodeNum���������ֽ���
*/
void GenerErrorCorCode(unsigned char *Mes, int MesNum, unsigned  char *result, int ErrCorCodeNum)
{
	int pGenPoly;
	for (pGenPoly = 0; pGenPoly < GenerTableSize&&pGenerPolyTable[pGenPoly] != ErrCorCodeNum; pGenPoly++)
		;
	if (pGenPoly >= GenerTableSize)
	{
		cout << "��������������" << endl;
		return;
	}

	const int PolyNum = MesNum + ErrCorCodeNum;
	unsigned char *MesPoly = new unsigned char[PolyNum];//�����ڴ�
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

	delete MesPoly;//�ͷ��ڴ�
	delete GenPoly;
	delete tem;

	return;
}



