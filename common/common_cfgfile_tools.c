/*1751533 ��3 ���ӷ�*/
#include"../common/common_cfgfile_tools.h"

/***************************************************************************
�������ƣ�
��    �ܣ���ֻ�����д��ĳ�ļ�
���������
�� �� ֵ��
˵    ����
***************************************************************************/
FILE *open_cfgfile(const char *cfgname, int opt)
{
	FILE *fp = NULL;

	/*��ֻ����ʽ��*/
	if (opt == OPEN_OPT_RDONLY)
		fp = fopen(cfgname, "r");
	/*�Զ�д��ʽ��*/
	if (opt == OPEN_OPT_RDWR) {
		fp = fopen(cfgname, "r+");
		if (fp == NULL)
			fp = fopen(cfgname, "w+");
	}
	/*�����ļ�ָ��*/
	return fp;
}

/***************************************************************************
�������ƣ�
��    �ܣ��ر�һ���ļ�
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void close_cfgfile(FILE *fp)
{
	fclose(fp);
}

/***************************************************************************
�������ƣ�
��    �ܣ����һ����
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int group_add(FILE *fp, const char *group_name)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	rewind(fp);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256];

	/*���ж�ȡ������Ƿ��Ѵ���group_name*/
	for (; fgets(temp, 256, fp);)
		/*���Ѵ���group_name,�򷵻�0��FAIL*/
		if (strstr(temp, group_name))
			return FAIL;

	/*д��ɹ��򷵻�1��SUCCESS*/
	if (fprintf(fp, "[%s]\n\n", group_name))
		return SUCCESS;

	return FAIL;
}

/***************************************************************************
�������ƣ�
��    �ܣ�ɾ��һ���鼰������
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int group_del(FILE *fp, const char *group_name)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	rewind(fp);

	/*��¼ɾ������*/
	int group_del_counter = 0;

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*����һ�����ڴ��ʣ�����ݵ�����*/
	static char remain[1024 * 1024] = { 0 };
	/*���Լ�¼�ļ�ָ��λ��*/
	long fp_temp1;
	long fp_temp2;

	/*���ж�ȡ������Ƿ��Ѵ���group_name*/
	for (fp_temp1 = ftell(fp); fgets(temp, 256, fp); fp_temp1 = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
		if (strstr(temp, group_name))
		{
			/*�ҵ���һ�����λ��*/
			for (fp_temp2 = ftell(fp); fgets(temp, 256, fp); fp_temp2 = ftell(fp), clear_str(temp, 256))
				if (strstr(temp, "[") && strstr(temp, "]"))
				{
					fseek(fp, fp_temp2, SEEK_SET);
					break;
				}
			/*�����Ժ���������*/
			int len = file_length(fp) - fp_temp2;//��¼��Ҫ����ĳ���
			fread(remain, sizeof(char), len, fp);

			/*ɾ����������ʼ����������*/
			file_resize(fp, fp_temp1);

			/*�����ָ����������*/
			fseek(fp, 0, SEEK_END);
			for (int counter = 0; remain[counter]; counter++)
				fprintf(fp, "%c", remain[counter]);

			/*�ָ�ָ��λ��*/
			fseek(fp, fp_temp1, SEEK_SET);
			/*������+1*/
			group_del_counter++;
		}

	return group_del_counter;
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ַ���ǰlen��Ԫ�أ���Ϊ0
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void clear_str(char *str, int len)
{
	int i;
	for (i = 0; i < len; i++)
		str[i] = 0;
}

/***************************************************************************
�������ƣ�
��    �ܣ���ĳ�������ĳ��Ŀ
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int item_add(FILE *fp, const char *group_name, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fseek(fp, 0, SEEK_SET);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	int ret = 0;

	/*�����Ƿ��Ѵ���item_name*/
	if (item_name_exist(fp, group_name, item_name))
		return FAIL;

	/*����ΪNULL����ڼ������ļ����*/
	if (item_name == NULL)
	{
		fseek(fp, 0, SEEK_END);
		/*����ͬ���ͼ�����*/
		ret = fp_add(fp, item_name, item_value, item_type);

	}

	/*������ΪNULLʱ���ڸ����ʼ*/
	else {
		/*���Լ�¼�ļ�ָ��λ��*/
		long fp_temp;
		/*����һ�����ڴ��ʣ�����ݵ�����*/
		static char remain[1024 * 1024] = { 0 };
		/*���ж�ȡ������group_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp) != NULL; fp_temp = ftell(fp), clear_str(temp, 256))
		{

			if ((int)(strstr(temp, group_name)))
			{
				fp_temp = ftell(fp);

				/*�����Ժ���������*/
				int len = file_length(fp) - fp_temp;//��¼��Ҫ����ĳ���
				fread(remain, sizeof(char), len, fp);
				/*ɾ����������ʼ����������*/
				file_resize(fp, fp_temp);

				fseek(fp, 0, SEEK_END);
				/*����ͬ���ͼ�����*/
				ret=fp_add(fp, item_name, item_value, item_type);

				fseek(fp, 0, SEEK_END);
				/*�����ָ����������*/
				for (int counter = 0; remain[counter]; counter++)
					fprintf(fp, "%c", remain[counter]);
				clear_str(remain, 1024 * 1024);

				if (ret) {	
					return SUCCESS;
				}
				return FAIL;
			}
		}
	}//end of else

	if (ret)
		return SUCCESS;
	return FAIL;
}

