#include "containerComunication.h"
#define WAIT_FOR_OPCODE -2

int main ()
{
    const char* instructionPipe = "instructionPipe";
    const char* resultPipe = "resultPipe";

//    if (mkfifo(instructionPipe, 0777) || mkfifo(resultPipe, 0777))
//    {
//        perror("named pipe creation");
//        throw runtime_error("");
//    }

    int infd = open(instructionPipe, O_RDONLY);
    int outfd = open(resultPipe, O_WRONLY);
    if(outfd == -1 || infd == -1) //error while opening the pipe
    {
        perror("pipe");
        throw runtime_error("");
    }

    vector<string> params;
    int opCode = WAIT_FOR_OPCODE;

    try
    {
        DB db;
        unzipDB();
        LoadDB(db);

        // Redirect cout to the ostringstream
        ostringstream oss;
        cout.rdbuf(oss.rdbuf());

        while (true)
        {
            opCode = getInstructionFromParent(infd, params);
            executeParentCommand(opCode, params, db);
            string outputString = oss.str();  // Get the string from the ostringstream
            outputString += DELIMITER;
            write(outfd, outputString.c_str(), outputString.size()); // Write the child outputString to the pipe
            oss.str("");
        }
    }
    catch(const exception& e)
    {
        cout << e.what() << endl;
    }

    //needs to handel container logic shut down

    close(outfd);
    close(infd);
    return 0;
}