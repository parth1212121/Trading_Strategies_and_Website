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

//EXTRA  FEATURES 

       // feature_1

vector<double> rolling_mean(vector<double> values) {

    int m= values.size();

    vector<double> result(values.size(), 0.0);                  // window - 10.

    for(int i=0;i<values.size()-1;i++){

            double sum =0;

            int x= min((i+10),(m-1));

            for(int j=i+1;j<=x;j++){
                    sum+=(values[j]);
            }

            sum=sum/double(x-i);

            result[i]=sum;

    }

    result[values.size()-1]=values[values.size()-1];

    return result;
}


map<string,vector<string>> track_rolling_mean(map<string,vector<string>> &data_map) {

    map<string, vector<double>> result;
    vector<double> closes ;

    for(auto i : data_map["CLOSE"]){
        closes.push_back(stod(i));
    }

    vector<double> rolling_means = rolling_mean(closes);

    vector<string>R_M;

    for(auto i : rolling_means ){
        R_M.push_back(to_string(i));
    }
 
    data_map["R_M"]=R_M;


    return data_map;
}

        // feature_2

vector<double> exponential_weighted_mean(vector<double> values, int com) {

    int m= values.size();
    vector<double> result(values.size(), 0.0);
    double alpha = 2.0 / (com + 1.0);
    result[m-1] = values[m-1]; // First value remains the same
    for (int i = m-2; i >= 0; i--) {
        result[i] = alpha * values[i] + (1 - alpha) * result[i+1];
    }
    return result;

}

map<string, vector<string>> track_ewm(map<string,vector<string>> &data_map) {

    vector<double> cl_price, daily_return, ewm_values;
    vector<string>ewm_str;

    // Convert string data to numeric
    for ( string str : data_map["CLOSE"]) {
        cl_price.push_back(stod(str));
    }

    // Calculate daily returns
    for (int i = 0; i < cl_price.size()-1; i++) {
        daily_return.push_back((cl_price[i] - cl_price[i + 1]) / cl_price[i + 1]);
    }
    daily_return.push_back(daily_return.back());


    // Calculate exponential weighted mean
    ewm_values = exponential_weighted_mean(daily_return, 10);

    // Convert results to string format
    for ( double value : ewm_values) {
        ewm_str.push_back(to_string(value));
    }

    // Populate result map

    data_map["EWM"]=ewm_str;
    return data_map;

}

//EXTRA  FEATURES CLOSED


map<string,vector<string>> parse_data_train(string symbol) {
    // Open the CSV file

    map<string, vector<string>> dataMap;

    ifstream file(symbol+"_train.csv");
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
    // Map to store data with column names as keys
    
    // Read each line of the file
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

    // Close the file
    file.close();

    return dataMap;
}

map<string,vector<string>> parse_data_test(string symbol) {
    // Open the CSV file

    map<string, vector<string>> dataMap;

    ifstream file(symbol+"_test.csv");
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
    // Map to store data with column names as keys
    
    // Read each line of the file
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

    // Close the file
    file.close();

    return dataMap;
}


vector<vector<double>> Training_Matrix(map<string,vector<string>> train_data){

    vector<vector<double>> X; // Training_Matrix

    for (int i = 0; i < train_data["CLOSE"].size()-1; i++) {

        vector<double> row;
        row.push_back(1.0); // Bias term           //.....Feature Enginnering 3.

        //  Features 

        row.push_back(stod(train_data["CLOSE"][i+1])); // Close(t-1)
        row.push_back(stod(train_data["OPEN"][i+1])); // Open(t-1)
        row.push_back(stod(train_data["VWAP"][i+1])); // VWAP(t-1)
        row.push_back(stod(train_data["LOW"][i+1])); // Low(t-1)
        row.push_back(stod(train_data["HIGH"][i+1])); // High(t-1)
        row.push_back(stod(train_data["NO OF TRADES"][i+1])); // No of Trades(t-1)

// EXTRA FEATURES.....

                                   // Features to be added...

        row.push_back(stod(train_data["R_M"][i+1])); // Rolling Mean(t-1)
        row.push_back(stod(train_data["EWM"][i+1])); // Exponential_weighing_Mean(t-1)
        

// EXTRA FEATURES CLOSED...

        row.push_back(stod(train_data["OPEN"][i])); // Open(t)
        row.push_back(stod(train_data["PREV. CLOSE"][i+1])); // PREV. CLOSE_Price(t-1)

        X.push_back(row);

    }// Close(t-1)Open(t-1)VWAP(t-1)Low(t-1)High(t-1)No of Trades(t-1)Open(t)

    return X;

}


