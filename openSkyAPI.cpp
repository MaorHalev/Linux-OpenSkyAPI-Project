#include "utility.h"
#define READ_END  0
#define WRITE_END  1
#define DELIMITER '\0'
const int BUFFER_SIZE = 10000;

int printMenu();
int printInstructionsAndGetInput(vector<string>& params);
void getInput(vector<string>& params);
void passInstructionsToChild(int opCode, vector<string>& params, int pipefd[]);
int getInstructionFromParent(int* pipefd,vector<string>& params);
vector<string> splitString(const string& str);
void executeParentCommand(int opCode,vector<string>& params,DB& db);
void cleanup(int* infd, int* outfd);
void collectAndPrintResults(int* outfd);
void zipDB();
static bool is_dir(const string& dir);
static void walk_directory(const string& startdir, const string& inputdir, zip_t *zipper);
static void zip_directory(const string& inputdir, const string& output_filename);
void gracefullExit();
void handleSIGUSR1AndSIGINT(int signal);
void handleSIGTERMParent(int signal);

pid_t pid;  // Declare a global variable to store the child process PID
int main ()
{
    int infd[2] = {0, 0};
    int outfd[2] = {0, 0};

    vector<string> params;
    int opCode = 0;
    try
    {
        // Create the pipes
        if (pipe(infd) == -1 || pipe(outfd) == -1)
        {
            perror("pipe");
            throw runtime_error("");
        }

        // Fork a child process
        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            throw runtime_error("fork error");
        }

        if (pid == 0)
        { // Child process
            DB db;
            LoadDB(db);
            signal(SIGUSR1,handleSIGUSR1AndSIGINT);
            signal(SIGINT,handleSIGUSR1AndSIGINT);
            ostringstream oss;
            // Redirect cout to the ostringstream
            cout.rdbuf(oss.rdbuf());

            close(infd[WRITE_END]); 	// Child does not write to stdin
            close(outfd[READ_END]); 	// Child does not read from stdout

            while (true)
            {
                opCode = getInstructionFromParent(infd, params);
                executeParentCommand(opCode, params, db);

                // Get the string from the ostringstream
                string outputString = "";
                outputString = oss.str();
                outputString += DELIMITER;

                // Write the child outputString to the pipe
                write(outfd[WRITE_END], outputString.c_str(), outputString.size());
                oss.str("");
            }
        }
        else
        {// Parent process
            signal(SIGINT,handleSIGTERMParent);
            close(infd[READ_END]); 	    // Parent does not read from stdin
            close(outfd[WRITE_END]);	// Parent does not write to stdout
            // Write to parent-to-child pipe
            while (true)
            {
                opCode = printInstructionsAndGetInput(params);
                passInstructionsToChild(opCode, params, infd);
                collectAndPrintResults(outfd);
            }

        }
    }
    catch(const exception& e)
    {
        cout << e.what() << endl;
    }
    catch(...)
    {

    }
    cleanup(infd,outfd);
    return 0;
}

void handleSIGUSR1AndSIGINT(int signal)
{
    zipDB();
    exit(0);
}
void handleSIGTERMParent(int signal)
{
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

int printMenu()
{
    const char* message =   "Which method would you like to run?\n"
                            "1.Print arrivels of an airport.\n"
                            "2.Print flights by time\n"
                            "3.Print flights by aircraft.\n"
                            "4.Update the database.\n"
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
            break;
        case 7:
            cout << "Gracefull exiting." << endl;
            break;
        default:
            throw invalid_argument("Invalid choice");
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

void passInstructionsToChild(int opCode, vector<string>& params, int pipefd[])
{
    if(opCode == 7)
    {
        gracefullExit();
    }

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

void executeParentCommand(int opCode,vector<string>& params,DB& db)
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
            zipDB();
            break;
        case 6: //return child proccess pid
            cout << "Child pid is: " << getpid() << endl;
            break;
        default:
            throw runtime_error("Child got an unwanted operation code.");
    }
}

void gracefullExit()
{
    // Send SIGUSR1 signal to the child process
    if (kill(pid, SIGUSR1) == -1)
    {
        throw runtime_error("Failed to send SIGUSR1 signal.");
    }
    // Wait for the child process to terminate
    int status;
    waitpid(pid, &status, 0);
    exit(0);
}

void zipDB()
{
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB";//get into the db folder
    string path = directoryPath.generic_string();
    if (filesystem::exists(path+".zip"))
    {
        filesystem::remove(path+".zip");
    }
    zip_directory(path, "flightsDB.zip");
}

static bool is_dir(const string& dir)
{
    struct stat st;
    ::stat(dir.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

static void walk_directory(const string& startdir, const string& inputdir, zip_t *zipper)
{
    DIR *dp = opendir(inputdir.c_str());
    if (dp == nullptr)
    {
        throw runtime_error("Failed to open input directory: ");
    }

    struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name != std::string(".") && dirp->d_name != std::string(".."))
        {
            std::string fullname = inputdir + "/" + dirp->d_name;
            if (is_dir(fullname))
            {
                if (zip_dir_add(zipper, fullname.substr(startdir.length() + 1).c_str(), ZIP_FL_ENC_UTF_8) < 0)
                {
                    throw runtime_error("Failed to add directory to zip: ");
                }
                walk_directory(startdir, fullname, zipper);
            }
            else
            {
                zip_source_t *source = zip_source_file(zipper, fullname.c_str(), 0, 0);
                if (source == nullptr)
                {
                    throw runtime_error("Failed to add file to zip: ");
                }
                if (zip_file_add(zipper, fullname.substr(startdir.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
                {
                    zip_source_free(source);
                    throw runtime_error("Failed to add file to zip: ");
                }
            }
        }
    }
    closedir(dp);
}

static void zip_directory(const string& inputdir, const string& output_filename)
{
    int errorp;
    zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
    if (zipper == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }

    try
    {
        walk_directory(inputdir, inputdir, zipper);
    }
    catch(...)
    {
        zip_close(zipper);
        throw;
    }
    zip_close(zipper);
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

    while ((bytesRead = read(outfd[READ_END], buffer, BUFFER_SIZE)) > 0)
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

