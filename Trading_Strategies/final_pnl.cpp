#include <iostream>
#include <fstream>   
#include <sstream>      
#include <string>
#include <vector>
#include <map>
#include <vector>
#include<math.h>
#include <algorithm>
using namespace std;


static void write_pnl(const string& filename,double data) {
    ofstream file(filename);

    // Check if the file is open
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    else{
        file<<data<<endl;
    }
     

    // Close the file
    file.close();
}