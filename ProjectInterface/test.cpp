#include <iostream>
#include <stdio.h>
#include "Project.h"
#include "direct.h"
#include "io.h"
#include <stdlib.h>
#include <string>
#include<windows.h>
#include "INIParser.h"
using namespace std;
using namespace pj;

int main(int argc, char **argv)
{
	Project::getInstance()->isDebug(true);
	Project::getInstance()->exception("����");
	Project::getInstance()->writeTaskMsg(1, "�ɼ�", 12, DOING, 20, nullptr);




	getchar();
	return 0;
}