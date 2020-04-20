/*1751533 计3 宋子帆*/
#include"../common/common_cfgfile_tools.h"

/***************************************************************************
函数名称：
功    能：以只读或读写打开某文件
输入参数：
返 回 值：
说    明：
***************************************************************************/
FILE *open_cfgfile(const char *cfgname, int opt)
{
	FILE *fp = NULL;

	/*以只读方式打开*/
	if (opt == OPEN_OPT_RDONLY)
		fp = fopen(cfgname, "r");
	/*以读写方式打开*/
	if (opt == OPEN_OPT_RDWR) {
		fp = fopen(cfgname, "r+");
		if (fp == NULL)
			fp = fopen(cfgname, "w+");
	}
	/*返回文件指针*/
	return fp;
}

/***************************************************************************
函数名称：
功    能：关闭一个文件
输入参数：
返 回 值：
说    明：
***************************************************************************/
void close_cfgfile(FILE *fp)
{
	fclose(fp);
}

/***************************************************************************
函数名称：
功    能：添加一个组
输入参数：
返 回 值：
说    明：
***************************************************************************/
int group_add(FILE *fp, const char *group_name)
{
	/*将文件指针返回开头*/
	rewind(fp);

	/*定义一个用于存放单行内容的字符串*/
	char temp[256];

	/*逐行读取，检查是否已存在group_name*/
	for (; fgets(temp, 256, fp);)
		/*若已存在group_name,则返回0即FAIL*/
		if (strstr(temp, group_name))
			return FAIL;

	/*写入成功则返回1即SUCCESS*/
	if (fprintf(fp, "[%s]\n\n", group_name))
		return SUCCESS;

	return FAIL;
}

