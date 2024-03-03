#include "ADX.h"
#include "parse_data.cpp"
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "final_pnl.cpp"

using namespace std;

void ADX( string symbol , int n , int x ,double adx_threshold ,string start_date , string end_date ) {
    
    string command = "python3 Data.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(2) ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;



    table=parse_data(symbol);

    /* cout<<"std::vector<vector<double> > stock_data = {";
    for(int i=0;i<table["DATE"].size();i++){
        string Date=convertDateFormat(table["DATE"][i]);
       
        cout<<"{"<<table["CLOSE"][i]<<","<<table["HIGH"][i]<<","<<table["LOW"][i]<<"},";
    }   
    cout<<"};"<<endl;
    cout<<"std::vector<string> dates = {";
    for(int i=0;i<table["DATE"].size();i++){
        string Date=convertDateFormat(table["DATE"][i]);
       
        cout<<"\""<<Date<<"\",";
    }  */


    vector<double> TRt;
    TRt.push_back(0);
    
    for(int i=1; i<table["DATE"].size() ; i++){

        double value=(stod(table["HIGH"][i])-stod(table["LOW"][i]));

        if(value<(stod(table["HIGH"][i])-stod(table["CLOSE"][i-1]))){
            value=(stod(table["HIGH"][i])-stod(table["CLOSE"][i-1]));
        }
        if(value<(stod(table["LOW"][i])-stod(table["CLOSE"][i-1]))){
            value=(stod(table["LOW"][i])-stod(table["CLOSE"][i-1]));
        }

        TRt.push_back(value);
    }

    vector<double> DMp;
    DMp.push_back(0);
    for(int i=1;i<table["HIGH"].size();i++){
        DMp.push_back(max((stod(table["HIGH"][i])-stod(table["HIGH"][i-1])),0.0));
       
    }
    vector<double> DMm;
    DMm.push_back(0);
    for(int i=1;i<table["HIGH"].size();i++){
        DMm.push_back(max((stod(table["LOW"][i])-stod(table["LOW"][i-1])),0.0));
    }


    vector<double> ATR;
    ATR.push_back(0);
    ATR.push_back(TRt[1]);

    double N=n;
    double atr=TRt[1];
    for(int i=2;i<table["DATE"].size();i++){
        atr = (2.0/(N+1)) * ( TRt[i] - atr ) + atr ;
        ATR.push_back(atr);
    }
    

    vector<double> DIp;
    DIp.push_back(0);
    DIp.push_back(DMp[1]/ATR[1]);
    double dip=DMp[1]/ATR[1];
    for(int i=2;i<table["DATE"].size();i++){
    
          
        dip = (2.0/(4.0)) * ( (DMp[i]/ATR[i]) - dip) + dip ;
        
    
        DIp.push_back(dip);
        
    }

    vector<double> DIm;
    DIm.push_back(0);
    DIm.push_back(DMm[1]/ATR[1]);
    double dim=DMm[1]/ATR[1];
    for(int i=2;i<table["DATE"].size();i++){
    
          
        dim = (2.0/(4.0)) * ( (DMm[i]/ATR[i]) - dim) + dim ;
        
    
        DIm.push_back(dim);
        
    }

    vector<double> DX;
    for(int i=0;i<1;i++){
        DX.push_back(0);
    }
    for(int i=1;i<table["DATE"].size();i++){
        if(DIp[i]+DIm[i]==0){
            
            DX.push_back(0);
        }
        else{
            DX.push_back(((DIp[i]-DIm[i])/(DIp[i]+DIm[i]))*100.0);
        }
        
        
    }
    
    double cash=0;
    vector<vector<string> > cash_flow;
    vector<vector<string> > order_statictics;
    cash_flow.push_back({"Date","Cashflow"});
    order_statictics.push_back({"Date","Order_dir","Quantity","Price"});

    double ewmt=DX[1];
    for(int j=1;j<table["DATE"].size();j++){
        
        if(j!=1){
            
            
            
            ewmt= (2.0/(N+1)) * ((DX[j]) - ewmt ) + ewmt ;
            
            
        }
        
        double ADX=ewmt;
        //cout<<table["DATE"][j]<<"\t"<<ADX<<"\t"<<DX[j]<<"\t"<<DIp[j]<<"\t"<<DIm[j]<<"\t"<<DMp[j]<<"\t"<<DMm[j]<<"\t"<<ATR[j]<<"\t"<<TRt[j]<<endl;
        
        if(DX[j]!=0){
            if(ADX>adx_threshold && holding<x){
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

        
            else if(ADX<adx_threshold && holding>-x){
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