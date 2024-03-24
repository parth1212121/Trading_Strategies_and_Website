#include "BEST_OF_ALL.h"
using namespace std;

struct StrategyResult {
    vector<vector<string> > cash_flow_thread;
    vector<vector<string> > order_statics_thread;
    double pnl_thread; // Profit and Loss
    // Add any other necessary results here
};


string one_year_backQ(string date){

    string year="";
    for(int i=date.size()-4;i<date.size();i++){
        year.push_back(date[i]);
    }
    string month="";
    for(int i=date.size()-7;i<date.size()-5;i++){
        month.push_back(date[i]);
    }
    string day="";
    for(int i=0;i<2;i++){
        day.push_back(date[i]);
    }


    int x= stoi(year);
    string ans="";
    if(x%4==0 && stoi(month)==2 && stoi(day)==29){
        x--;
        string res=to_string(x);
        ans="01/03/"+res;
 
        return ans;
    }
    x--;
    
    string res=to_string(x);
    
    
    for(int i=0;i<date.size()-4;i++){
        ans.push_back(date[i]);
    }

    ans=ans+res;
    return ans;
}

map< string,vector<string> > parse_dataQ(string symbol,string startegy) {
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

StrategyResult* RSIQ(string symbol, int n, int x , double oversold_threshold , double overbought_threshold , string start_date,string end_date,bool write) {


    string command = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n+1) + " " +  "RSI" ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_dataQ(symbol,"RSI");
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
        
       

    if(write){
        writeCSV("order_statistics.csv",order_statictics);
        writeCSV("daily_cashflow.csv",cash_flow);
        write_pnl("final_pnl.txt",cash);
    }
    
    StrategyResult* result = new StrategyResult();
    result -> cash_flow_thread= cash_flow;
    result -> order_statics_thread= order_statictics;
    result -> pnl_thread= cash;
    return result;
    

}

StrategyResult* MACDQ( string symbol , int x , string start_date , string end_date ,bool write) {
    int n=1;
    string command = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n)+ " " +  "MACD"  ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_dataQ(symbol,"MACD");
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
    if(write){
        writeCSV("order_statistics.csv",order_statictics);
        writeCSV("daily_cashflow.csv",cash_flow);
        write_pnl("final_pnl.txt",cash);
    }
    
    StrategyResult* result = new StrategyResult();
    result -> cash_flow_thread= cash_flow;
    result -> order_statics_thread= order_statictics;
    result -> pnl_thread= cash;
    return result;
}

