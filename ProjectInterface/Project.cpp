#include "Project.h"
using namespace pj;

Project *Project::getInstance() {
	if (project == nullptr){
		project = new Project();
	}
	return project;
}
void Project::isDebug(bool isDebug) {

}
bool Project::threadStarted() {

}
void Project::threadClosed() {

}
void Project::exception(const char* exeptionMsg) {

}
void Project::writeTaskMsg(int taskId, string taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum) {

}
void Project::writeTaskMsg(int taskId, string taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, const char* exeptionMsg) {

}
void Project::write(bool isException, int taskId, string taskName, int taskLength, TaskStatus taskStatus, int taskDoneNum, string exceptionMsg) {

}
void Project::readCommand() {

}
void Project::dealCommand(string command) {

}