vector<vector<double>> Training_Matrix_Rev(map<string,vector<string>> test_data){

    vector<vector<double>> X; // Training_Matrix

    for (int i = 1; i < test_data["CLOSE"].size(); i++) {

        vector<double> row;
        row.push_back(1.0); // Bias term           //.....Feature Enginnering 3.

        //  Features 

        row.push_back(stod(test_data["CLOSE"][i-1])); // Close(t-1)
        row.push_back(stod(test_data["OPEN"][i-1])); // Open(t-1)
        row.push_back(stod(test_data["VWAP"][i-1])); // VWAP(t-1)
        row.push_back(stod(test_data["LOW"][i-1])); // Low(t-1)
        row.push_back(stod(test_data["HIGH"][i-1])); // High(t-1)
        row.push_back(stod(test_data["NO OF TRADES"][i-1])); // No of Trades(t-1)

// EXTRA FEATURES.....

                                   // Features to be added...

        row.push_back(stod(test_data["R_M"][i-1])); // Rolling Mean(t-1)
        row.push_back(stod(test_data["EWM"][i-1])); // Exponential_weighing_Mean(t-1)
        

// EXTRA FEATURES CLOSED...

        row.push_back(stod(test_data["OPEN"][i])); // Open(t)
        row.push_back(stod(test_data["PREV. CLOSE"][i-1])); // PREV. CLOSE_Price(t-1)

        X.push_back(row);

    }// Close(t-1)Open(t-1)VWAP(t-1)Low(t-1)High(t-1)No of Trades(t-1)Open(t)

    return X;

}


vector<double> Target_vector(map<string,vector<string>> train_data){

    vector<double> target; 

    for (int i = 0; i < train_data["CLOSE"].size()-1; i++) {
        target.push_back(stod(train_data["CLOSE"][i]));
    }

    return target;
}

// Normalization
double calculate_mean(vector<double> data) {
    double sum = 0.0;
    for (const double& value : data) {
        sum += value;
    }
    return sum / data.size();
}


double calculate_std_dev(vector<double> data) {
    double mean = calculate_mean(data);
    double variance = 0.0;
    for (const double& value : data) {
        double num = value - mean;
        variance += num * num;
    }
    variance /= data.size();
    return sqrt(variance);
}


vector<vector<double>> Z_Normalize_X(vector<vector<double>>&X,vector<vector<double>>&X_test){

    vector<vector<double>>X_T;

    int n= X[0].size();
    for(int i=0;i<n;i++){
        vector<double>column;
        X_T.push_back(column);
    }

    int m = X.size();
   // cout<<m<<endl;

    for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                X_T[j].push_back(X[i][j]);
            }
    }

    vector<double>mean;
    vector<double>std_dev;

    for(int i=0;i<n;i++){
        double mean_num= calculate_mean(X_T[i]);
        double std_dev_num= calculate_std_dev(X_T[i]);
        mean.push_back(mean_num);
        std_dev.push_back(std_dev_num);
    }

    for(int i=0;i<m;i++){
        for(int j=1;j<n;j++){        // "1" Because we shall not normalize the bias term ...    
                X[i][j]=(X[i][j]-mean[j])/(std_dev[j]);        // Normalization...
        }
    }


    int m_test= X_test.size();

    for(int i=0;i<m_test;i++){
        for(int j=1;j<n;j++){        // "1" Because we shall not normalize the bias term ...    
                X_test[i][j]=(X_test[i][j]-mean[j])/(std_dev[j]);        // Normalization...
        }
    }   

   return X;

}

vector<double> Z_Normalize_target(vector<double>target){

        int m=target.size();

        double mean_num= calculate_mean(target);
        double std_dev_num= calculate_std_dev(target);
    
        for(int i=0;i<m;i++){
            target[i]=(target[i]-mean_num)/(std_dev_num);
        }

        return target;

}


