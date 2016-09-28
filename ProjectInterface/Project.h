#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include <stdio.h>
using namespace std;

namespace pj {

	enum TaskStatus
	{
		DOING = 0x0000001,
		DONE = 0x0000002,
		EXCEPTION = 0x0000003,
		OTHER = 0x0000000,
	};

	typedef function<void(int, int)> StopTask;

	class Project {
	public:
		static Project *getInstance();
		bool threadStarted();
		void threadClosed();
		void exception(const char* exeptionMsg);
		void writeTaskMsg(int taskId, const char* taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, const char* exeptionMsg = nullptr);
		
		void isDebug(bool isDebug);
		void onReceivedStopTaskCommand(StopTask stopTask);

	private:
		void write(bool isException, int taskId, const char* taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, const char* exceptionMsg);
		void readCommand();
		void dealCommand(string command);

		Project();
		~Project();

	private:
		TaskStatus _taskStatus;
		StopTask _stopTask =  nullptr;
		mutex mutex;//线程互斥对象

		bool _isDebug = false;

		int _threadNum = 0;
		int _maxThreadNum = 0xfffffff;
		int _processPid;
		int _taskDoneNum = 0;
		int _readCommandTime = 0;
		
		string _projectNamme;
		string _statusPath;
		string _commandPath;
	};

}