/***************************************************************************
函数名称：
功    能：删除一个组及其内容
输入参数：
返 回 值：
说    明：
***************************************************************************/
int group_del(FILE *fp, const char *group_name)
{
	/*将文件指针返回开头*/
	rewind(fp);

	/*记录删除组数*/
	int group_del_counter = 0;

	/*定义一个用于存放单行内容的字符串*/
	char temp[256] = { 0 };
	/*定义一个用于存放剩余内容的数组*/
	static char remain[1024 * 1024] = { 0 };
	/*用以记录文件指针位置*/
	long fp_temp1;
	long fp_temp2;

	/*逐行读取，检查是否已存在group_name*/
	for (fp_temp1 = ftell(fp); fgets(temp, 256, fp); fp_temp1 = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
		if (strstr(temp, group_name))
		{
			/*找到下一个组的位置*/
			for (fp_temp2 = ftell(fp); fgets(temp, 256, fp); fp_temp2 = ftell(fp), clear_str(temp, 256))
				if (strstr(temp, "[") && strstr(temp, "]"))
				{
					fseek(fp, fp_temp2, SEEK_SET);
					break;
				}
			/*保存以后所有内容*/
			int len = file_length(fp) - fp_temp2;//记录需要保存的长度
			fread(remain, sizeof(char), len, fp);

			/*删除该组名开始处所有内容*/
			file_resize(fp, fp_temp1);

			/*在最后恢复保存的内容*/
			fseek(fp, 0, SEEK_END);
			for (int counter = 0; remain[counter]; counter++)
				fprintf(fp, "%c", remain[counter]);

			/*恢复指针位置*/
			fseek(fp, fp_temp1, SEEK_SET);
			/*计数器+1*/
			group_del_counter++;
		}

	return group_del_counter;
}

/***************************************************************************
函数名称：
功    能：清空字符串前len个元素，置为0
输入参数：
返 回 值：
说    明：
***************************************************************************/
void clear_str(char *str, int len)
{
	int i;
	for (i = 0; i < len; i++)
		str[i] = 0;
}

/***************************************************************************
函数名称：
功    能：在某组中添加某项目
输入参数：
返 回 值：
说    明：
***************************************************************************/
int item_add(FILE *fp, const char *group_name, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	/*将文件指针返回开头*/
	fseek(fp, 0, SEEK_SET);

	/*定义一个用于存放单行内容的字符串*/
	char temp[256] = { 0 };
	int ret = 0;

	/*查找是否已存在item_name*/
	if (item_name_exist(fp, group_name, item_name))
		return FAIL;

	/*组名为NULL则加在简单配置文件最后*/
	if (item_name == NULL)
	{
		fseek(fp, 0, SEEK_END);
		/*按不同类型加入项*/
		ret = fp_add(fp, item_name, item_value, item_type);

	}

	/*组名不为NULL时加在该组最开始*/
	else {
		/*用以记录文件指针位置*/
		long fp_temp;
		/*定义一个用于存放剩余内容的数组*/
		static char remain[1024 * 1024] = { 0 };
		/*逐行读取，查找group_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp) != NULL; fp_temp = ftell(fp), clear_str(temp, 256))
		{

			if ((int)(strstr(temp, group_name)))
			{
				fp_temp = ftell(fp);

				/*保存以后所有内容*/
				int len = file_length(fp) - fp_temp;//记录需要保存的长度
				fread(remain, sizeof(char), len, fp);
				/*删除该组名开始处所有内容*/
				file_resize(fp, fp_temp);

				fseek(fp, 0, SEEK_END);
				/*按不同类型加入项*/
				ret=fp_add(fp, item_name, item_value, item_type);

				fseek(fp, 0, SEEK_END);
				/*在最后恢复保存的内容*/
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
函数名称：
功    能：删除某组中某项目
输入参数：
返 回 值：
说    明：
***************************************************************************/
int item_del(FILE *fp, const char *group_name, const char *item_name)
{
	/*将文件指针返回开头*/
	rewind(fp);

	/*定义一个用于存放单行内容的字符串*/
	char temp[256] = { 0 };
	/*定义一个用于存放剩余内容的数组*/
	static char remain[1024 * 1024] = { 0 };
	/*用以记录文件指针位置*/
	long fp_temp;
	/*记录删除的项数*/
	int del_counter = 0;

	/*组名为NULL则加在简单配置文件最后*/
	if (item_name == NULL)
	{
		/*查找是否已存在item_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
			if (strstr(temp, item_name))
			{
				/*保存以后所有内容*/
				int len = file_length(fp) - ftell(fp);//记录需要保存的长度
				fread(remain, sizeof(char), len, fp);

				/*删除该行开始所有内容*/
				file_resize(fp, fp_temp);

				/*在最后恢复保存的内容*/
				fseek(fp, 0, SEEK_END);
				for (int counter = 0; remain[counter]; counter++)
					fprintf(fp, "%c", remain[counter]);

				/*恢复指针位置*/
				fseek(fp, fp_temp, SEEK_SET);

				/*计数器+1*/
				del_counter++;
			}
	}

	/*组名不为NULL时*/
	else
	{
		/*逐行读取，查找group_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
			if (strstr(temp, group_name))
				for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256), clear_str(remain, 1024 * 1024))
					if (strstr(temp, item_name) && !strstr(temp, "["))
					{
						/*保存以后所有内容*/
						int len = file_length(fp) - ftell(fp);//记录需要保存的长度
						fread(remain, sizeof(char), len, fp);

						/*删除该行开始所有内容*/
						file_resize(fp, fp_temp);

						/*在最后恢复保存的内容*/
						fseek(fp, 0, SEEK_END);
						for (int counter = 0; remain[counter]; counter++)
							fprintf(fp, "%c", remain[counter]);

						/*恢复指针位置*/
						fseek(fp, fp_temp, SEEK_SET);

						/*计数器+1*/
						del_counter++;
					}
	}//end of else

	/*返回删除的项数*/
	return del_counter;
}

/***************************************************************************
函数名称：
功    能：检查某组中某项目是否存在
输入参数：
返 回 值：存在则返回1，不存在则返回0
说    明：
***************************************************************************/
int item_name_exist(FILE *fp, const char *group_name, const char *item_name)
{
	/*用以记录指针初始位置*/
	long fp_temp = ftell(fp);

	/*将文件指针返回开头*/
	rewind(fp);

	/*定义一个用于存放单行内容的字符串*/
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
					/*若读到下一个组则停止*/
					if (strstr(temp, "["))
						break;
					if (strstr(temp, item_name))
					{
						flag_exist++;
						break;
					}
				}
	}//end of else NULL

	/*将指针返回至原来位置*/
	fseek(fp, fp_temp, SEEK_SET);
	return flag_exist;
}