double undo_normalization(double ans, vector<double>target){


    double mean_num=calculate_mean(target);
    double std_dev_num=calculate_std_dev(target);

    ans=(ans*(std_dev_num)+(mean_num));
    return ans;

}
// Normalization closed.

double prediction(vector<double> training_example,vector<double> para){

    int n= para.size();
    double pred=0;

    for(int i=0;i<n;i++){
        pred+=(para[i]*training_example[i]);
    }
                                  
    return pred;                  

}

// Function to transpose a matrix
vector<vector<double>> transpose(const vector<vector<double>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<vector<double>> transposed(cols, vector<double>(rows, 0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

// Function to multiply two matrices
vector<vector<double>> multiply(const vector<vector<double>>& A, const vector<vector<double>>& B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();
    vector<vector<double>> result(rowsA, vector<double>(colsB, 0));
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

// Function to perform matrix inversion
vector<vector<double>> inverse(const vector<vector<double>>& matrix) {
    int n = matrix.size();
    vector<vector<double>> identity(n, vector<double>(n, 0));
    for (int i = 0; i < n; i++) {
        identity[i][i] = 1;
    }

    vector<vector<double>> A(matrix);

    for (int i = 0; i < n; i++) {
        // Find pivot row and swap
        int pivotRow = i;
        for (int j = i + 1; j < n; j++) {
            if (abs(A[j][i]) > abs(A[pivotRow][i])) {
                pivotRow = j;
            }
        }
        swap(A[i], A[pivotRow]);
        swap(identity[i], identity[pivotRow]);

        // Make the diagonal elements 1
        double pivot = A[i][i];
        if (pivot == 0) {
            cerr << "Matrix is singular." << endl;
            exit(1);
        }
        for (int j = 0; j < n; ++j) {
            A[i][j] /= pivot;
            identity[i][j] /= pivot;
        }

        // Make other elements 0
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                double factor = A[j][i];
                for (int k = 0; k < n; ++k) {
                    A[j][k] -= factor * A[i][k];
                    identity[j][k] -= factor * identity[i][k];
                }
            }
        }
    }

    return identity;
}

vector<double> Normal(vector<vector<double>> X, vector<double> target){

    vector<vector<double>>Y;
    Y.push_back(target);
    vector<vector<double>>Y_T= transpose(Y);

    vector<vector<double>>X_T= transpose(X);

    vector<vector<double>>R=multiply(X_T,X);
    vector<vector<double>>R_inv=inverse(R);

    vector<vector<double>>K=multiply(X_T,Y_T);

    vector<vector<double>>ANS2=multiply(R_inv,K);

    vector<double>give;
    for(int i=0;i<ANS2.size();i++){
        give.push_back(ANS2[i][0]);
    }

    return give;

}

double Model_Prediction(int date_index,map<string,vector<string>> Train_Data,map<string,vector<string>> Test_Data){

    
    Train_Data=track_rolling_mean(Train_Data);     // Feature Enginnering 2....
    Train_Data=track_ewm(Train_Data);     // Feature Enginnering 2....

    vector<vector<double>> X= Training_Matrix(Train_Data);
    vector<double> target= Target_vector(Train_Data); 

    for (auto& pair : Test_Data) {
        vector<string>& vec = pair.second; // Get reference to the vector
        reverse(vec.begin(), vec.end());   // Reverse the elements of the vector
    }

    Test_Data=track_rolling_mean(Test_Data);     // Feature Enginnering 2....
    Test_Data=track_ewm(Test_Data);     // Feature Enginnering 2....

    for (auto& pair : Test_Data) {
        vector<string>& vec = pair.second; // Get reference to the vector
        reverse(vec.begin(), vec.end());   // Reverse the elements of the vector
    }

    vector<vector<double>> X_test= Training_Matrix_Rev(Test_Data);

    int n = X[0].size();
    int m= target.size();

    vector<vector<double>>  X_norm=Z_Normalize_X(X,X_test);     // X_test is also normalized.....
    vector<double> target_norm = Z_Normalize_target(target);

    double price_prediction=0;

    vector<double>trained_para=Normal(X_norm,target_norm);

     price_prediction= prediction(X_test[date_index],trained_para);

     price_prediction=undo_normalization(price_prediction,target);

    return price_prediction;

}

// Function to write data to a CSV file
void writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Write data to the file
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            // Add comma if it's not the last element in the row
            if (i != row.size() - 1) {
                file << ",";
            }
        }
        file << std::endl; // Move to the next line
    }

    // Close the file
    file.close();
}

