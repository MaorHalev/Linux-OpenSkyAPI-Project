#ifndef OPENSKYAPI_PROCESSCOMUNICATION_H
#define OPENSKYAPI_PROCESSCOMUNICATION_H
#include "src/childProcessLogic.h"

#define READ_END  0
#define WRITE_END  1
#define DELIMITER '\0'

extern pid_t pid;  // Declare a global variable to store the child process PID
void passInstructionsToChild(int opCode, vector<string>& params, int* infd,int* outfd);
int getInstructionFromParent(int* pipefd,vector<string>& params);
vector<string> splitString(const string& str);
void executeParentCommand(int opCode,vector<string>& params,DB& db);
void cleanup(int* infd, int* outfd);
void collectAndPrintResults(int* outfd);
void gracefullExit(int* infd,int* outfd);
void handleChildSIGUSR1AndSIGINT(int signal);
void handleSIGINTParent(int signal);

#endif //OPENSKYAPI_PROCESSCOMUNICATION_H
