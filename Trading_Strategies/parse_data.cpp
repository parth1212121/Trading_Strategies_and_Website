#include <iostream>
#include <fstream>   
#include <sstream>      
#include <string>
#include <vector>
#include <map>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

static map<string,vector<string> > parse_data(string symbol) {
    // Open the CSV file

    map<string, vector<string> > dataMap;

    ifstream file(symbol+".csv");
    // Check if the file is open
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return dataMap;
    }
    // Read the header line to get column names
    string header;
    getline(file, header);
    stringstream headerStream(header);
    vector<string> columnNames;
    string columnName;
    while (getline(headerStream, columnName, ',')) {
        columnNames.push_back(columnName);
    }

    string line;
    while (getline(file, line)) {
        stringstream lineStream(line);
        vector<string> rowData;
        string cell;

        // Read each cell of the line
        while (getline(lineStream, cell, ',')) {
            rowData.push_back(cell);
        }

        // Store data in the map
        for (size_t i = 0; i < columnNames.size(); ++i) {
            dataMap[columnNames[i]].push_back(rowData[i]);
        }
    }
    
    file.close();
    return dataMap;
}