double Final_PnL(string symbol,int x,int p){          // will alao require "string date_of_prediction". 

    map<string,vector<string>> Train_Data=parse_data_train(symbol);
    map<string,vector<string>> Test_Data=parse_data_test(symbol);

    // Reversing all the vectors in Test_Data.....

    for (auto& pair : Test_Data) {
        vector<string>& vec = pair.second; // Get reference to the vector
        reverse(vec.begin(), vec.end());   // Reverse the elements of the vector
    }

    map<string,int>date_index;

    for(int i=0;i<Test_Data["DATE"].size();i++){
        date_index[Test_Data["DATE"][i]]=i;
    }

    int m= Test_Data["DATE"].size();

    int positions=0;
    
    double daily_cashflow= 0;
    

    double net_PnL=0;

    vector<vector<string>>daily_cash_vector;

    vector<string>d1;
    d1.push_back("Date");
    d1.push_back("Cashflow");
    daily_cash_vector.push_back(d1);

    vector<vector<string>>order_stats_vector;

    vector<string>d2;
    d2.push_back("Date");
    d2.push_back("Order_Statics");
    d2.push_back("Quantity");
    d2.push_back("Price");
    order_stats_vector.push_back(d2);


    //cout<<m<<endl;

    for(int i=0;i<m-1;i++){      // m-1

        string date= Test_Data["DATE"][i+1];
        double model_price= Model_Prediction(i,Train_Data,Test_Data);
        double actual_price= stod(Test_Data["CLOSE"][i+1]);

        bool buy=false;
        bool sell=false;

        double per_chng= ((model_price-actual_price)/(actual_price))*100;

        if(abs(positions)<=x ){
                if(per_chng>=p){
                        x++;
                        buy=true;
                }

                else if(per_chng<=(-p)){
                        x--;
                        sell=true;
                }
        }

        //  Daily Cashflow.....

        // Order_Statistics.....

        if(buy){

            daily_cashflow -= actual_price;
            vector<string>v;
            v.push_back(date);
            v.push_back(to_string(daily_cashflow));
            daily_cash_vector.push_back(v);

            vector<string>b;
            b.push_back(date);
            b.push_back("BUY");
            b.push_back("1");
            b.push_back(to_string(actual_price));
            order_stats_vector.push_back(b);


        }

        else if(sell){


            daily_cashflow += actual_price;
            vector<string>v;
            v.push_back(date);
            v.push_back(to_string(daily_cashflow));
            daily_cash_vector.push_back(v);    

            vector<string>b;
            b.push_back(date);
            b.push_back("SELL");
            b.push_back("1");
            b.push_back(to_string(actual_price));
            order_stats_vector.push_back(b);

        }

        else{

            vector<string>v;
            v.push_back(date);
            v.push_back(to_string(daily_cashflow));
            daily_cash_vector.push_back(v);  

        }

        buy=false;
        sell=false;

        writeCSV("daily_cashflow.csv", daily_cash_vector);
        writeCSV("order_statistics.csv", order_stats_vector);  

        cout<<date<<"  "<<model_price<<" "<<actual_price<<endl;    // To be written further....

    }

    double close_price_last_day= stod(Test_Data["CLOSE"].back());
    net_PnL=daily_cashflow+ (positions)*(-close_price_last_day);

    vector<vector<string>>cc= {{to_string(net_PnL)}};

    writeCSV("final_PnL.csv", cc);  

    return 0;

}

int main(){

// When the user will enter the command ,, this will form 2 python csv files namely SBIN_train.csv and 
//SBIN_Test.csv 
// MY code will train the model using SBIN_train.csv and then will calculate users PnL using SBIN_tset.csv

    Final_PnL("SBIN",24,0.02);

    return 0;

}


