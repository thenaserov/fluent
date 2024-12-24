#include <iostream>  
#include <fstream>  
#include <chrono>  
#include <ctime>  
#include <thread>  
#include <iomanip>  
#include <random>  // Include the random library  

int main() {  
    std::ofstream logFile("fluentLog.txt", std::ios::app); // Open the file in append mode  
    if (!logFile) {  
        std::cerr << "Error opening file!" << std::endl;  
        return 1;  
    }  

    // Create a random number generator  
    std::random_device rd;  // Obtain a random number from hardware  
    std::mt19937 eng(rd()); // Seed the generator  
    std::uniform_int_distribution<> voltageDist(0, 5000); // Voltage range from 0 to 5000  
    std::uniform_int_distribution<> currentDist(0, 100);   // Current range from 0 to 100  

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

        // Generate random voltage and current  
        int voltage = voltageDist(eng); // Random voltage  
        int current = currentDist(eng);  // Random current  

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
