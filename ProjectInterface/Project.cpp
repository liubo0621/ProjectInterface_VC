#include "Project.h"
#include <fstream>
#include <stdlib.h>
#include <windows.h>
#include <io.h>
using namespace pj;

static Project* project = nullptr;
Project::Project() {
	//ȡ����id TODO
	_processPid = 1;
	// ��ȡini�����ļ� TODO
	_readCommandTime = 1000;
	_projectNamme = "����";
	_statusPath = "D:\\process\\1.txt";
	_commandPath = "D:\\process\\2.txt";

	//����·��
	string path = string(_statusPath).substr(0, string(_statusPath).rfind("\\"));
	if (_access(path.c_str(), 0) == -1) {// ����-1 ˵���ļ��в�����
		string cmd = "md " + path;
		system(cmd.c_str());
	}

	// �������ļ����߳�
	thread t1(&Project::readCommand, this);
	//t1.join();
	t1.detach();
};
Project::~Project() {
	delete project;
}

Project *Project::getInstance() {
	if (project == nullptr){
		project = new Project();
	}
	return project;
}

bool Project::threadStarted() {
	mutex.lock();
	bool flag = false;
	if (_threadNum <= _maxThreadNum || _maxThreadNum == 0){
		_threadNum++;
		flag = true;
	}
	mutex.unlock();

	return flag;
}
void Project::threadClosed() {
	mutex.lock();
	_threadNum--;
	mutex.unlock();
}
void Project::exception(const char* exeptionMsg) {
	bool isException = true;
	int taskId = 0;
	const char* taskName = NULL;
	int taskLength = 0;
	TaskStatus taskStatus = OTHER;
	int taskDoneNum = _taskDoneNum;

	write(isException, taskId, taskName, taskLength, taskStatus, taskDoneNum, exeptionMsg);
}
void Project::writeTaskMsg(int taskId, const char* taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, const char* exeptionMsg) {
	bool isException = false;
	_taskDoneNum = taskDoneNum;

	write(isException, taskId, taskName, taskLength, taskStatus, taskDoneNum, exeptionMsg);
}
void Project::write(bool isException, int taskId, const char* taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, const char* exceptionMsg) {
	mutex.lock();

	//��ʽ��ʱ��
	time_t mytime;
	struct tm *local;
	time(&mytime);
	local = localtime(&mytime);
	char szTime[50] = { 0 };
	sprintf(szTime, "%04d-%02d-%02d %02d:%02d:%02d", (1900 + local->tm_year), (local->tm_mon + 1), local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

	const char* time = szTime;
	char * isExc = isException ? "true" : "false";
	int threadId = 1;

	char *msg = new char[1024];
	memset(msg, '\0', 1024);
	sprintf(msg, "<process_id=%d,write_file_time=%s,process_name=%s,exception=%s,thread_id=%d,thread_num=%d,task_id=%d,task_name=%s,task_length=%d,task_status=%d,task_done_num=%d,exception_msg=%s/>\n",
		_processPid, time, _projectNamme, isExc, threadId, _threadNum, taskId, taskName == nullptr ? "null" : taskName, taskLength, taskStatus, taskDoneNum, exceptionMsg == nullptr ? "null" : exceptionMsg);

	if (_isDebug) {
		printf("write: %s\n", msg);
	}

	//write
	FILE* statusFile = fopen(_statusPath, "a");
	if (statusFile == nullptr) {
		printf("write project's status error");
		mutex.unlock();
		return;
	}
	fwrite(msg, 1, strlen(msg), statusFile);
	fclose(statusFile);
	mutex.unlock();
}
void Project::readCommand() {
	while (true)
	{
		ifstream commandFile(_commandPath);
		if (!commandFile) {
			Sleep(_readCommandTime);
			continue;
		}
		string command = "";

		const int bufferSize = 256;
		char * buffer = new char[bufferSize + 1];
		while (!commandFile.eof()) {
			memset(buffer, '\0', bufferSize);
			commandFile.getline(buffer, bufferSize);
			command += string(buffer);
		}
		commandFile.close();
		delete buffer;
		//����ɾ���ļ�
		remove(_commandPath);

		if (_isDebug){
			printf("read: %s\n", command.c_str());
		}

		dealCommand(command);

		//sleep TODO
		Sleep(_readCommandTime);
	}
}

//	TASK:STOP taskId,threadId
//	THRead:MAX:NUM threadNum
void Project::dealCommand(string command) {

}

void Project::isDebug(bool isDebug) {
	this->_isDebug = isDebug;
}
void Project::onReceivedStopTaskCommand(StopTask stopTask) {
	this->_stopTask = stopTask;
}