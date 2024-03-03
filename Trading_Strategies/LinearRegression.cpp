#include <iostream>
#include <vector>                // LOADS OF PROBLERMS WHEN TRYING TO LINK!!!!
#include <string>
#include <map>
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"
using namespace std;



map<string,vector<string> > parse_data_LR(string symbol,string startegy) {
    // Open the CSV file

    map<string, vector<string> > dataMap;

    ifstream file(symbol+"_"+startegy+ ".csv");
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


vector<vector<double>> Training_Matrix(map<string,vector<string>> train_data){

    vector<vector<double>> X; // Training_Matrix

    for (int i = 1; i < train_data["CLOSE"].size(); i++) {

        vector<double> row;
        row.push_back(1.0); // Bias term           //.....Feature Enginnering 3.

        row.push_back(stod(train_data["CLOSE"][i-1])); // Close(t-1)
        row.push_back(stod(train_data["OPEN"][i-1])); // Open(t-1)
        row.push_back(stod(train_data["VWAP"][i-1])); // VWAP(t-1)
        row.push_back(stod(train_data["LOW"][i-1])); // Low(t-1)
        row.push_back(stod(train_data["HIGH"][i-1])); // High(t-1)
        row.push_back(stod(train_data["NO OF TRADES"][i-1])); // No of Trades(t-1)

        row.push_back(stod(train_data["OPEN"][i])); // Open(t)
        X.push_back(row);

    }

    return X;

}


vector<vector<double>> Training_Matrix_Rev(map<string,vector<string>> test_data){

    vector<vector<double>> X; // Test_Matrix

    for (int i = 1; i < test_data["CLOSE"].size(); i++) {

        vector<double> row;
        row.push_back(1.0); // Bias term       

        //  Features 

        row.push_back(stod(test_data["CLOSE"][i-1])); // Close(t-1)
        row.push_back(stod(test_data["OPEN"][i-1])); // Open(t-1)
        row.push_back(stod(test_data["VWAP"][i-1])); // VWAP(t-1)
        row.push_back(stod(test_data["LOW"][i-1])); // Low(t-1)
        row.push_back(stod(test_data["HIGH"][i-1])); // High(t-1)
        row.push_back(stod(test_data["NO OF TRADES"][i-1])); // No of Trades(t-1)

        row.push_back(stod(test_data["OPEN"][i])); // Open(t)

        X.push_back(row);

    }

    return X;

}


vector<double> Target_vector(map<string,vector<string>> train_data){

    vector<double> target; 

    for (int i = 1; i < train_data["CLOSE"].size(); i++) {
        target.push_back(stod(train_data["CLOSE"][i]));
    }

    return target;
}


double prediction(vector<double> training_example,vector<double> para){

    int n= para.size();
    double pred=0;

    for(int i=0;i<n;i++){
        pred+=(para[i]*training_example[i]);
    }
                                  
    return pred;                  

}

// Function to transpose a matrix
vector<vector<double>> transpose(const vector<vector<double>> matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<vector<double>> transposed(cols, vector<double>(rows, 0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }
    return transposed;
}

// Function to multiply two matrices
vector<vector<double>> multiply(const vector<vector<double>> A, const vector<vector<double>> B) {
    int rowsA = A.size();
    int colsA = A[0].size();
    int colsB = B[0].size();
    vector<vector<double>> result(rowsA, vector<double>(colsB, 0));
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}

// Function to perform matrix inversion
vector<vector<double>> inverse(const vector<vector<double>> matrix) {
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
        for (int j = 0; j < n; j++) {
            A[i][j] /= pivot;
            identity[i][j] /= pivot;
        }

        // Make other elements 0
        for (int j = 0; j < n; j++) {
            if (j != i) {
                double factor = A[j][i];
                for (int k = 0; k < n; k++) {
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

double Model_Prediction(int date_index,vector<vector<double>>X,vector<double> target,vector<vector<double>> X_test,vector<double>trained_para){

    int n = X[0].size();
    int m= target.size();

    double price_prediction=0;

    price_prediction= prediction(X_test[date_index],trained_para);

    return price_prediction;

}

void Final_PnL(string symbol,int x,int p,bool write){          // will allow require "string date_of_prediction". 

    map<string,vector<string>> Train_Data=parse_data_LR(symbol,"LR_TRAIN");
    map<string,vector<string>> Test_Data=parse_data_LR(symbol,"LR_TEST");

    vector<vector<double>> X= Training_Matrix(Train_Data);
    vector<double> target= Target_vector(Train_Data); 

    vector<vector<double>> X_test= Training_Matrix_Rev(Test_Data);


    vector<double>trained_para=Normal(X,target);

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

    for(int i=0;i<m-1;i++){      // m-1

        string date= Test_Data["DATE"][i+1];
        double model_price= Model_Prediction(i,X,target,X_test,trained_para);
        double actual_price= stod(Test_Data["CLOSE"][i+1]);

        bool buy=false;
        bool sell=false;

        double per_chng= ((model_price-actual_price)/(actual_price))*100;

                if(per_chng>=p && positions<x){
                        positions++;
                        buy=true;
                }

                else if(per_chng<=(-p) && positions>(-x)){
                        positions--;
                        sell=true;
                }
        

        //  Daily Cashflow.....

        // Order_Statistics.....

        if(buy){

            daily_cashflow -= actual_price;
            vector<string>v;
            v.push_back(convertDateFormat(date));
            v.push_back(to_string(daily_cashflow));
            daily_cash_vector.push_back(v);

            vector<string>b;
            b.push_back(convertDateFormat(date));
            b.push_back("BUY");
            b.push_back("1");
            b.push_back(to_string(actual_price));
            order_stats_vector.push_back(b);

        }

        else if(sell){

            daily_cashflow += actual_price;
            vector<string>v;
            v.push_back(convertDateFormat(date));
            v.push_back(to_string(daily_cashflow));
            daily_cash_vector.push_back(v);    

            vector<string>b;
            b.push_back(convertDateFormat(date));
            b.push_back("SELL");
            b.push_back("1");
            b.push_back(to_string(actual_price));
            order_stats_vector.push_back(b);

        }
        else{

            vector<string>v;
            v.push_back(convertDateFormat(date));
            v.push_back(to_string(daily_cashflow));
            daily_cash_vector.push_back(v);  
        }

        buy=false;
        sell=false;

    }

    double close_price_last_day= stod(Test_Data["CLOSE"].back());
    net_PnL=daily_cashflow+ (positions)*(close_price_last_day);

    vector<vector<string>>cc= {{to_string(net_PnL)}};

    if(write){

    writeCSV("order_statistics.csv",order_stats_vector);
    writeCSV("daily_cashflow.csv",daily_cash_vector);
    write_pnl("final_pnl.txt",net_PnL);

    }


    remove((symbol + "_LR_TRAIN.csv").c_str());
    remove((symbol + "_LR_TEST.csv").c_str());

    return ;
   
}

void LinearRegeression( string symbol , int x , int p ,string train_start_date , string train_end_date ,string start_date , string end_date, bool write ) {

    
    string command1 = "python3 LRData.py "+ symbol + " " + train_start_date + " " + train_end_date + " " + to_string(2) +" " +"LR_TRAIN" ;
    
    system( command1.c_str());

    string command2 = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(2) +" " +"LR_TEST"  ;
    
    system( command2.c_str());


    Final_PnL(symbol,x,p,write);   // dates will be used by python files for 2 csv files generation...
    return;
}