/***************************************************************************
函数名称：
功    能：更新某组中某项目的值
输入参数：
返 回 值：
说    明：
***************************************************************************/
int item_update(FILE *fp, const char *group_name, const char *item_name, const void *item_value, const enum ITEM_TYPE item_type)
{
	/*定义一个用于存放单行内容的字符串*/
	char temp[256] = { 0 };
	/*定义一个用于存放剩余内容的数组*/
	static char remain[1024 * 1024] = { 0 };
	/*用以记录文件指针位置*/
	long fp_temp;
	/*记录是否已更新*/
	int flag_update = 0;

	/*若item_name不存在，则在group_name组中加入该项*/
	flag_update = item_add(fp, group_name, item_name, item_value, item_type);

	if (flag_update)
		return SUCCESS;

	/*将文件指针返回开头*/
	rewind(fp);

	/*组名为NULL时*/
	if (group_name == NULL)
	{
		/*查找是否已存在item_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				/*若存在重复项则删除多余项*/
				if (flag_update)
				{
					item_del(fp, group_name, item_name);
					/*恢复指针位置*/
					fseek(fp, fp_temp, SEEK_SET);
				}
				else {
					/*保存以后所有内容*/
					int len = file_length(fp) - ftell(fp);//记录需要保存的长度
					fread(remain, sizeof(char), len, fp);

					/*删除该行开始所有内容*/
					file_resize(fp, fp_temp);

					/*按不同类型更新内容*/
					fseek(fp, 0, SEEK_END);
					flag_update=fp_add(fp, item_name, item_value, item_type);

					/*在最后恢复保存的内容*/
					for (int counter = 0; remain[counter]; counter++)
						fprintf(fp, "%c", remain[counter]);
					clear_str(remain, 1024 * 1024);

					/*恢复指针位置*/
					fseek(fp, fp_temp, SEEK_SET);
				}//end of else
			}

	}//end of if NULL

	/*不为NULL时*/
	else
	{
		/*逐行读取，查找group_name*/
		for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256))
			if (strstr(temp, group_name) && !flag_update)
				for (fp_temp = ftell(fp); fgets(temp, 256, fp); fp_temp = ftell(fp), clear_str(temp, 256))
				{
					/*若读到下一组，则停止*/
					if (strstr(temp, "["))
						break;

					if (strstr(temp, item_name))
					{
						/*若存在重复项则删除多余项*/
						if (flag_update)
						{
							item_del(fp, group_name, item_name);
							/*恢复指针位置*/
							fseek(fp, fp_temp, SEEK_SET);
						}
						else {
							/*保存以后所有内容*/
							int len = file_length(fp) - ftell(fp);//记录需要保存的长度
							fread(remain, sizeof(char), len, fp);

							/*删除该行开始所有内容*/
							file_resize(fp, fp_temp);
							fp_temp = ftell(fp);

							/*按不同类型更新内容*/
							fseek(fp, 0, SEEK_END);
							flag_update = fp_add(fp, item_name, item_value, item_type);

							/*在最后恢复保存的内容*/
							for (int counter = 0; remain[counter]; counter++)
								fprintf(fp, "%c", remain[counter]);
							clear_str(remain, 1024 * 1024);

							/*恢复指针位置*/
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
函数名称：
功    能：从配置文件中读取某项目的值
输入参数：
返 回 值：
说    明：
***************************************************************************/
int item_get_value(FILE *fp, const char *group_name, const char *item_name, void *item_value, const enum ITEM_TYPE item_type)
{
	/*将文件指针返回开头*/
	rewind(fp);

	/*定义一个用于存放单行内容的字符串*/
	char temp[256] = { 0 };
	/*用以存放value值*/
	char value[256] = { 0 };
	/*用以记录是否读取*/
	int flag_value = 0;

	/*组名为NULL时*/
	if (item_name == NULL)
	{
		/*查找是否已存在item_name*/
		for (; fgets(temp, 256, fp) && !flag_value; clear_str(temp, 256))
			if (strstr(temp, item_name))
			{
				/*将'#'或';'置0，忽略后面的内容*/
				polish_str(temp);

				/*将=后的内容输入到value中*/
				sprintf(value, "%s", strstr(temp, "=") + 1);

				/*若item_value为NULL，什么也不干*/
				if (item_value == NULL)
					;
				/*非NULL时将value内的值读到item_value中*/
				else
				{
					/*按不同类型读取*/
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

				/*确认读取*/
				flag_value++;
			}
	}//end of if NULL

	/*组名非NULL时*/
	else
	{
		/*逐行读取，查找group_name*/
		for (; fgets(temp, 256, fp) && !flag_value; clear_str(temp, 256))
			if (strstr(temp, group_name) && !flag_value)
				for (; fgets(temp, 256, fp); clear_str(temp, 256))
				{
					/*若读到下一组，则停止*/
					if (strstr(temp, "["))
						break;

					if (strstr(temp, item_name))
					{
						/*将'#'或';'置0，忽略后面的内容*/
						polish_str(temp);

						/*将=后的内容输入到value中*/
						sprintf(value, "%s", strstr(temp, "=") + 1);

						/*若item_value为NULL，什么也不干*/
						if (item_value == NULL)
							;
						/*非NULL时将value内的值读到item_value中*/
						else
						{
							/*按不同类型读取*/
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

						 /*确认读取*/
						flag_value++;
					}
				}
	}//end of else
	if (flag_value)
		return SUCCESS;
	return FAIL;
}

/***************************************************************************
函数名称：
功    能：忽略字符串'#'和';'后的内容
输入参数：
返 回 值：
说    明：
***************************************************************************/
void polish_str(char str[])
{
	int p = 0;

	/*将'#'或';'置0，忽略后面的内容*/
	for (p = 0; str[p]; p++)
		if (str[p] == '#' || str[p] == ';')
			str[p] = 0;
}

/***************************************************************************
函数名称：
功    能：按不同类型向文件添加项目
输入参数：
返 回 值：若成功则返回1，失败则返回0
说    明：
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
函数名称：
功    能：
输入参数：
返 回 值：
说    明：此函数允许放到 common_cfgfile_tools.c 中做为内部工具函数
***************************************************************************/
static int file_length(FILE *fp)
{
	int file_len, now_pos;

	/* 取fp当前指针位置 */
	now_pos = ftell(fp);

	/* 将文件指针移动到最后，此时tellp的结果就是文件大小 */
	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);

	/* 指针移动回函数调用前的原位置 */
	fseek(fp, now_pos, SEEK_SET);

	return file_len;
}

/***************************************************************************
函数名称：
功    能：
输入参数：
返 回 值：
说    明：此函数允许放到 common_cfgfile_tools.c 中做为内部工具函数
***************************************************************************/
static int file_resize(FILE *fp, int newsize)
{
	int now_pos = ftell(fp);
	/* 如果大小是负数则直接返回-1 */
	if (newsize < 0)
		return -1;

	_chsize(_fileno(fp), newsize);

	/* 如果当前文件指针超过了文件大小，则rewind */
	if (now_pos > newsize)
		rewind(fp);

	return 0;
}