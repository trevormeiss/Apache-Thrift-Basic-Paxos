//
//  helper.cpp
//  Server
//
//  Created by Trevor Meiss on 10/20/15.
//  Copyright © 2015 Trevor Meiss. All rights reserved.
//

#include <string>

#include <ctime>
#include <sys/time.h>
#include "helper.hpp"

// Check if character is alphanumeric
static inline bool is_not_alnum(char c)
{
    return !isalnum(c);
}
// Check if entire string is alphanumeric
bool stringIsValid(const std::string &str)
{
    return find_if(str.begin(), str.end(), is_not_alnum) == str.end();
}

// Return the System Time as DD-MM-YYY HH:MM:SS.SSS
std::string systemTime()
{
    timeval t;
    gettimeofday(&t, NULL);
    long long millis = (long long) t.tv_sec * 1000L + t.tv_usec / 1000;
    int cmillis = millis % 1000;
    
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer,80,"%d-%m-%Y %I:%M:%S",timeinfo);
    std::string str(buffer);
    str += ".";
    
    // Pad 0's to milliseconds
    if (cmillis < 100) {
        str += "0";
        if (cmillis < 10)
            str += "0";
    }
    
    str += std::to_string(cmillis);
    
    return str;
}

// Trim whitespace from the lest and right of string
std::string trim(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;
    
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;
    
    return std::string(it, rit.base());
}

// Compare two strings
bool compareStringNoCase(std::string a, int start, int length, std::string b)
{
    bool equal = true;
    int i;
    
    if (a.size() < (start + length - 1))
    {
        fprintf(stderr, "ERROR, first string in compare not long enough");
        equal = false;
    }
    if (b.size() < (length - 1))
    {
        fprintf(stderr, "ERROR, second string in compare not long enough");
        equal = false;
    }
    else
    {
        for (i = 0; i<length; i++)
        {
            if (tolower(a[i+start]) != b[i])
            {
                equal = false;
                break;
            }
        }
    }
    return equal;
}