#include <iostream>
#include <pthread.h>
#include <vector>                // LOADS OF PROBLERMS WHEN TRYING TO LINK!!!!
#include <string>
#include <map>
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"
#include "LinearRegression.h"
using namespace std;
struct StrategyResult ;
map< string,vector<string> > parse_dataQ(string symbol,string startegy);

StrategyResult* PAIRSQ(string symbol1, string symbol2 , int n , int x , string start_date , string end_date , double threshold , double stop_loss_threshold ,bool write) ;
StrategyResult* RSIQ(string symbol, int n, int x , double oversold_threshold , double overbought_threshold , string start_date,string end_date,bool write) ;
StrategyResult* MACDQ( string symbol , int x , string start_date , string end_date ,bool write) ;
StrategyResult* DMAPPQ(string symbol, int n, int x ,double p , int max_hold_days , double c1 , double c2 , string start_date , string end_date ,bool write ) ;
StrategyResult* ADXQ( string symbol , int n , int x ,double adx_threshold ,string start_date , string end_date ,bool write ) ;
StrategyResult*  BASICQ( string symbol , int n , int x , string start_date , string end_date ,bool write);
StrategyResult* DMAQ(string symbol , int n , int x , double p , string start_date , string end_date ,bool write ) ;

string one_year_backQ(string date);
vector<vector<double>> Training_MatrixQ(map<string,vector<string>> train_data);
vector<vector<double>> Training_Matrix_RevQ(map<string,vector<string>> test_data);
vector<double> Target_vectorQ(map<string,vector<string>> train_data);
double predictionQ(vector<double> training_example,vector<double> para);
vector<vector<double>> transposeQ(const vector<vector<double>> matrix);
vector<vector<double>> multiplyQ(const vector<vector<double>> A, const vector<vector<double>> B);
vector<vector<double>> inverseQ(const vector<vector<double>> matrix);
vector<double> NormalQ(vector<vector<double>> X, vector<double> target);
double Model_PredictionQ(int date_index,vector<vector<double>>X,vector<double> target,vector<vector<double>> X_test,vector<double>trained_para);
StrategyResult* Final_PnLQ(string symbol,int x,int p,bool write);
StrategyResult* LinearRegeressionQ( string symbol , int x , int p ,string train_start_date , string train_end_date ,string start_date , string end_date, bool write ); 


struct StrategyArgs ;
void* basic(void* args) ;

void* macd(void* args) ;
void* dma(void* args) ;
void* adx(void* args) ;
void* rsi(void* args) ;
void* dmapp(void* args);
void* lr(void* args);

void bestOfAll(const std::string& symbol, const std::string& start_date, const std::string& end_date) ;
