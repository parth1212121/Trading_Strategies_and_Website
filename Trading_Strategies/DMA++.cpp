#include "DMA++.h"
#include "parse_data.cpp"
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"
using namespace std;

void DMAPP(string symbol, int n, int x ,double p , int max_hold_days , double c1 , double c2 , string start_date , string end_date ) {

    string command = "python3 Data.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n) ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_data(symbol);
    double cash=0;
    vector<int> day_buyed;

    vector<vector<string> > cash_flow;
    vector<vector<string> > order_statictics;
    cash_flow.push_back({"Date","Cashflow"});
    order_statictics.push_back({"Date","Order_dir","Quantity","Price"});
    
    vector<double> price_change;

    price_change.push_back(0);
    for(int i=1;i<table["DATE"].size();i++){
        price_change.push_back(abs(stod(table["CLOSE"][i])-stod(table["CLOSE"][i-1])));
    }

    vector<double> ER;
    for(int i=0;i<n-1;i++){
        ER.push_back(0);
    }
    for(int i=n-1;i<table["DATE"].size();i++){
        double Absolute_price_change=0.0;
        for(int j=0;j<n-1;j++){
            Absolute_price_change+=price_change[i-j];
        }   
        if(Absolute_price_change!=0.0){
            ER.push_back((stod(table["CLOSE"][i])-stod(table["CLOSE"][i-n+1]))/Absolute_price_change);
        }
        else{
            ER.push_back(-1.1234567890);
        }
        
        
    }
    
    vector<pair<string,int> > BUYED;

    vector<double> SF;
    for(int i=0;i<n-1;i++){
        SF.push_back(0);
    }
    SF.push_back(0.5);
    double SFt_=0.5;

    for(int i=n;i<table["DATE"].size();i++){
        if(ER[i]!=-1.1234567890){
            double SFt=SFt_+c1*(((2.0*ER[i])/(1.0+c2)-1.0)/((2.0*ER[i])/(1.0+c2)+1.0)-SFt_);
            SF.push_back(SFt);
            SFt_=SFt;
        }
        else{
            SF.push_back(SFt_);
        }
    }
   
    double AMA=stod(table["CLOSE"][n-1]);

    
    
    for(int j=n-1;j<table["DATE"].size();j++){
        if(j!=n-1){
           
            AMA=AMA+(SF[j]*(stod(table["CLOSE"][j])-AMA));
        
        }
        
        
        

        if(day_buyed.size()>0){
            if(day_buyed[0]==max_hold_days){
                
                vector<string> today_cash_flow;
                string Date=convertDateFormat(table["DATE"][j]);
                today_cash_flow.push_back(Date);
                today_cash_flow.push_back(to_string(cash));
                cash_flow.push_back(today_cash_flow);
                vector<string> today_statistics;
                Date=convertDateFormat(table["DATE"][j]);
                today_statistics.push_back(Date);
                
                if(holding<0){
                    cash-=stod(table["CLOSE"][j]);
                    today_statistics.push_back("BUY");
                    holding++;
                }
                else{
                    cash+=stod(table["CLOSE"][j]);
                    today_statistics.push_back("SELL");
                    holding--;
                }

                
             
                
                today_statistics.push_back("1");
                today_statistics.push_back(table["CLOSE"][j]);
                order_statictics.push_back(today_statistics);
                vector<int > temp;
                for(int i=1;i<day_buyed.size();i++){
                    temp.push_back(day_buyed[i]);
                }
                day_buyed.clear();
                
                for(int i=0;i<temp.size();i++){
                    day_buyed.push_back(temp[i]);
                }
               
            }
        }
        
        
        if((stod(table["CLOSE"][j])-AMA)>((p/100.0)*AMA) && holding<x){
            
            if(holding<0){
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
                vector<int > temp;
                for(int i=1;i<day_buyed.size();i++){
                    temp.push_back(day_buyed[i]);
                }
                day_buyed.clear();
                for(int i=0;i<temp.size();i++){
                    day_buyed.push_back(temp[i]);
                }
                
            }
            else{
                
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
                vector<int > temp;
                for(int i=0;i<day_buyed.size();i++){
                    temp.push_back(day_buyed[i]);
                }
                day_buyed.clear();
                for(int i=0;i<temp.size();i++){
                    day_buyed.push_back(temp[i]);
                }
                day_buyed.push_back(0);
                
            }
            
        }

        
        else if((-stod(table["CLOSE"][j])+AMA)>((p/100.0)*AMA)  && holding>-x){
            if(holding>0){
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
                vector<int > temp;
                for(int i=1;i<day_buyed.size();i++){
                    temp.push_back(day_buyed[i]);
                }
                day_buyed.clear();
                for(int i=0;i<temp.size();i++){
                    day_buyed.push_back(temp[i]);
                }

            }
            else{
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
                vector<int > temp;
                for(int i=0;i<day_buyed.size();i++){
                    temp.push_back(day_buyed[i]);
                }
                day_buyed.clear();
                for(int i=0;i<temp.size();i++){
                    day_buyed.push_back(temp[i]);
                }
                day_buyed.push_back(0);
            }
        }
        else{
                vector<string> today_cash_flow;
                string Date=convertDateFormat(table["DATE"][j]);
                today_cash_flow.push_back(Date);
                today_cash_flow.push_back(to_string(cash));
                cash_flow.push_back(today_cash_flow);
            }
        vector<int > temp;
        for(int i=0;i<day_buyed.size();i++){
            temp.push_back(day_buyed[i]);
        }
        day_buyed.clear();
        for(int i=0;i<temp.size();i++){
            day_buyed.push_back(temp[i]+1);
        }
        
        //cout<<table["DATE"][j]<<" ----- "<<AMA<<" "<<holding<<endl;

        

    }
    cash+=stod(table["CLOSE"][table["CLOSE"].size()-1])*holding;
        
       

    writeCSV("order_statistics.csv",order_statictics);
    writeCSV("daily_cashflow.csv",cash_flow);
    write_pnl("final_pnl.txt",cash);

    remove((symbol+ ".csv").c_str());
    return;
}
