#include "containerComunication.h"


void passInstructionsToChild(int opCode, vector<string>& params,int outfd)
{
    string instruction = to_string(opCode);
    for (const string& param : params)
    {//instructions to child shall contain opcade + params.
        instruction += ' ' + param;
    }

    ssize_t bytesWritten = write(outfd, instruction.c_str(), instruction.size());
    if (bytesWritten == -1 && errno != EPIPE)
    {//if faild to write to the pipe
        perror("write");
        throw runtime_error("");
    }
    else if ((bytesWritten == -1 && errno == EPIPE) || bytesWritten == 0)
    {
        cout << "No one is listening on the other side of the pipe , returning to menu until the service will resume." << endl;
    }
    params.clear();
}

int getInstructionFromParent(int infd,vector<string>& params)
{
    int opCode;
    char buffer[BUFFER_SIZE];

    //read parent pipe data
    ssize_t bytesRead = read(infd, buffer, BUFFER_SIZE);
    if (bytesRead == -1 && errno != EPIPE)
    {
        perror("read");
        throw runtime_error("");
    }
    else if ((bytesRead == -1 && errno == EPIPE) || bytesRead == 0)
    {
        return WAIT_FOR_OPCODE;
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
            rerunScript(params);
            LoadDB(db);
            break;
        case 5:
            zipDB();
            break;
        case 6:
            cout << "need to implement shut down " << endl;
            break;
        default://there was a problem in the pipe - parent checks this condition before.
            throw runtime_error("Child got an unwanted operation code.");
    }
}


void collectAndPrintResults(int infd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;
    string outputString;

    while ((bytesRead = read(infd, buffer, BUFFER_SIZE-1)) > 0)
    {
        if (bytesRead == -1 && errno != EPIPE)
        {
            perror("read");
            throw runtime_error("");
        }
        else if (bytesRead == -1 && errno == EPIPE)
        {
            cout << "No one is listening on the other side of the pipe , returning to menu until the service will resume." << endl;
            return;
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


