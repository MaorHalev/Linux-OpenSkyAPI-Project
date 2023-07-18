
#include "utility.h"

flight::flight(string &flightStr)
{
    vector<string> flightParts = splitFlightCsvStr(flightStr);//parse the csv
    this->flightStr = flightStr;
    icao24 = flightParts[0];
    callSign = flightParts[5];
    depTimeEpoch = stoi(flightParts[1]);
    arrivalTimeEpoch = stoi(flightParts[3]);
    arriveFrom = flightParts[2];
    departureTo = flightParts[4];
    time_t epoch = arrivalTimeEpoch;
    struct tm* timeinfo = localtime(&epoch);
    strftime(formatedArrivalTime, 80,"%c", timeinfo);//get formatted time (bonus)
    epoch = depTimeEpoch;
    timeinfo = localtime(&epoch);
    strftime(formatedDepTime, 80,"%c", timeinfo);
}

airport::airport(string airportName)
{
    this->airportName = airportName;
}

void airport::getFile(string path, bool isArrivals)
{
    string line;
    ifstream infile(path);
    if (infile)//while not end of file
    {
        getline(infile, line); // skip first line - description.
        if (isArrivals)//if arrival flight
        {
            while (infile && getline(infile, line))
            {
                arrFlights.push_back(line);
            }
        }
        else//departure flight
        {
            while (infile && getline(infile, line))
            {
                depFlights.push_back(line);
            }
        }
    }
}

