#include "Project.h"
#include <fstream>
#include <stdlib.h>
#include <windows.h>
using namespace pj;

static Project* project = nullptr;
Project::Project() {
	//取进程id TODO
	_processPid = 1;
	// 读取ini配置文件 TODO
	_readCommandTime = 1000;
	_projectNamme = "测试";
	_statusPath = "D:\\process\\1.txt";
	_commandPath = "D:\\process\\2.txt";

	//创建路径
	string path = string(_statusPath).substr(0, string(_statusPath).rfind("\\"));
	string cmd = "md " + path;
	system(cmd.c_str());

	// 开启读文件的线程
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
	//格式化时间 TODO
	const char* time = "2016-07-03 12:09:12";
	char * isExc = isException ? "true" : "false";
	int threadId = 1;

	char *msg = new char[1024];
	memset(msg, '\0', 1024);
	sprintf(msg, "<process_id=%d,write_file_time=%s,process_name=%s,exception=%s,thread_id=%d,thread_num=%d,task_id=%d,task_name=%s,task_length=%d,task_status=%d,task_done_num=%d,exception_msg=%s/>\n",
		_processPid, time, _projectNamme, isExc, threadId, _threadNum, taskId, taskName == nullptr ? "null" : taskName, taskLength, taskStatus, taskDoneNum, exceptionMsg == nullptr ? "null": exceptionMsg);
	
#if _isDebug
		printf("write: %s", msg);
#endif

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
		cout << "readCommand" << endl;
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
		//读完删除文件
		remove(_commandPath);

#if _isDebug
		printf("read: %s", command.c_str());
#endif

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