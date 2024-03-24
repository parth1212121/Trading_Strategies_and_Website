#include "RSI.h"
#include "parse_data.cpp"
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"


using namespace std;


void RSI(string symbol, int n, int x , double oversold_threshold , double overbought_threshold , string start_date,string end_date) {


    string command = "python3 Data.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n+1) ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_data(symbol);
    double cash=0;
    vector<vector<string> > cash_flow;
    vector<vector<string> > order_statictics;
    cash_flow.push_back({"Date","Cashflow"});
    order_statictics.push_back({"Date","Order_dir","Quantity","Price"});
    
    for(int j=n;j<table["DATE"].size();j++){
    
        double gain=0;
        double loss=0;
        double number_of_days=n;
        bool increase=true;

        for(int i=n-1;i>=0;i--){
          
            double now=stod(table["CLOSE"][j-i]);
            double prev=stod(table["CLOSE"][j-i-1]);

            if(now-prev>0){
                gain+=now-prev;
         
            }
            else{
                loss+=prev-now;
            }
        }
        double RSI=100.0-100.0/(1+gain/loss);
        
        if(RSI<oversold_threshold && holding<x){
            holding++;
            cash-=stod(table["CLOSE"][j]);
            vector<string> today_cash_flow;
            string Date=convertDateFormat(table["DATE"][j]);
            today_cash_flow.push_back(Date);
            today_cash_flow.push_back(to_string(cash));
            cash_flow.push_back(today_cash_flow);
            vector<string> today_statistics;
            Date=convertDateFormat(table["DATE"][j]);
            today_statistics.push_back(Date);
            today_statistics.push_back("BUY");
            today_statistics.push_back("1");
            today_statistics.push_back(table["CLOSE"][j]);
            order_statictics.push_back(today_statistics);
        }

       
        else if(RSI>overbought_threshold && holding>-x){
            holding--;
            cash+=stod(table["CLOSE"][j]);
            vector<string> today_cash_flow;
            string Date=convertDateFormat(table["DATE"][j]);
            today_cash_flow.push_back(Date);
            today_cash_flow.push_back(to_string(cash));
            cash_flow.push_back(today_cash_flow);
            vector<string> today_statistics;
            Date=convertDateFormat(table["DATE"][j]);
            today_statistics.push_back(Date);
            today_statistics.push_back("SELL");
            today_statistics.push_back("1");
            today_statistics.push_back(table["CLOSE"][j]);
            order_statictics.push_back(today_statistics);
        }
        else{
            vector<string> today_cash_flow;
            string Date=convertDateFormat(table["DATE"][j]);
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
