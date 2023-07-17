#include "containerComunication.h"
#define WAIT_FOR_OPCODE -2
#define SHUTDOWN 6

int main ()
{
    const char* instructionPipe = "/tmp/flights_pipe/instructionPipe";
    const char* resultPipe = "/tmp/flights_pipe/resultPipe";
    int infd , outfd;

    try
    {
       if ((mkfifo(instructionPipe, 0666) || mkfifo(resultPipe, 0666)) && errno != EEXIST)
       {
           perror("named pipe creation");
           throw runtime_error("");
       }

        int infd = open(instructionPipe, O_RDONLY);
        int outfd = open(resultPipe, O_WRONLY);
        if(outfd == -1 || infd == -1) //error while opening the pipe
        {
            perror("pipe");
            throw runtime_error("");
        }

        vector<string> params;
        int opCode = WAIT_FOR_OPCODE;

        DB db;
        unzipDB();
        LoadDB(db);

        // Redirect cout to the ostringstream
        ostringstream oss;
        cout.rdbuf(oss.rdbuf());

        while (true)
        {
            opCode = getInstructionFromParent(infd, params);
            if (opCode == SHUTDOWN)
            {
                break;
            }
            executeParentCommand(opCode, params, db);
            string outputString = oss.str();  // Get the string from the ostringstream
            outputString += DELIMITER;
            int writeBytes = write(outfd, outputString.c_str(), outputString.size()); // Write the child outputString to the pipe
            if (writeBytes == -1 && errno != EPIPE)
            {
                perror("");
                throw runtime_error("");
            }
            oss.str("");
        }
    }
    catch(const exception& e)
    {
        perror("");
        cout << e.what() << endl;
        zipDB();
        close(outfd);
        close(infd);
        unlink(instructionPipe);
        unlink(resultPipe);
    }

    //needs to handel container logic shut down

    zipDB();
    close(outfd);
    close(infd);
    unlink(instructionPipe);
    unlink(resultPipe);
    return 0;
}