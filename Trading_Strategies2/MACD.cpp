#include "MACD.h"
#include "parse_data.cpp"
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"
using namespace std;

void MACD( string symbol , int x , string start_date , string end_date ) {
    int n=1;
    string command = "python3 Data.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n) ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_data(symbol);
    double cash=0;
    vector<vector<string> > cash_flow;
    vector<vector<string> > order_statictics;
    cash_flow.push_back({"Date","Cashflow"});
    order_statictics.push_back({"Date","Order_dir","Quantity","Price"});
    double EWMtL=stod(table["CLOSE"][ 0]);
    double EWMtS=stod(table["CLOSE"][ 0]);
    double signal=0;
    double MACD=0;
    
    for(int i=0;i<table["DATE"].size();i++){
        if(i!=0){
            EWMtL = (2.0* ( stod(table["CLOSE"][ i  ]) - EWMtL )/27.0)  + EWMtL ;
    
    
            EWMtS = (2.0* ( stod(table["CLOSE"][ i ]) - EWMtS )/13.0)  + EWMtS ;
            MACD=EWMtS-EWMtL;
            signal = ((2.0* (( MACD) - signal ))/10.0)  + signal ;

        }
        
   
        
        if(MACD>signal && holding<x ){
            holding++;
            cash-=stod(table["CLOSE"][i]);
            vector<string> today_cash_flow;
            string Date=convertDateFormat(table["DATE"][i]);
            today_cash_flow.push_back(Date);
            today_cash_flow.push_back(to_string(cash));
            cash_flow.push_back(today_cash_flow);
            vector<string> today_statistics;
            Date=convertDateFormat(table["DATE"][i]);
            today_statistics.push_back(Date);
            today_statistics.push_back("BUY");
            today_statistics.push_back("1");
            today_statistics.push_back(table["CLOSE"][i]);
            order_statictics.push_back(today_statistics);
        }

       
        else if(MACD<signal && holding>-x ){
            holding--;
            cash+=stod(table["CLOSE"][i]);
            vector<string> today_cash_flow;
            string Date=convertDateFormat(table["DATE"][i]);
            today_cash_flow.push_back(Date);
            today_cash_flow.push_back(to_string(cash));
            cash_flow.push_back(today_cash_flow);
            vector<string> today_statistics;
            Date=convertDateFormat(table["DATE"][i]);
            today_statistics.push_back(Date);
            today_statistics.push_back("SELL");
            today_statistics.push_back("1");
            today_statistics.push_back(table["CLOSE"][i]);
            order_statictics.push_back(today_statistics);
        }
        else{
            vector<string> today_cash_flow;
            string Date=convertDateFormat(table["DATE"][i]);
            today_cash_flow.push_back(Date);
            today_cash_flow.push_back(to_string(cash));
            cash_flow.push_back(today_cash_flow);
        }

        

    }
    cash+=stod(table["CLOSE"][table["CLOSE"].size()-1])*holding;
    writeCSV("order_statistics.csv",order_statictics);
    writeCSV("daily_cashflow.csv",cash_flow);
    write_pnl("final_pnl.txt",cash);
    remove((symbol+ ".csv").c_str());
    return ;
}