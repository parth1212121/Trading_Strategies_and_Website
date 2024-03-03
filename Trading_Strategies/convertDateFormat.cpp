#include <iostream>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

static string convertDateFormat(string& inputDate) {

    stringstream inputStream(inputDate);
    stringstream outputStream;

    int year, month, day;

    inputStream >> year;       
    inputStream.ignore(1, '-');   
    inputStream >> month;    
    inputStream.ignore(1, '-'); 
    inputStream >> day;          


    outputStream << std::setfill('0') << std::setw(2) << day << '/' 
                 << std::setw(2) << month << '/' << year;

    return outputStream.str();
}