/***************************************************************************
�������ƣ�
��    �ܣ�ɾ��ĳ����ĳ��Ŀ
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int item_del(FILE *fp, const char *group_name, const char *item_name)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	rewind(fp);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*����һ�����ڴ��ʣ�����ݵ�����*/
	static char remain[1024 * 1024] = { 0 };
	/*���Լ�¼�ļ�ָ��λ��*/
	long fp_temp;
	/*��¼ɾ��������*/
	int del_counter = 0;

	/*����ΪNULL����ڼ������ļ����*/
	if (item_name == NULL)
	{
		/*�����Ƿ��Ѵ���item_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
			if (strstr(temp, item_name))
			{
				/*�����Ժ���������*/
				int len = file_length(fp) - ftell(fp);//��¼��Ҫ����ĳ���
				fread(remain, sizeof(char), len, fp);

				/*ɾ�����п�ʼ��������*/
				file_resize(fp, fp_temp);

				/*�����ָ����������*/
				fseek(fp, 0, SEEK_END);
				for (int counter = 0; remain[counter]; counter++)
					fprintf(fp, "%c", remain[counter]);

				/*�ָ�ָ��λ��*/
				fseek(fp, fp_temp, SEEK_SET);

				/*������+1*/
				del_counter++;
			}
	}

	/*������ΪNULLʱ*/
	else
	{
		/*���ж�ȡ������group_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
			if (strstr(temp, group_name))
				for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
					if (strstr(temp, item_name) && !strstr(temp, "["))
					{
						/*�����Ժ���������*/
						int len = file_length(fp) - ftell(fp);//��¼��Ҫ����ĳ���
						fread(remain, sizeof(char), len, fp);

						/*ɾ�����п�ʼ��������*/
						file_resize(fp, fp_temp);

						/*�����ָ����������*/
						fseek(fp, 0, SEEK_END);
						for (int counter = 0; remain[counter]; counter++)
							fprintf(fp, "%c", remain[counter]);

						/*�ָ�ָ��λ��*/
						fseek(fp, fp_temp, SEEK_SET);

						/*������+1*/
						del_counter++;
					}
	}//end of else

	/*����ɾ��������*/
	return del_counter;
}

