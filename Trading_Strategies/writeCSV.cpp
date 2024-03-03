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


static void writeCSV(const string& filename, const vector<vector <string> >& data) {
    ofstream file(filename);

    // Check if the file is open
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    if(data[0].size()==2){
        vector<vector<string> > final_data;
        vector<string> temp1;
        for(int j=0;j<data[0].size();j++){
            temp1.push_back(data[0][j]);
        }
        final_data.push_back(temp1);
        vector<string> temp2;
        for(int j=0;j<data[1].size();j++){
            temp2.push_back(data[1][j]);
        }
        final_data.push_back(temp2);
        for(int i=2;i<data.size();i++){
            if(data[i][0]!=final_data[final_data.size()-1][0]){
                vector<string> temp;
                for(int j=0;j<data[i].size();j++){
                    temp.push_back(data[i][j]);
                }
                final_data.push_back(temp);
            }
            else{
                final_data.pop_back();
                vector<string> temp;
                for(int j=0;j<data[i].size();j++){
                    temp.push_back(data[i][j]);
                }
                final_data.push_back(temp);

            }
            
        }
        // Write data to the file
        for (const auto& row : final_data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                // Add comma if it's not the last element in the row
                if (i != row.size() - 1) {
                    file << ",";
                }
            }
            file << endl; // Move to the next line
        }
    }
    else {
        vector<vector<string> > final_data;
        vector<string> temp1;
        for(int j=0;j<data[0].size();j++){
            temp1.push_back(data[0][j]);
        }
        final_data.push_back(temp1);
        vector<string> temp2;
        for(int j=0;j<data[1].size();j++){
            temp2.push_back(data[1][j]);
        }
        final_data.push_back(temp2);
        int r=0;
        for(int i=2;i<data.size();i++){

            if(data[i][0]!=final_data[final_data.size()-1][0]){
                if(data[i][1]=="SELL"){
                    r=-1;
                }
                else{
                    r=1;
                }
                vector<string> temp;
                for(int j=0;j<data[i].size();j++){
                    temp.push_back(data[i][j]);
                }
                final_data.push_back(temp);
            }
            else{
                final_data.pop_back();
                if(data[i][1]=="SELL"){
                    r--;
                }
                else{
                    r++;
                }
                vector<string> temp;
                temp.push_back(data[i][0]);
                if(r>0){
                    temp.push_back("BUY");
                }
                else{
                    temp.push_back("SELL");
                }
                temp.push_back(to_string(r));
                for(int j=3;j<data[i].size();j++){
                    temp.push_back(data[i][j]);
                }
                final_data.push_back(temp);
            }
            
        }
        // Write data to the file
        for (const auto& row : final_data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                // Add comma if it's not the last element in the row
                if (i != row.size() - 1) {
                    file << ",";
                }
            }
            file << endl; // Move to the next line
        }
    }
    

    // Close the file
    file.close();
}