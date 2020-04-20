/*1751533 ��3 ���ӷ�*/
#include"../common/common_cfgfile_tools.hpp"

/***************************************************************************
�������ƣ�
��    �ܣ���ֻ�����д��ĳ�ļ�
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int open_cfgfile(fstream &fp,const char *cfgname, int opt)
{

	/*��ֻ����ʽ��*/
	if (opt == OPEN_OPT_RDONLY)
		fp.open(cfgname, ios::in | ios::binary);
	/*�Զ�д��ʽ��*/
	if (opt == OPEN_OPT_RDWR) {
		fp.open(cfgname, ios::in  | ios::out | ios::binary);
		if (fp.is_open() == 0)
			fp.open(cfgname, ios::out | ios::binary);
	}
	if(fp.is_open()==0)
		return FAIL;
	return SUCCESS;
}

/***************************************************************************
�������ƣ�
��    �ܣ��ر�һ���ļ�
���������
�� �� ֵ��
˵    ����
***************************************************************************/
void close_cfgfile(fstream &fp)
{
	fp.close();
}

/***************************************************************************
�������ƣ�
��    �ܣ����һ����
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int group_add(fstream &fp, const char *group_name)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.seekg(0, ios::beg);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256];

	/*���ж�ȡ������Ƿ��Ѵ���group_name*/
	for (; fp.getline(temp, 256);)
		/*���Ѵ���group_name,�򷵻�0��FAIL*/
		if (strstr(temp, group_name))
			return FAIL;

	fp.clear();
	fp.seekg(0, ios::end);

	/*д��ɹ��򷵻�1��SUCCESS*/
	if (fp << "[" << group_name << "]" << "\r\n" << "\r\n") 
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
int group_del(fstream &fp, const char *filename, const char *group_name)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.clear();
	fp.seekg(0, ios::beg);

	/*��¼ɾ������*/
	int group_del_counter = 0;

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*����һ�����ڴ��ʣ�����ݵ�����*/
	static char remain[1024 * 1024] = { 0 };
	/*���Լ�¼�ļ�ָ��λ��*/
	int fp_temp1;
	int fp_temp2;

	/*���ж�ȡ������Ƿ��Ѵ���group_name*/
	for (fp_temp1 = int(int(fp.tellg())); fp.getline(temp, 256); fp_temp1 = int(fp.tellg()), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
		if (strstr(temp, group_name))
		{
			/*�ҵ���һ�����λ��*/
			for (fp_temp2 = int(fp.tellg()); fp.getline(temp, 256); fp_temp2 = int(fp.tellg()), clear_str(temp, 256))
				if (strstr(temp, "[") && strstr(temp, "]"))
				{
					fp.clear();
					fp.seekg(fp_temp2, ios::beg);
					break;
				}
			/*�����Ժ���������*/
			int len = file_length(fp) - fp_temp2;//��¼��Ҫ����ĳ���
			fp.read(remain, len);

			/*ɾ����������ʼ����������*/
			file_resize(filename, fp, fp_temp1);

			/*�����ָ����������*/
			fp.clear();
			fp.seekg(0, ios::end);
			for (int counter = 0; remain[counter]; counter++)
				fp << remain[counter];

			/*�ָ�ָ��λ��*/
			fp.clear();
			fp.seekg(fp_temp1, ios::beg);
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
int item_add(fstream &fp, const char *group_name, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.clear();
	fp.seekg(0, ios::beg);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	int ret = 0;

	/*�����Ƿ��Ѵ���item_name*/
	if (item_name_exist(fp, group_name, item_name))
		return FAIL;

	/*����ΪNULL����ڼ������ļ����*/
	if (item_name == NULL)
	{
		fp.clear();
		fp.seekg(0, ios::end);
		/*����ͬ���ͼ�����*/
		fp_add(fp, item_name, item_value, item_type);
		ret++;
	}

	/*������ΪNULLʱ���ڸ����ʼ*/
	else {
		/*���Լ�¼�ļ�ָ��λ��*/
		int fp_temp;
		/*����һ�����ڴ��ʣ�����ݵ�����*/
		static char remain[1024 * 1024] = { 0 };
		/*���ж�ȡ������group_name*/
		for (fp_temp = int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256))
		{

			if ((int)(strstr(temp, group_name)))
			{
				fp_temp = int(fp.tellg());

				/*�����Ժ���������*/
				int len = file_length(fp) - fp_temp;//��¼��Ҫ����ĳ���
				fp.read(remain, len);
				/*ɾ����������ʼ����������*/
				fp.clear();
				fp.seekg(fp_temp, ios::beg);
				for (int counter = 0; counter < len; counter++)
					fp << ' ';

				fp.clear();
				fp.seekg(fp_temp, ios::beg);
				/*����ͬ���ͼ�����*/
				fp_add(fp, item_name, item_value, item_type);
				ret++;

				/*�����ָ����������*/
				for (int counter = 0; remain[counter]; counter++)
					fp << remain[counter];
				clear_str(remain, 1024 * 1024);

				if (ret) 
					return SUCCESS;
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
int item_del(fstream &fp, const char *filename, const char *group_name, const char *item_name)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.clear();
	fp.seekg(0, ios::beg);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*����һ�����ڴ��ʣ�����ݵ�����*/
	static char remain[1024 * 1024] = { 0 };
	/*���Լ�¼�ļ�ָ��λ��*/
	int fp_temp;
	/*��¼ɾ��������*/
	int del_counter = 0;

	/*����ΪNULL����ڼ������ļ����*/
	if (item_name == NULL)
	{
		/*�����Ƿ��Ѵ���item_name*/
		for (fp_temp = (int)int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
			if (strstr(temp, item_name))
			{
				/*�����Ժ���������*/
				int len = file_length(fp) - int(fp.tellg());//��¼��Ҫ����ĳ���
				fp.read(remain, len);

				/*ɾ�����п�ʼ��������*/
				file_resize(filename, fp, fp_temp);

				/*�����ָ����������*/
				fp.clear();
				fp.seekg(0, ios::end);
				for (int counter = 0; remain[counter]; counter++)
					fp << remain[counter];

				/*�ָ�ָ��λ��*/
				fp.clear();
				fp.seekg(fp_temp, ios::beg);

				/*������+1*/
				del_counter++;
			}
	}

	/*������ΪNULLʱ*/
	else
	{
		/*���ж�ȡ������group_name*/
		for (fp_temp = int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
			if (strstr(temp, group_name))
				for (fp_temp = int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
					if (strstr(temp, item_name) && !strstr(temp, "["))
					{
						/*�����Ժ���������*/
						int len = file_length(fp) - int(fp.tellg());//��¼��Ҫ����ĳ���
						fp.read(remain, len);

						/*ɾ�����п�ʼ��������*/
						file_resize(filename, fp, fp_temp);

						/*�����ָ����������*/
						fp.clear();
						fp.seekg(0, ios::end);
						for (int counter = 0; remain[counter]; counter++)
							fp << remain[counter];

						/*�ָ�ָ��λ��*/
						fp.clear();
						fp.seekg(fp_temp, ios::beg);

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
int item_name_exist(fstream &fp, const char *group_name, const char *item_name)
{
	/*���Լ�¼ָ���ʼλ��*/
	int fp_temp = int(fp.tellg());

	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.clear();
	fp.seekg(0, ios::beg);

	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	int flag_exist = 0;

	if (item_name == NULL) {
		for (; fp.getline(temp, 256); clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				flag_exist++;
				break;
			}
	}
	else
	{
		for (; fp.getline(temp, 256); clear_str(temp, 256))
			if (strstr(temp, group_name))
				for (; fp.getline(temp, 256); clear_str(temp, 256))
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
	fp.clear();
	fp.seekg(fp_temp, ios::beg);
	return flag_exist;
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ĳ����ĳ��Ŀ��ֵ
���������
�� �� ֵ��
˵    ����
***************************************************************************/
int item_update(fstream &fp, const char *filename, const char *group_name, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	/*����һ�����ڴ�ŵ������ݵ��ַ���*/
	char temp[256] = { 0 };
	/*����һ�����ڴ��ʣ�����ݵ�����*/
	static char remain[1024 * 1024] = { 0 };
	/*���Լ�¼�ļ�ָ��λ��*/
	int fp_temp;
	/*��¼�Ƿ��Ѹ���*/
	int flag_update = 0;

	/*��item_name�����ڣ�����group_name���м������*/
	flag_update = item_add(fp, group_name, item_name, item_value, item_type);

	if (flag_update)
		return SUCCESS;

	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.clear();
	fp.seekg(0, ios::beg);

	/*����ΪNULLʱ*/
	if (group_name == NULL)
	{
		/*�����Ƿ��Ѵ���item_name*/
		for (fp_temp = int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				/*�������ظ�����ɾ��������*/
				if (flag_update)
				{
					item_del(fp,filename, group_name, item_name);
					/*�ָ�ָ��λ��*/
					fp.clear();
					fp.seekg(fp_temp, ios::beg);
				}
				else {
					/*�����Ժ���������*/
					int len = file_length(fp) - int(fp.tellg());//��¼��Ҫ����ĳ���
					fp.read(remain, len);

					/*ɾ�����п�ʼ��������*/
					file_resize(filename,fp, fp_temp);

					/*����ͬ���͸�������*/
					fp.clear();
					fp.seekg(0, ios::end);
					fp_add(fp, item_name, item_value, item_type);
					flag_update++;

					/*�����ָ����������*/
					for (int counter = 0; remain[counter]; counter++)
						fp << remain[counter];
					clear_str(remain, 1024 * 1024);

					/*�ָ�ָ��λ��*/
					fp.clear();
					fp.seekg(fp_temp, ios::beg);
				}//end of else
			}

	}//end of if NULL

	 /*��ΪNULLʱ*/
	else
	{
		/*���ж�ȡ������group_name*/
		for (fp_temp = int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256))
			if (strstr(temp, group_name) && !flag_update)
				for (fp_temp = int(fp.tellg()); fp.getline(temp, 256); fp_temp = int(fp.tellg()), clear_str(temp, 256))
				{
					/*��������һ�飬��ֹͣ*/
					if (strstr(temp, "["))
						break;

					if (strstr(temp, item_name))
					{
						/*�������ظ�����ɾ��������*/
						if (flag_update)
						{
							item_del(fp,filename, group_name, item_name);
							/*�ָ�ָ��λ��*/
							fp.clear();
							fp.seekg(fp_temp, ios::beg);
						}
						else {
							/*�����Ժ���������*/
							int len = file_length(fp) - int(fp.tellg());//��¼��Ҫ����ĳ���
							fp.read(remain, len);

							/*ɾ�����п�ʼ��������*/
							file_resize(filename, fp, fp_temp);
							fp_temp = int(fp.tellg());

							/*����ͬ���͸�������*/
							fp.clear();
							fp.seekg(0, ios::end);
							fp_add(fp, item_name, item_value, item_type);
							flag_update++;

							/*�����ָ����������*/
							for (int counter = 0; remain[counter]; counter++)
								fp << remain[counter];
							clear_str(remain, 1024 * 1024);

							/*�ָ�ָ��λ��*/
							fp.clear();
							fp.seekg(fp_temp, ios::beg);
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
int item_get_value(fstream &fp, const char *group_name, const char *item_name, void *item_value, const enum ITEM_TYPE item_type)
{
	/*���ļ�ָ�뷵�ؿ�ͷ*/
	fp.clear();
	fp.seekg(0, ios::beg);

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
		for (; fp.getline(temp, 256) && !flag_value; clear_str(temp, 256))
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
					if (TYPE_NULL == item_type) {

					}
						
				}//end of else

				 /*ȷ�϶�ȡ*/
				flag_value++;
			}
	}//end of if NULL

	 /*������NULLʱ*/
	else
	{
		/*���ж�ȡ������group_name*/
		for (; fp.getline(temp, 256) && !flag_value; clear_str(temp, 256))
			if (strstr(temp, group_name) && !flag_value)
				for (; fp.getline(temp, 256); clear_str(temp, 256))
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
							if (TYPE_NULL == item_type) {

							}
								
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
��    �ܣ�
���������
�� �� ֵ��
˵    �����˺�������ŵ� common_cfgfile_tools.cpp ����Ϊ�ڲ����ߺ���
***************************************************************************/
static int file_length(fstream &fp)
{
	int file_len, now_pos;

	/* ��ǰΪ����״̬�򷵻�-1 */
	if (fp.fail())
		return -1;

	/* ȡfp��ǰָ��λ�� */
	now_pos = int(int(fp.tellg()));

	/* ���ļ�ָ���ƶ�����󣬴�ʱtellp�Ľ�������ļ���С */
	fp.seekg(0, ios::end);
	file_len = int(fp.tellp());

	/* ָ���ƶ��غ�������ǰ��ԭλ�� */
	fp.seekg(now_pos, ios::beg);

	return file_len;
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ͬ�������ļ������Ŀ
���������
�� �� ֵ�����ɹ��򷵻�1��ʧ���򷵻�0
˵    ����
***************************************************************************/
int fp_add(fstream &fp, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	if (TYPE_INT == item_type)
		fp << item_name << "=" << *((int*)(item_value)) << "\r\n";
	if (TYPE_DOUBLE == item_type)
		fp << item_name << "=" << *((double*)(item_value)) << "\r\n";
	if (TYPE_STRING == item_type)
		fp << item_name << "=" << (char*)(item_value) << "\r\n";
	if (TYPE_CHARACTER == item_type)
		fp << item_name << "=" << *(char*)(item_value) << "\r\n";
	if (TYPE_NULL == item_type)
		fp << item_name << "=" << "\r\n";

	return 0;
}

/***************************************************************************
�������ƣ�
��    �ܣ�
���������
�� �� ֵ��
˵    �����˺�������ŵ� common_cfgfile_tools.cpp ����Ϊ�ڲ����ߺ���
***************************************************************************/
static int file_resize(const char *filename, fstream &fp, int newsize)
{
	int now_pos;

	/* ��ǰΪ����״̬�򷵻�-1 */
	if (fp.fail())
		return -1;

	/* ȡfp��ǰָ��λ�� */
	now_pos = int(int(fp.tellg()));

	/* �����С�Ǹ�����ֱ�ӷ���-1 */
	if (newsize < 0)
		return -1;

	experimental::filesystem::resize_file(filename, newsize);

	/* �����ǰ�ļ�ָ�볬�����ļ���С����ص��ļ�ͷ */
	if (now_pos > newsize)
		fp.seekg(0, ios::beg);

	return 0;
}