void LoadDB(DB &db)
{
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB"; // Get into the db folder

    if (!filesystem::exists(directoryPath))
    {
        throw runtime_error("Please run the script first to create a database.");
    }

    set<string> existingAirports;

    for (const auto &dirEntry : std::filesystem::directory_iterator(directoryPath)) // For each folder of airport
    {
        if (dirEntry.is_directory())
        {
            existingAirports.insert(dirEntry.path().stem().string());

            airport airport(dirEntry.path().stem().string()); // Create airport with the folder name

            for (const auto &file_entry : std::filesystem::directory_iterator(dirEntry.path())) // Get into the arrivals and departure files and get the flights
            {
                if (file_entry.is_regular_file() && file_entry.path().extension() == ".arv")
                {
                    airport.getFile(file_entry.path().string(), true);
                }
                if (file_entry.is_regular_file() && file_entry.path().extension() == ".dpt")
                {
                    airport.getFile(file_entry.path().string(), false);
                }
            }

            db.arrAirports.push_back(airport);
        }
    }

    auto it = db.arrAirports.begin();
    while (it != db.arrAirports.end())
    {
        if (existingAirports.find(it->airportName) == existingAirports.end())
        {
            it = db.arrAirports.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

airport *getAirport(DB &db, string airportName)
{//finds airport in the database
    for (airport &airport : db.arrAirports)
    {
        if (airport.airportName == airportName)
        {
            return &airport;
        }
    }
    return nullptr;//if not found return nullptr
}

vector<string> splitFlightCsvStr(const string &flightStr)
{//parses from csv to flight fields
    vector<string> fields;
    stringstream ss(flightStr);
    string field;
    while (getline(ss, field, ','))
    {
        fields.push_back(field);
    }
    return fields;
}

void rerunScript(vector<string>& params)
{
    //first we give the base name of the script
    string script = "./lastFlightsScript.sh";
    for(int i = 0; i < params.size(); i++)//concatenate inputs to string
    {
        script +=  " "  + params[i];
    }
    system(script.c_str());//run script
}

void zipDB()
{
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB";//get into the db folder
    string path = directoryPath.generic_string();
    if (filesystem::exists(path+".zip"))
    {//if zip exist - run it over.
        filesystem::remove(path+".zip");
    }
    zip_directory(path, "flightsDB.zip");
}

static bool is_dir(const string& dir)
{
    struct stat st;
    stat(dir.c_str(), &st);
    return S_ISDIR(st.st_mode);
}

static void walk_directory(const string& startdir, const string& inputdir, zip_t *zipper)
{
    // Open the input directory
    DIR *dp = opendir(inputdir.c_str());
    if (dp == nullptr)
    {
        throw runtime_error("Failed to open input directory: " + inputdir);
    }
    struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL)
    {
        if (dirp->d_name != string(".") && dirp->d_name != string(".."))
        {// Ignore current directory and parent directory entries
            string fullname = inputdir + "/" + dirp->d_name;
            if (is_dir(fullname))
            {// Check if the entry is a directory
                // Add the directory to the zip
                if (zip_dir_add(zipper, fullname.substr(startdir.length() + 1).c_str(), ZIP_FL_ENC_UTF_8) < 0)
                {
                    throw runtime_error("Failed to add directory to zip: " + fullname);
                }

                // Recursively walk through the subdirectory
                walk_directory(startdir, fullname, zipper);
            }
            else
            {// The entry is a file
                // Create a zip source from the file
                zip_source_t *source = zip_source_file(zipper, fullname.c_str(), 0, 0);
                if (source == nullptr)
                {
                    throw runtime_error("Failed to add file to zip: " + fullname);
                }
                // Add the file to the zip
                if (zip_file_add(zipper, fullname.substr(startdir.length() + 1).c_str(), source, ZIP_FL_ENC_UTF_8) < 0)
                {
                    zip_source_free(source);
                    throw runtime_error("Failed to add file to zip: " + fullname);
                }
            }
        }
    }
    // Close the directory
    closedir(dp);
}

static void zip_directory(const string& inputdir, const string& output_filename)
{
    int errorp;
    // Open the output file as a zip archive
    zip_t *zipper = zip_open(output_filename.c_str(), ZIP_CREATE | ZIP_EXCL, &errorp);
    if (zipper == nullptr)
    {
        zip_error_t ziperror;
        zip_error_init_with_code(&ziperror, errorp);
        throw runtime_error("Failed to open output file " + output_filename + ": " + zip_error_strerror(&ziperror));
    }
    try
    { // Recursively walk through the input directory and add its contents to the zip archive
        walk_directory(inputdir, inputdir, zipper);
    }
    catch(...)
    {// Close the zip archive and rethrow the exception
        zip_close(zipper);
        throw runtime_error("Unable to zip due to an error while zipping.");
    }
    zip_close(zipper);
}

void unzipDB()
{
    // Get the path to the flightsDB.zip file in the current directory
    filesystem::path directoryPath = filesystem::current_path() / "flightsDB.zip";
    const char *archive;
    const char *targetDir = "flightsDB";
    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[BUFFER_SIZE];
    int i, len, fd, err;
    long long sum;

    archive = directoryPath.c_str();
    // Open the flightsDB.zip file as a zip archive
    if ((za = zip_open(archive, 0, &err)) == NULL)
    {//fails to open flightsDB.zip file.
        return;
    }

    // Create the target directory
    mkdir(targetDir, 0755);

    for (i = 0; i < zip_get_num_entries(za, 0); i++)
    {
        if (zip_stat_index(za, i, 0, &sb) == 0)
        {//succeed retrieve information about the entry.
            // Get the entry path within the target directory
            filesystem::path entryPath = filesystem::path(targetDir) / sb.name;
            string entryPathStr = entryPath.string();
            len = strlen(sb.name);
            if (sb.name[len - 1] == '/')
            {// Create the directory within the target directory
                mkdir(entryPathStr.c_str(), 0755);
            }
            else
            {//is a file that is not a directory
                // Open the zipped file for reading
                zf = zip_fopen_index(za, i, 0);
                // Open a new file for writing
                fd = open(entryPathStr.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777);
                if (fd < 0)
                {//unsuccessful opening the file
                    zip_fclose(zf);
                    throw runtime_error("Failed to open zipped file: " + entryPathStr);
                }
                sum = 0;
                // Read data from the zipped file and write it to the new file
                while (sum != sb.size)
                {//read all the data from zipped file into the new file
                    len = zip_fread(zf, buf, BUFFER_SIZE);
                    write(fd, buf, len);
                    sum += len;
                }
                close(fd);
                zip_fclose(zf);
            }
        }
    }
    // Close the zip archive
    zip_close(za);
}


