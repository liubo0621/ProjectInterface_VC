#include <iostream>
#include <stdio.h>
#include "Project.h"
#include "direct.h"
#include "io.h"
#include <stdlib.h>
#include <string>
#include<windows.h>
using namespace std;
using namespace pj;

void check_make_path(string file_name) {

	//long result = CreateDirectory("D:\\aaa", NULL);
	string dir = file_name.substr(0, file_name.rfind("\\"));
	printf(" -- %s", dir.c_str());
	string cmd = "md " + dir;
	int result = mkdir(dir.c_str());//ֻ�ܴ���һ��Ŀ¼
	printf("%d", result);
	system(cmd.c_str());
}

int main(int argc, char **argv)
{
	Project::getInstance()->isDebug(true);
	Project::getInstance()->exception("����");
	Project::getInstance()->writeTaskMsg(1, "�ɼ�", 12, DOING, 20, nullptr);
	getchar();
	return 0;
}