StrategyResult* DMAPPQ(string symbol, int n, int x ,double p , int max_hold_days , double c1 , double c2 , string start_date , string end_date ,bool write ) {

    string command = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n) + " " +  "DMAPP" ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_dataQ(symbol,"DMAPP");
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
        int BUY=0;
        
        

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
                    BUY=1;
                    cash-=stod(table["CLOSE"][j]);
                    today_statistics.push_back("BUY");
                    holding++;
                }
                else{
                    BUY=-1;
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
        
        
        if((stod(table["CLOSE"][j])-AMA)>((p/100.0)*AMA) && holding<x && BUY!=1){
            
            if(holding<0 ){
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

        
        else if((-stod(table["CLOSE"][j])+AMA)>((p/100.0)*AMA)  && holding>-x && BUY!=-1){
            
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
        else if(BUY==0){
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
        
        //cout<<table["CLOSE"][j]<<"\t"<<AMA*(1.0+p/100.0)<<"\t"<<AMA*(1.0-p/100.0)<<"\t"<<SF[j]<<"\t"<<ER[j]<<"\t"<<holding<<endl;
        

    }
    cash+=stod(table["CLOSE"][table["CLOSE"].size()-1])*holding;
        
       

    if(write){
        writeCSV("order_statistics.csv",order_statictics);
        writeCSV("daily_cashflow.csv",cash_flow);
        write_pnl("final_pnl.txt",cash);
    }
    
    StrategyResult* result = new StrategyResult();
    result -> cash_flow_thread= cash_flow;
    result -> order_statics_thread= order_statictics;
    result -> pnl_thread= cash;
    return result;
}

StrategyResult* ADXQ( string symbol , int n , int x ,double adx_threshold ,string start_date , string end_date ,bool write ) {
    
    string command = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(2)  + " " +  "ADX" ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;



    table=parse_dataQ(symbol,"ADX");

   


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
        
       
    if(write){
        writeCSV("order_statistics.csv",order_statictics);
        writeCSV("daily_cashflow.csv",cash_flow);
        write_pnl("final_pnl.txt",cash);
    }
    
    StrategyResult* result = new StrategyResult();
    result -> cash_flow_thread= cash_flow;
    result -> order_statics_thread= order_statictics;
    result -> pnl_thread= cash;
    return result;
}

StrategyResult*  BASICQ( string symbol , int n , int x , string start_date , string end_date ,bool write){
    
    
    string command = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n+1) + " " +  "BASIC" ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_dataQ(symbol,"BASIC");
    double cash=0;
    vector<vector<string> > cash_flow;
    vector<vector<string> > order_statictics;
    cash_flow.push_back({"Date","Cashflow"});
    order_statictics.push_back({"Date","Order_dir","Quantity","Price"});
    
    
    for(int j=n;j<table["DATE"].size();j++){
        
        bool increase=true;
        for(int i=n;i>0;i--){
            
            double now=stod(table["CLOSE"][j-i]);
            double next=stod(table["CLOSE"][j-i+1]);
            if(next<=now){
                increase=false;
                break;
            }
        }

        
        

        bool decrease=true;
        for(int i=n;i>0;i--){
            double now=stod(table["CLOSE"][-i+j]);
            double next=stod(table["CLOSE"][-i+j+1]);
            if(next>=now){
                decrease=false;
            }
        }
        if(increase && holding<x){
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
        else if(decrease && holding>-x){
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
        
       

    if(write){
        writeCSV("order_statistics.csv",order_statictics);
        writeCSV("daily_cashflow.csv",cash_flow);
        write_pnl("final_pnl.txt",cash);
    }
    
    StrategyResult* result = new StrategyResult();
    result -> cash_flow_thread= cash_flow;
    result -> order_statics_thread= order_statictics;
    result -> pnl_thread= cash;
    return result;
}

StrategyResult* DMAQ(string symbol , int n , int x , double p , string start_date , string end_date ,bool write ) {
    string command = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(n+1)   + " " +  "DMA" ;
    
    system( command.c_str());
    int holding=0;
    map<string,vector<string> > table;
    table=parse_dataQ(symbol,"DMA");
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
    if(write){
        writeCSV("order_statistics.csv",order_statictics);
        writeCSV("daily_cashflow.csv",cash_flow);
        write_pnl("final_pnl.txt",cash);
    }
    
    StrategyResult* result = new StrategyResult();
    result -> cash_flow_thread= cash_flow;
    result -> order_statics_thread= order_statictics;
    result -> pnl_thread= cash;
    return result;
    
}


// Linear regression


vector<vector<double>> Training_MatrixQ(map<string,vector<string>> train_data){

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


vector<vector<double>> Training_Matrix_RevQ(map<string,vector<string>> test_data){

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


vector<double> Target_vectorQ(map<string,vector<string>> train_data){

    vector<double> target; 

    for (int i = 1; i < train_data["CLOSE"].size(); i++) {
        target.push_back(stod(train_data["CLOSE"][i]));
    }

    return target;
}


double predictionQ(vector<double> training_example,vector<double> para){

    int n= para.size();
    double pred=0;

    for(int i=0;i<n;i++){
        pred+=(para[i]*training_example[i]);
    }
                                  
    return pred;                  

}

// Function to transpose a matrix
vector<vector<double>> transposeQ(const vector<vector<double>> matrix) {
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
vector<vector<double>> multiplyQ(const vector<vector<double>> A, const vector<vector<double>> B) {
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
vector<vector<double>> inverseQ(const vector<vector<double>> matrix) {
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

vector<double> NormalQ(vector<vector<double>> X, vector<double> target){

    vector<vector<double>>Y;
    Y.push_back(target);
    vector<vector<double>>Y_T= transposeQ(Y);

    vector<vector<double>>X_T= transposeQ(X);

    vector<vector<double>>R=multiplyQ(X_T,X);
    vector<vector<double>>R_inv=inverseQ(R);

    vector<vector<double>>K=multiplyQ(X_T,Y_T);

    vector<vector<double>>ANS2=multiplyQ(R_inv,K);

    vector<double>give;
    for(int i=0;i<ANS2.size();i++){
        give.push_back(ANS2[i][0]);
    }

    return give;

}

double Model_PredictionQ(int date_index,vector<vector<double>>X,vector<double> target,vector<vector<double>> X_test,vector<double>trained_para){

    int n = X[0].size();
    int m= target.size();

    double price_prediction=0;

    price_prediction= predictionQ(X_test[date_index],trained_para);

    return price_prediction;

}

StrategyResult* Final_PnLQ(string symbol,int x,int p,bool write){          // will allow require "string date_of_prediction". 

    map<string,vector<string>> Train_Data=parse_dataQ(symbol,"LR_TRAIN");
    map<string,vector<string>> Test_Data=parse_dataQ(symbol,"LR_TEST");

    vector<vector<double>> X= Training_MatrixQ(Train_Data);
    vector<double> target= Target_vectorQ(Train_Data); 

    vector<vector<double>> X_test= Training_Matrix_RevQ(Test_Data);


    vector<double>trained_para=NormalQ(X,target);

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
    d2.push_back("Order_dir");
    d2.push_back("Quantity");
    d2.push_back("Price");
    order_stats_vector.push_back(d2);

    for(int i=0;i<m-1;i++){      // m-1

        string date= Test_Data["DATE"][i+1];
        double model_price= Model_PredictionQ(i,X,target,X_test,trained_para);
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

    writeCSV("LR_simple_order_statictics.csv",order_stats_vector);
    writeCSV("LR_simple_daily_cashflow.csv",daily_cash_vector);
    writeCSV("final_simple_PnL.csv", cc);  

    }

    remove((symbol + "_LR_TRAIN.csv").c_str());
    remove((symbol + "_LR_TEST.csv").c_str());

    StrategyResult* result = new StrategyResult();
    
    result -> cash_flow_thread= daily_cash_vector;
    result -> order_statics_thread= order_stats_vector;
    result -> pnl_thread= net_PnL;
    
    // Asuming that the final value of "cash" id final_PnL.

    return result;
   
}

StrategyResult* LinearRegeressionQ( string symbol , int x , int p ,string train_start_date , string train_end_date ,string start_date , string end_date, bool write ) {


    string command1 = "python3 LRData.py "+ symbol + " " + train_start_date + " " + train_end_date + " " + to_string(2) +" " +"LR_TRAIN" ;
    
    system( command1.c_str());

    string command2 = "python3 LRData.py "+ symbol + " " + start_date + " " + end_date + " " + to_string(2) +" " +"LR_TEST"  ;
    
    system( command2.c_str());


    return  Final_PnLQ(symbol,x,p,write);   // dates will be used by python files for 2 csv files generation...

}






struct StrategyArgs {
    std::string symbol;
    string start_date;
    string end_date;
    // Add any other necessary parameters here
};

void* basic(void* args) {

 // Extract strategy parameters from arguments
    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    StrategyResult* result = BASICQ(symbol,7,5,start_date,end_date,0);

    return (void*) result;


}

void* macd(void* args) {

    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    StrategyResult* result = MACDQ(symbol,5,start_date,end_date,0);

    return (void*) result;

}

void* dma(void* args) {

    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    StrategyResult* result =  DMAQ(symbol,50,5,2,start_date,end_date,0);

    return (void*) result;
}

void* adx(void* args) {

    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    StrategyResult* result = ADXQ(symbol,14, 5 , 25, start_date,end_date,0);

    return (void*) result;

}

void* rsi(void* args) {

    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    StrategyResult* result = RSIQ(symbol,14,5,30,70,start_date,end_date,0);

    return (void*) result;

}
void* dmapp(void* args) {

    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    StrategyResult* result = DMAPPQ(symbol,14,5,5,28,2,0.2,start_date,end_date,0);

    return (void*) result;

}

void* lr(void* args) {

    StrategyArgs* arguments = (StrategyArgs*) args;
    string symbol = arguments->symbol;
    string start_date = arguments->start_date;
    string end_date = arguments->end_date;

    string start_train_date=one_year_backQ(start_date);

    string end_train_date=one_year_backQ(end_date);

    StrategyResult* result = LinearRegeressionQ(symbol,5,2,start_train_date,end_train_date,start_date,end_date,0);

    return (void*) result;

    // p not given...

}




void bestOfAll(const std::string& symbol, const std::string& start_date, const std::string& end_date) {

    StrategyArgs args;
    args.symbol = symbol;
    args.start_date = start_date;
    args.end_date = end_date;
    
    pthread_t threads[7];
    pthread_create(&threads[0], NULL, basic, (void*)&args);
    pthread_create(&threads[1], NULL, macd, (void*)&args);
    pthread_create(&threads[2], NULL, dma, (void*)&args);
    pthread_create(&threads[3], NULL, adx, (void*)&args);
    pthread_create(&threads[4], NULL, rsi, (void*)&args);
    pthread_create(&threads[5], NULL, dmapp, (void*)&args);
    pthread_create(&threads[6], NULL, lr, (void*)&args);


    StrategyResult* results[7];
    pthread_join(threads[0], (void**)&results[0]);
    pthread_join(threads[1], (void**)&results[1]);
    pthread_join(threads[2], (void**)&results[2]);       // There seems to be a problerm with flushing of threads.
    pthread_join(threads[3], (void**)&results[3]);
    pthread_join(threads[4], (void**)&results[4]);
    pthread_join(threads[5], (void**)&results[5]);
    pthread_join(threads[6], (void**)&results[6]);                                                // For initial 2-3 times correct data is wriiten in "best" after that some data shrinks..
                                                   // then after 1-2 times of writing of currupted data,, It again resumes writing correct data..
                                                        // Seems to  be a Hardware interference!!!!
    // Find the strategy with the best PnL
    StrategyResult* bestResult = results[0];
    for (int i = 1; i < 7; ++i) {
        
        if (results[i]->pnl_thread > bestResult->pnl_thread) {
            
            bestResult = results[i];
        }
    }

    writeCSV("order_statistics.csv", bestResult->order_statics_thread);
    writeCSV("daily_cashflow.csv", bestResult->cash_flow_thread);
    write_pnl("final_pnl.txt",bestResult->pnl_thread);

    for (int i = 0; i < 7; ++i) {
        delete results[i]; // Free memory allocated for StrategyResult
    }
    remove((symbol+ "_ADX.csv").c_str());
    remove((symbol+ "_BASIC.csv").c_str());
    remove((symbol+ "_DMA.csv").c_str());
    remove((symbol+ "_DMAPP.csv").c_str());
    remove((symbol+ "_MACD.csv").c_str());
    remove((symbol+ "_RSI.csv").c_str());
    remove((symbol+ ".csv").c_str());
    //cout<<bestResult->cash_flow_thread.size()<<endl;

    return ;


    
}

