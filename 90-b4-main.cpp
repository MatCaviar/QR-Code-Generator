//1751533 ��3 ���ӷ�
#include"90-b4.h"

int main()
{
	char content[8192] = { 0 };

	cout << "�������ά�����ݣ�" << endl;
	cin.getline(content,8192);
	if (strlen(content) > 150)
	{
		cout << "�������ݹ��󣬶�ά������ʧ�ܣ�" << endl;
		return 0;
	}
	QRcode(content);

	setcolor(3, 15);
	cout << "��ά�����ɳɹ���" << endl;

	return 0;
}
