#include "processComunication.h"

void passInstructionsToChild(int opCode, vector<string>& params, int outfd)
{
    string instruction = to_string(opCode);
    for (const string& param : params)
    {//instructions to child shall contain opcade + params.
        instruction += ' ' + param;
    }

    ssize_t bytesWritten = write(outfd, instruction.c_str(), instruction.size());
    if (bytesWritten == -1)
    {//if faild to write to the pipe
        perror("write");
        throw runtime_error("");
    }
    params.clear();
}

void collectAndPrintResults(int infd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    string outputString;

    while ((bytesRead = read(infd, buffer, BUFFER_SIZE-1)) > 0)
    {
        if (bytesRead == -1)
        {
            perror("read");
            throw runtime_error("");
        }
        outputString += buffer;
        // Check if the delimiter is received
        if (buffer[bytesRead - 1] == DELIMITER)
        {
            break;
        }
    }
    write(STDOUT_FILENO,outputString.c_str(),outputString.size());
}
