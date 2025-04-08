#ifndef File_Z
#define File_Z

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime>
#include <mutex>
#include <sstream>

#include <functional>
#include <conio.h>
#include <limits>

using namespace std;



namespace TwoCli {

    // Base class for records (renamed for clarity)
    class stbase {
    public:
        virtual ~stbase() = default; // Default destructor for proper cleanup
        virtual string toString() const = 0; // Pure virtual function for string conversion

        // Get current timestamp
        static string getCurrentTimeMark() {
            auto nowTime = chrono::system_clock::now();
            time_t currentTime = chrono::system_clock::to_time_t(nowTime);

            char buffer[26]; // Need 26 chars for proper date/time format

#ifdef _WIN32
            ctime_s(buffer, sizeof(buffer), &currentTime);
#else
            ctime_r(&currentTime, buffer);
#endif

            string timestamp(buffer);
            if (!timestamp.empty() && timestamp[timestamp.length() - 1] == '\n') {
                timestamp.pop_back(); // Remove newline character
            }
            return timestamp;
        }
    };

    // Forward declaration
    template<typename R>
    class FileManager;

    // File manager implementation
    template<typename R>
    class FileManager {
    private:
        string filename;
        mutex file_mutex; // Fixed variable name

        // Function to clear the screen
        static void clearScreen() {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        }

    public:
        FileManager(const string& file) : filename(file) {
            // Create empty file if it doesn't exist
            ifstream checkFile(filename);
            if (!checkFile.good()) {
                ofstream newFile(filename);
                newFile.close();
            }
        }

        // Read all records from file
        vector<R> readRecords() {
            lock_guard<mutex> lock(file_mutex);
            vector<R> records; //  using R instead of T

            ifstream file(filename);
            if (file.is_open()) {
                string line;
                while (getline(file, line)) {
                    if (!line.empty()) {
                        try {
                            records.push_back(R::fromString(line));
                        }
                        catch (...) {
                            cerr << "Warning: Skipped invalid line in file" << endl;
                        }
                    }
                }
                file.close();
            }

            return records;
        }

        // Write records to file
        bool writeRecords(const vector<R>& records) {
            lock_guard<mutex> lock(file_mutex);

            ofstream file(filename);
            if (file.is_open()) {
                for (const auto& record : records) {
                    file << record.toString() << endl;
                }
                file.close();
                return true;
            }
            else {
                cerr << "Error: Could not open file for writing: " << filename << endl;
                return false;
            }
        }

        // Add a single record
        bool addRecord(const R& record) {
            auto records = readRecords();
            records.push_back(record);
            return writeRecords(records);
        }

        // Get next available ID for new records
        int getNextId() {
            auto records = readRecords();
            if (records.empty()) {
                return 1;
            }

            int maxId = 0;
            for (const auto& record : records) {
                if (record.id > maxId) {
                    maxId = record.id;
                }
            }
            return maxId + 1;
        }

        // Create a live monitor that watches for changes
        void startMonitor(
            function<void(const vector<R>&)> displayFunc,
            function<bool(const vector<R>&, const vector<R>&)> hasChangesFunc,
            int refreshInterval = 3 // this is time to make it longer or shorter 
        ) {
            clearScreen();
            cout << "Starting live monitoring. Screen will refresh every "
                << refreshInterval << " seconds when changes detected." << endl;
            cout << "Press Enter to return to menu." << endl;

            vector<R> lastRecords;
            string lastDisplayTimestamp;
            bool needsRefresh = true;  // Force initial display
            bool running = true;

            // Ensure input buffer is clear
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            while (running) {
                // Read current records
                vector<R> currentRecords = readRecords();
                string currentTimestamp = stbase::getCurrentTimeMark(); 

                // Check for changes (using custom function or default size check)
                bool hasChanges = needsRefresh ||
                    hasChangesFunc(currentRecords, lastRecords);

                if (hasChanges) {
                    clearScreen();
                    cout << "===== LIVE MONITOR =====" << endl;
                    cout << "Refreshing every " << refreshInterval << " seconds when changes detected" << endl;
                    cout << "Last updated: " << currentTimestamp << endl;
                    cout << "(Press Enter to return to menu)" << endl << endl;

                    if (lastRecords.size() < currentRecords.size() && !lastRecords.empty()) {
                        cout << "*** NEW ORDER RECEIVED! ***" << endl << endl;
                    }

                    // Call the display function provided by the user
                    displayFunc(currentRecords);

                    // Update last known state
                    lastRecords = currentRecords;
                    lastDisplayTimestamp = currentTimestamp;
                    needsRefresh = false;
                }

                // More efficient polling with longer sleep intervals
                auto startTime = chrono::steady_clock::now();
                while (chrono::duration_cast<chrono::seconds>(
                    chrono::steady_clock::now() - startTime).count() < refreshInterval) {
                    if (_kbhit()) {
                        char ch = _getch();
                        if (ch == '\r' || ch == '\n') {
                            running = false;
                            break;
                        }
                    }
                    // Sleep between checks
                    this_thread::sleep_for(chrono::milliseconds(250));
                }
            }

            cout << "Exiting monitor mode..." << endl;
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    };

} // namespace TwoCli

#endif // File_Z