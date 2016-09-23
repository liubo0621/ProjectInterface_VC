#pragma once

#include <iostream>
#include <string>
#include <functional>
using namespace std;

namespace pj {

	struct TaskStatus
	{
		const int DOING = 0x0000001;
		const int	DONE = 0x0000002;
		const int EXCEPTION = 0x0000003;
		const int OTHER = 0x0000000;
	};

	typedef function<void(int, int)> StopTask;

	class Project {
	public:
		static Project *getInstance();
		bool threadStarted();
		void threadClosed();
		void exception(const char* exeptionMsg);
		void writeTaskMsg(int taskId, string taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum);
		void writeTaskMsg(int taskId, string taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, const char* exeptionMsg);
		
		void isDebug(bool isDebug) {
			this->isDebug = isDebug;
		}
		void onReceivedStopTaskCommand(StopTask stopTask) {
			this->stopTask = stopTask;
		}

	private:
		void write(bool isException, int taskId, string taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, string exceptionMsg);
		void readCommand();
		void dealCommand(string command);

		Project() {};
		~Project(){
			delete project;

		}

	private:
		StopTask stopTask;
		static Project* project;

		bool isDebug = false;
	};

}