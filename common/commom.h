/*1751533 ¼Æ3 ËÎ×Ó·«*/
char Menu(char *content[], int xiangshu, char char_select[], char char_exit);
void Recursive_MARK_S(int(*p)[10], char(*mark)[10], int hang, int lie, int HANG, int LIE);
int judge_FALSE(int(*p)[10], int hang, int lie);
void Array_MOVE(int(*p)[10], int move_mode, int hang, int lie);
void array_output(int(*p)[10], char(*mark)[10], int hang, int lie, int output_MODE, char *sentence);
void Status_bar_TOP(int tempscore, int *allscore, int target);
void Border_PRINT(int hang, int lie, int line_NUM, int Solid_Hollow, int Divider,int Extra_RIGHT);
void SingleBlock(int G, int K, int X, int Y, int bg, int fg, int NUM, char *CH, int Solid_Hollow);
void SingleBlock_MOVE(int(*p)[10], int hang, int lie, int move_mode, int G, int K, int NUM,char *CH, int Solid_Hollow, int H, int L);
void input(int *phang, int *plie, int hang_MIN, int hang_MAX, int lie_MIN, int lie_MAX);
void keyboard_mouse_MOVE(int(*p)[10], char(*mark)[10], int hang, int lie, char select, int &H, int &L, int &allscore, int &target, int &flag_continue,int extra);
void keyboard_mouse_SELECT(int(*p)[10], char(*mark)[10], int hang, int lie, char select, int &H, int &L, int &allscore, int &target, int &flag_continue,int extra);
void getEND(void);
void redrew(int(*p)[10], char(*mark)[10], int hang, int lie,int extra);
void MARK_CLEAR(char(*mark)[10], int hang, int lie);
void Status_bar_BOTTLE(int(*p)[10], int H, int L, int print_mode, int X, int Y, char*ch, int remain, int extra_score, int allscore);
