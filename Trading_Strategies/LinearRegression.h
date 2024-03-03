#include <iostream>     // Basically as the user entrers the arguments 2 different files for traing and the other for testing will be created 
                       // Same has been added to to Data.py.
#include <fstream>      // THe problerm for finding the latest active day has to be worked on in Data.py
#include <sstream>      // Otherwise this may cause "seg_fault".
#include <string>
#include <vector>
#include <map>
#include <vector>
#include<math.h>
#include <algorithm>

using namespace std;
void LinearRegeression( string symbol , int x , int p ,string train_start_date , string train_end_date ,string start_date , string end_date, bool write );
void Final_PnL(string symbol,int x,int p,bool write);
double Model_Prediction(int date_index,map<string,vector<string>> Train_Data,map<string,vector<string>> Test_Data);
vector<double> Normal(vector<vector<double>> X, vector<double> target);
vector<vector<double>> inverse(const vector<vector<double>>& matrix);
vector<vector<double>> multiply(const vector<vector<double>>& A, const vector<vector<double>>& B);
vector<vector<double>> transpose(const vector<vector<double>>& matrix) ;
double prediction(vector<double> training_example,vector<double> para);
vector<double> Target_vector(map<string,vector<string>> train_data);
vector<vector<double>> Training_Matrix_Rev(map<string,vector<string>> test_data);
vector<vector<double>> Training_Matrix(map<string,vector<string>> train_data);
map<string,vector<string> > parse_data_LR(string symbol,string startegy) ;
