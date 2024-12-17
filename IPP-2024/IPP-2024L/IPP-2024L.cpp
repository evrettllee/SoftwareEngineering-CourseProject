#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>

extern "C"
{
    char* DATE(void)
    {
        time_t rawtime;
        struct tm* timeinfo;
        char* buffer = (char*)calloc(20, sizeof(char));
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if (timeinfo && buffer) 
        {
            strftime(buffer, 20, "Date: %d.%m.%y", timeinfo);
        }

        return buffer;
    }

    char* TIME(void)
    {
        time_t rawtime;
        struct tm* timeinfo;
        char* buffer = (char*)calloc(20, sizeof(char));
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        if (timeinfo && buffer) 
        {
            strftime(buffer, 20, "Time: %H:%M:%S", timeinfo);
        }

        return buffer;
    }
}