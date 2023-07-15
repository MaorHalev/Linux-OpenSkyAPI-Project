#ifndef OPENSKYAPI_CONTAINERCOMUNICATION_H
#define OPENSKYAPI_CONTAINERCOMUNICATION_H
#include "dbServiceLogic.h"

#define DELIMITER '\0'

void passInstructionsToChild(int opCode, vector<string>& params,int outfd);
int getInstructionFromParent(int infd,vector<string>& params);
vector<string> splitString(const string& str);
void executeParentCommand(int opCode,vector<string>& params,DB& db);
void collectAndPrintResults(int infd);

#endif //OPENSKYAPI_CONTAINERCOMUNICATION_H