/***************************************************************************
�������ƣ�
��    �ܣ����ĳ����ĳ��Ŀ�Ƿ����
���������
�� �� ֵ�������򷵻�1���������򷵻�0
˵    ����
***************************************************************************/
int item_name_exist(FILE *fp, const char *group_name, const char *item_name)
{
	/*���Լ�¼ָ���ʼλ��*/
	long fp_temp = ftell(fp);

	/*���ļ�ָ�뷵�ؿ�ͷ*/
	rewind(fp);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	int flag_exist = 0;

	if (item_name == NULL) {
		for (; fgets(temp, 256, fp); clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				flag_exist++;
				break;
			}
	}
	else
	{
		for (; fgets(temp, 256, fp); clear_str(temp, 256))
			if (strstr(temp, group_name))
				for (; fgets(temp, 256, fp); clear_str(temp, 256))
				{
					/*��������һ������ֹͣ*/
					if (strstr(temp, "["))
						break;
					if (strstr(temp, item_name))
					{
						flag_exist++;
						break;
					}
				}
	}//end of else NULL

	/*��ָ�뷵����ԭ��λ��*/
	fseek(fp, fp_temp, SEEK_SET);
	return flag_exist;
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ĳ����ĳ��Ŀ��ֵ
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int item_update(FILE *fp, const char *group_name, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*����һ�����ڴ��ʣ�����ݵ�����*/
	static char remain[1024 * 1024] = { 0 };
	/*���Լ�¼�ļ�ָ��λ��*/
	long fp_temp;
	/*��¼�Ƿ��Ѹ���*/
	int flag_update = 0;

	/*��item_name�����ڣ�����group_name���м������*/
	flag_update = item_add(fp, group_name, item_name, item_value, item_type);

	if (flag_update)
		return SUCCESS;

	/*���ļ�ָ�뷵�ؿ�ͷ*/
	rewind(fp);

	/*����ΪNULLʱ*/
	if (group_name == NULL)
	{
		/*�����Ƿ��Ѵ���item_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				/*�������ظ�����ɾ��������*/
				if (flag_update)
				{
					item_del(fp, group_name, item_name);
					/*�ָ�ָ��λ��*/
					fseek(fp, fp_temp, SEEK_SET);
				}
				else {
					/*�����Ժ���������*/
					int len = file_length(fp) - ftell(fp);//��¼��Ҫ����ĳ���
					fread(remain, sizeof(char), len, fp);

					/*ɾ�����п�ʼ��������*/
					file_resize(fp, fp_temp);

					/*����ͬ���͸�������*/
					fseek(fp, 0, SEEK_END);
					flag_update=fp_add(fp, item_name, item_value, item_type);

					/*�����ָ����������*/
					for (int counter = 0; remain[counter]; counter++)
						fprintf(fp, "%c", remain[counter]);
					clear_str(remain, 1024 * 1024);

					/*�ָ�ָ��λ��*/
					fseek(fp, fp_temp, SEEK_SET);
				}//end of else
			}

	}//end of if NULL

	/*��ΪNULLʱ*/
	else
	{
		/*���ж�ȡ������group_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256))
			if (strstr(temp, group_name) && !flag_update)
				for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256))
				{
					/*��������һ�飬��ֹͣ*/
					if (strstr(temp, "["))
						break;

					if (strstr(temp, item_name))
					{
						/*�������ظ�����ɾ��������*/
						if (flag_update)
						{
							item_del(fp, group_name, item_name);
							/*�ָ�ָ��λ��*/
							fseek(fp, fp_temp, SEEK_SET);
						}
						else {
							/*�����Ժ���������*/
							int len = file_length(fp) - ftell(fp);//��¼��Ҫ����ĳ���
							fread(remain, sizeof(char), len, fp);

							/*ɾ�����п�ʼ��������*/
							file_resize(fp, fp_temp);
							fp_temp = ftell(fp);

							/*����ͬ���͸�������*/
							fseek(fp, 0, SEEK_END);
							flag_update = fp_add(fp, item_name, item_value, item_type);

							/*�����ָ����������*/
							for (int counter = 0; remain[counter]; counter++)
								fprintf(fp, "%c", remain[counter]);
							clear_str(remain, 1024 * 1024);

							/*�ָ�ָ��λ��*/
							fseek(fp, fp_temp, SEEK_SET);
						}
					}
				}
	}//end of else NULL

	if (flag_update)
		return SUCCESS;
	return FAIL;
}

