#include "utility.h"
#define EXIT 7
#define READ_END  0
#define WRITE_END  1
const int BUFFER_SIZE = 100002;

int printMenu();
int printInstructionsAndGetInput(vector<string>& params);
void getInput(vector<string>& params);
void passInstructionsToChild(int opCode, const vector<string>& params, int pipefd[]);
int getInstructionFromParent(int* pipefd,vector<string>& params);
vector<string> splitString(const string& str);
void executeParentCommand(int opCode,vector<string>& params,DB db);
void cleanup(int* infd, int* outfd);
void collectAndPrintResults(int* outfd);

int main ()
{
    int infd[2] = {0, 0};
    int outfd[2] = {0, 0};

    vector<string> params;
    int opCode = 0;
    DB db;
    pid_t pid ;
    LoadDB(db);
    try
    {
        // Create the pipes
        if (pipe(infd) == -1 || pipe(outfd) == -1) {
            perror("pipe");
            throw runtime_error("");
        }

        // Fork a child process
        pid = fork();

        if (pid < 0) {
            perror("fork");
            throw runtime_error("fork error");
        }

        if (pid == 0)
        { // Child process
            dup2(infd[READ_END], STDIN_FILENO);
            dup2(outfd[WRITE_END], STDOUT_FILENO);

            close(infd[WRITE_END]); 	// Child does not write to stdin
            close(outfd[READ_END]); 	// Child does not read from stdout

            while (opCode != EXIT)
            {
                opCode = getInstructionFromParent(infd, params);
                executeParentCommand(opCode, params, db);
            }
        }
        else
        {// Parent process
            close(infd[READ_END]); 	    // Parent does not read from stdin
            close(outfd[WRITE_END]);	// Parent does not write to stdout
            // Write to parent-to-child pipe
            while (opCode != EXIT)
            {
                opCode = printInstructionsAndGetInput(params);
                passInstructionsToChild(opCode, params, infd);
                collectAndPrintResults(outfd);
            }
        }
    }
    catch(const invalid_argument& e)
    {//an exception that could not use errno.


            cout << e.what() << endl;
    }
    catch(const exception& e)
    {

    }
    cleanup(infd,outfd);
    return 0;
}

int printMenu()
{
    const char* message =   "Which method would you like to run?\n"
                            "1.Print arrivels of an airport.\n"
                            "2.Print flights by time\n"
                            "3.Print flights by aircraft.\n"
                            "4.Update the database.\n "
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

int printInstructionsAndGetInput(vector<string>& params)
{
    int choice = printMenu();
    switch(choice)
    {
        case 1:
        case 2:
            cout << "Please enter the ICAO of needed airports." << endl;
            getInput(params);
            break;
        case 3:
            cout << "Please enter the aircraft names of needed aircrafts." << endl;
            getInput(params);
            break;
        case 4:
            cout << "Updating the existing airports in the DB." << endl;
            break;
        case 5:
            cout << "Zipping the DB files." << endl;
            break;
        case 6:
            cout << "Child process PID is: ";
            ////////////////////////////////getChildProcess pid from pipe
            break;
        case 7:
            cout << "Gracefull exiting." << endl;
            break;
    }
    return choice;
}

void getInput(vector<string>& params) 
{
    string input;
    getline(cin, input);

    istringstream iss(input);
    string token;
    while (getline(iss, token, ' ')) 
    {
        params.push_back(token);
    }
}

void passInstructionsToChild(int opCode, const vector<string>& params, int pipefd[]) 
{
    string instruction = to_string(opCode);
    for (const string& param : params) 
    {
        instruction += ' ' + param;
    }

    ssize_t bytesWritten = write(pipefd[WRITE_END], instruction.c_str(), instruction.size());
    if (bytesWritten == -1) 
    {
        perror("write");
        throw runtime_error("");
    }
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

void executeParentCommand(int opCode,vector<string>& params,DB db)
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
            cout << getpid();
            break;
        case 7:
            //zip & exit.
            break;
    }
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
    bytesRead = read(outfd[READ_END], buffer, sizeof(buffer));
    if (bytesRead == -1)
    {
        perror("read");
        throw runtime_error("");
    }
    string output(buffer,bytesRead);
    cout << output << endl;
}

