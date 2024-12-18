#include <iostream>  
#include <fstream>  
#include <chrono>  
#include <ctime>  
#include <thread>  
#include <iomanip>  

int main() {  
    std::ofstream logFile("fluentLog.txt", std::ios::app); // Open the file in append mode  
    if (!logFile) {  
        std::cerr << "Error opening file!" << std::endl;  
        return 1;  
    }  

    while (true) {  
        // Get current time  
        auto now = std::chrono::system_clock::now();  
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);  
        std::tm* now_tm = std::localtime(&now_c);  

        // Format date and time  
        std::ostringstream dateStream;  
        dateStream << std::put_time(now_tm, "%Y.%m.%d");  
        std::ostringstream timeStream;  
        timeStream << std::put_time(now_tm, "%H:%M");  

        // Replace these with actual voltage and current readings  
        int voltage = 3120; // Placeholder for actual voltage  
        int current = 10;   // Placeholder for actual current  

        // Write to file  
        logFile << dateStream.str() << "|"   
                 << timeStream.str() << "|"   
                 << voltage << "|"   
                 << current << std::endl;  

        // Flush the output to ensure it's written to the file  
        logFile.flush();  

        // Sleep for a specified interval (e.g., 1 second)  
        std::this_thread::sleep_for(std::chrono::seconds(1));  
    }  

    logFile.close(); // Close the file (this line will never be reached)  
    return 0;  
}