/***************************************************************************
�������ƣ�
��    �ܣ��������ļ��ж�ȡĳ��Ŀ��ֵ
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int item_get_value(FILE *fp, const char *group_name, const char *item_name, void *item_value, const enum ITEM_TYPE item_type)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	rewind(fp);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*���Դ��valueֵ*/
	char value[256] = { 0 };
	/*���Լ�¼�Ƿ��ȡ*/
	int flag_value = 0;

	/*����ΪNULLʱ*/
	if (item_name == NULL)
	{
		/*�����Ƿ��Ѵ���item_name*/
		for (; fgets(temp, 256, fp) && !flag_value; clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				/*��'#'��';'��0�����Ժ��������*/
				polish_str(temp);

				/*��=����������뵽value��*/
				sprintf(value, "%s", strstr(temp, "=") + 1);

				/*��item_valueΪNULL��ʲôҲ����*/
				if (item_value == NULL)
					;
				/*��NULLʱ��value�ڵ�ֵ����item_value��*/
				else
				{
					/*����ͬ���Ͷ�ȡ*/
					if (TYPE_INT == item_type)
						sscanf(value, "%d", (int*)(item_value));
					if (TYPE_DOUBLE == item_type)
						sscanf(value, "%lf", (double*)(item_value));
					if (TYPE_STRING == item_type)
						sscanf(value, "%s", (char*)(item_value));
					if (TYPE_CHARACTER == item_type)
						sscanf(value, "%c", (char*)(item_value));
					if (TYPE_NULL == item_type)
						;
				}//end of else

				/*ȷ�϶�ȡ*/
				flag_value++;
			}
	}//end of if NULL

	/*������NULLʱ*/
	else
	{
		/*���ж�ȡ������group_name*/
		for (; fgets(temp, 256, fp) && !flag_value; clear_str(temp, 256))
			if (strstr(temp, group_name) && !flag_value)
				for (; fgets(temp, 256, fp); clear_str(temp, 256))
				{
					/*��������һ�飬��ֹͣ*/
					if (strstr(temp, "["))
						break;

					if (strstr(temp, item_name))
					{
						/*��'#'��';'��0�����Ժ��������*/
						polish_str(temp);

						/*��=����������뵽value��*/
						sprintf(value, "%s", strstr(temp, "=") + 1);

						/*��item_valueΪNULL��ʲôҲ����*/
						if (item_value == NULL)
							;
						/*��NULLʱ��value�ڵ�ֵ����item_value��*/
						else
						{
							/*����ͬ���Ͷ�ȡ*/
							if (TYPE_INT == item_type)
								sscanf(value, "%d", (int*)(item_value));
							if (TYPE_DOUBLE == item_type)
								sscanf(value, "%lf", (double*)(item_value));
							if (TYPE_STRING == item_type)
								sscanf(value, "%s", (char*)(item_value));
							if (TYPE_CHARACTER == item_type)
								sscanf(value, "%c", (char*)(item_value));
							if (TYPE_NULL == item_type)
								;
						}//end of else

						 /*ȷ�϶�ȡ*/
						flag_value++;
					}
				}
	}//end of else
	if (flag_value)
		return SUCCESS;
	return FAIL;
}

/***************************************************************************
�������ƣ�
��    �ܣ������ַ���'#'��';'�������
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void polish_str(char str[])
{
	int p = 0;

	/*��'#'��';'��0�����Ժ��������*/
	for (p = 0; str[p]; p++)
		if (str[p] == '#' || str[p] == ';')
			str[p] = 0;
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ͬ�������ļ������Ŀ
���������
�� �� ֵ�����ɹ��򷵻�1��ʧ���򷵻�0
˵    ����
***************************************************************************/
int fp_add(FILE* fp, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	if (TYPE_INT == item_type)
		return fprintf(fp, "%s=%d\n", item_name, *((int*)(item_value)));
	if (TYPE_DOUBLE == item_type)
		return fprintf(fp, "%s=%lf\n", item_name, *((double*)(item_value)));
	if (TYPE_STRING == item_type)
		return fprintf(fp, "%s=%s\n", item_name, (char*)(item_value));
	if (TYPE_CHARACTER == item_type)
		return fprintf(fp, "%s=%c\n", item_name, *((char*)(item_value)));
	if (TYPE_NULL == item_type)
		return fprintf(fp, "%s=\n", item_name);
}

/***************************************************************************
�������ƣ�
��    �ܣ�
���������
�� �� ֵ��
˵    �����˺�������ŵ� common_cfgfile_tools.c ����Ϊ�ڲ����ߺ���
***************************************************************************/
static int file_length(FILE *fp)
{
	int file_len, now_pos;

	/* ȡfp��ǰָ��λ�� */
	now_pos = ftell(fp);

	/* ���ļ�ָ���ƶ�����󣬴�ʱtellp�Ľ�������ļ���С */
	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);

	/* ָ���ƶ��غ�������ǰ��ԭλ�� */
	fseek(fp, now_pos, SEEK_SET);

	return file_len;
}

/***************************************************************************
�������ƣ�
��    �ܣ�
���������
�� �� ֵ��
˵    �����˺�������ŵ� common_cfgfile_tools.c ����Ϊ�ڲ����ߺ���
***************************************************************************/
static int file_resize(FILE *fp, int newsize)
{
	int now_pos = ftell(fp);
	/* �����С�Ǹ�����ֱ�ӷ���-1 */
	if (newsize < 0)
		return -1;

	_chsize(_fileno(fp), newsize);

	/* �����ǰ�ļ�ָ�볬�����ļ���С����rewind */
	if (now_pos > newsize)
		rewind(fp);

	return 0;
}