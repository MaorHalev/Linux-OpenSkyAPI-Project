#ifndef OPENSKYAPI_PROCESSCOMUNICATION_H
#define OPENSKYAPI_PROCESSCOMUNICATION_H
#include "utility.h"

#define DELIMITER '\0'

void passInstructionsToChild(int opCode, vector<string>& params, int outfd);
void collectAndPrintResults(int infd);

#endif //OPENSKYAPI_PROCESSCOMUNICATION_H
