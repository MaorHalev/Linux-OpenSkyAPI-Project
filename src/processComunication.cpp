#include "processComunication.h"

void passInstructionsToChild(int opCode, vector<string>& params, int* infd,int* outfd)
{
    if(opCode == 7)
    {//if user want to exit - send signal.
        gracefullExit(infd,outfd);
    }

    string instruction = to_string(opCode);
    for (const string& param : params)
    {//instructions to child shall contain opcade + params.
        instruction += ' ' + param;
    }

    ssize_t bytesWritten = write(infd[WRITE_END], instruction.c_str(), instruction.size());
    if (bytesWritten == -1)
    {//if faild to write to the pipe
        perror("write");
        throw runtime_error("");
    }
    params.clear();
}

int getInstructionFromParent(int* pipefd,vector<string>& params)
{
    int opCode;
    char buffer[BUFFER_SIZE];

    //read parent pipe data
    ssize_t bytesRead = read(pipefd[0], buffer, BUFFER_SIZE);
    if (bytesRead == -1)
    {
        perror("read");
        throw runtime_error("");
    }
    string message(buffer, bytesRead);

    //get parameters from parent massege in a string vector format first place is opCode.
    params = splitString(message);
    //get opCode and erase it from params
    opCode = stoi(params[0]);
    params.erase(params.begin());
    return opCode;
}

vector<string> splitString(const string& str)
{
    vector<string> substrings;
    size_t start = 0;
    size_t end = str.find(' ');

    while (end != string::npos)
    {
        string substring = str.substr(start, end - start);
        substrings.push_back(substring);
        start = end + 1;
        end = str.find(' ', start);
    }

    // Add the last substring after the last delimiter
    string lastSubstring = str.substr(start);
    substrings.push_back(lastSubstring);
    return substrings;
}

void executeParentCommand(int opCode,vector<string>& params,DB& db)
{
    switch(opCode)
    {
        case 1:
            printAllArrivals(db, params);
            break;
        case 2:
            printAllFlightsByTime(db,params);
            break;
        case 3:
            printAllFlightsByAircraft(db,params);
            break;
        case 4:
            rerunScript(db);
            LoadDB(db);
            break;
        case 5:
            zipDB();
            break;
        case 6: //return child proccess pid
            cout << "Child pid is: " << getpid() << endl;
            break;
        default://there was a problem in the pipe - parent checks this condition before.
            throw runtime_error("Child got an unwanted operation code.");
    }
}

void gracefullExit(int* infd,int* outfd)
{
    // Send SIGUSR1 signal to the child process
    if (kill(pid, SIGUSR1) == -1)
    {
        throw runtime_error("Failed to send SIGUSR1 signal.");
    }
    // Wait for the child process to terminate
    int status;
    waitpid(pid, &status, 0);
    cleanup(infd,outfd);
    exit(0);
}

void cleanup(int* infd, int* outfd)
{
    close(infd[READ_END]);
    close(infd[WRITE_END]);

    close(outfd[READ_END]);
    close(outfd[WRITE_END]);
}

void collectAndPrintResults(int* outfd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    string outputString;

    while ((bytesRead = read(outfd[READ_END], buffer, BUFFER_SIZE-1)) > 0)
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

void handleChildSIGUSR1AndSIGINT(int signal)
{
    zipDB();
    exit(0);
}

void handleSIGINTParent(int signal)
{
    write(STDOUT_FILENO, "Gracefull exiting.\n", 20);
    // Send SIGTERM signal to the child process
    if (kill(pid, SIGINT) == -1)
    {
        throw runtime_error("Failed to send SIGTERM signal.");
    }
    // Wait for the child process to terminate
    int status;
    waitpid(pid, &status, 0);
    exit(0);
}

