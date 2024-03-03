#include "DMA.h"
#include "parse_data.cpp"
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"
using namespace std;

void DMA(string symbol , int n , int x , double p , string start_date , string end_date ) {
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

        double DMA=0;
        double SD=0;
        
        bool increase=true;
        for(int i=n-1;i>=0;i--){
            
            DMA+=stod(table["CLOSE"][j-i]);
            
        }
        double number_of_days=n;
        DMA/=number_of_days;
        for(int i=n-1;i>=0;i--){
            
            SD+=((stod(table["CLOSE"][j-i])-DMA)*(stod(table["CLOSE"][j-i])-DMA));
            
        }

        SD=sqrt(SD/number_of_days);
        //cout<<table["DATE"][j]<<" "<<stod(table["CLOSE"][j])<<" "<< DMA <<" ? "<<p<< " "<<SD<<endl;
        if(( stod(table["CLOSE"][j]) - DMA )>p*SD && holding<x){
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
        else if(( -stod(table["CLOSE"][j]) + DMA )>p*SD && holding>-x){
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
    return;
}
