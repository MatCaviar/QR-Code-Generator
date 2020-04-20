//1751533 计3 宋子帆
#include"90-b4.h"

int main()
{
	char content[8192] = { 0 };

	cout << "请输入二维码内容：" << endl;
	cin.getline(content,8192);
	if (strlen(content) > 150)
	{
		cout << "输入数据过大，二维码生成失败！" << endl;
		return 0;
	}
	QRcode(content);

	setcolor(3, 15);
	cout << "二维码生成成功！" << endl;

	return 0;
}
