#include "utility.h"

int printMenu();
vector<string> getAirports();

int main ()
{
    DB db;
    LoadDB(db);
    try
    {
        int pipefd[2];
        if (pipe(pipefd) == -1) 
        {
            perror("pipe");
            throw runtime_error("");
        }
        pid_t pid = fork();
        if (pid == -1) 
        {
            perror("fork");
            throw runtime_error("");
        }
        while (true)
        {
            if(pid == 0)
            {//running child process
                vector<string> params;
                int opCode = getInstructionFromParent(pipefd,params);
                executeParentCommand(opCode,params,db);
            }
            else
            {//running parent process
                getInput();
            }
        }
    }
    catch(const invalid_argument& e)
    {//an exception that couldnt use errno.  
        write(STDOUT_FILENO, e.what(), strlen(e.what()));
    }
    catch(const exception& e)
    {
        
    }
    
    return 0;
}

int printMenu()
{
    const char* message = "Which method would you like to run?\n"
                           "1.Update the database.\n "
                           "2.Print flights by aircraft.\n"
                           "3.Print flights by time\n"
                           "4.Print arrivels of an airport.\n"
                           "5.Zip the database files\n"
                           "6.Print child proccess PID.\n"
                           "7.Exit.\n";

    ssize_t bytes_written = write(STDOUT_FILENO, message, strlen(message));

    if (bytes_written == -1) 
    {// Error occurred while writing
        perror("Error writing");
        throw runtime_error("Error writing");
    }
    
    char buffer[1024]; // Buffer to store the read data
    ssize_t bytesRead;

    // Read data from standard input into the buffer
    bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (bytesRead == -1) 
    {// Error occurred while reading
        perror("Error reading from input");
        throw runtime_error("Error reading from input");
    }
    buffer[bytesRead] = '\0';

    // Convert the read data to an integer
    int number = atoi(buffer);

    if(number < 1 || number > 7)
    {//invalid choice
        throw invalid_argument("Invalid choice");
    }

    return number;
}

void getInput()
{
    vector<string> airports;
    int choice = printMenu();
    switch(choice)
    {
        case 1:
            airports = getAirports();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
    }
}

vector<string> getAirports()
{

}

int getInstructionFromParent(int* pipefd,vector<string> params)
{
    close(pipefd[1]);  // Close the write end of the pipe
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = read(pipefd[0], buffer, BUFFER_SIZE);
    if (bytesRead == -1) 
    {
        perror("read");
        throw runtime_error("");
    }
    close(pipefd[0]);  // Close the read end of the pipe
    string message(buffer, bytesRead);
    params = splitString(message);
    return params.size();
}

vector<string> splitString(const string& str) 
{
    vector<string> substrings;
    size_t start = 0;
    size_t end = str.find(' ');

    while (end != string::npos) 
    {
        std::string substring = str.substr(start, end - start);
        substrings.push_back(substring);
        start = end + 1;
        end = str.find(' ', start);
    }

    // Add the last substring after the last delimiter
    string lastSubstring = str.substr(start);
    substrings.push_back(lastSubstring);
    return substrings;
}

void executeParentCommand(int opCode,vector<string> params,DB db)
{
    switch(opCode)
    {
        case 1:
            printAllArrivels(db,params);
            break;
        case 2:
            printAllFlightsByTime(db,params);
            break;
        case 3:
            printAllFlightsByAircraft(db,params);
            break;
        case 4:
            rerunScript(db);
            break;
        case 5:
            //zip
            break;
        case 6:
            //return child proccess pid
            break;
        case 7:
            //zip & exit.
            break;
    }
}
/*
int main() 
{



    else 
    {
        // Parent process
        close(pipefd[0]);  // Close the read end of the pipe

        string message = "Hello from the parent process!";
        ssize_t bytesWritten = write(pipefd[1], message.c_str(), message.size());
        if (bytesWritten == -1) 
        {
            perror("write");
            throw runtime_error("");
        }

        close(pipefd[1]);  // Close the write end of the pipe
    }

    return 0;
}*/
