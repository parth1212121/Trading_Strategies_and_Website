#include "parse_data.cpp"
#include "writeCSV.cpp"
#include "convertDateFormat.cpp"
#include "PAIRS.h"
#include "final_pnl.cpp"
using namespace std;

void PAIRS(string symbol1, string symbol2 , int n , int x , string start_date , string end_date , double threshold , double stop_loss_threshold ) {
    if(stop_loss_threshold==-1.0){
        
        
        string command = "python3 Data.py "+ symbol1 + " " + start_date + " " + end_date + " " + to_string(n) ;
        
        system( command.c_str());
        int holding=0;
        map<string,vector<string> > table1;
        table1=parse_data(symbol1);

        command = "python3 Data.py "+ symbol2 + " " + start_date + " " + end_date + " " + to_string(n) ;
        
        system( command.c_str());
    
        map<string,vector<string> > table2;
        table2=parse_data(symbol2);
        /* for(int i=0;i<table1["DATE"].size();i++){
            cout<<"{"<<table1["CLOSE"][i]<<","<<table1["CLOSE"][i]<<"},";
        }
        cout<<endl; */


        double cash=0;
        vector<vector<string> > cash_flow;
        vector<vector<string> > order_statictics1;
        vector<vector<string> > order_statictics2;
        cash_flow.push_back({"Date","Cashflow"});
        order_statictics1.push_back({"Date","Order_dir","Quantity","Price"});
        order_statictics2.push_back({"Date","Order_dir","Quantity","Price"});
    
        for(int j=n-1;j<table1["DATE"].size();j++){
            
            double mean=0;
            double SD=0;
            
        
            for(int i=n-1;i>=0;i--){
                
                mean+=stod(table1["CLOSE"][j-i])-stod(table2["CLOSE"][j-i]);
                
            }

            double number_of_days=n;
            mean/=number_of_days;
            for(int i=n-1;i>=0;i--){
                
                SD+=((stod(table1["CLOSE"][j-i])-stod(table2["CLOSE"][j-i])-mean)*(stod(table1["CLOSE"][j-i])-stod(table2["CLOSE"][j-i])-mean));
                
            }

            SD=sqrt(SD/number_of_days);
            double z_score=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j])-mean)/SD;
                
            if( z_score>threshold && holding<x){
                holding++;
                cash+=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j]));
                vector<string> today_cash_flow;
                string Date=convertDateFormat(table1["DATE"][j]);
                today_cash_flow.push_back(Date);
                today_cash_flow.push_back(to_string(cash));
                cash_flow.push_back(today_cash_flow);
                vector<string> today_statistics1;
                Date=convertDateFormat(table1["DATE"][j]);
                today_statistics1.push_back(Date);
                today_statistics1.push_back("SELL");
                today_statistics1.push_back("1");
                today_statistics1.push_back(table1["CLOSE"][j]);
                order_statictics1.push_back(today_statistics1);
                vector<string> today_statistics2;
                Date=convertDateFormat(table1["DATE"][j]);
                today_statistics2.push_back(Date);
                today_statistics2.push_back("BUY");
                today_statistics2.push_back("1");
                today_statistics2.push_back(table2["CLOSE"][j]);
                order_statictics2.push_back(today_statistics2);
            }

            
            else if( z_score<-threshold && holding>-x){
                holding--;
                cash+=(-stod(table1["CLOSE"][j])+stod(table2["CLOSE"][j]));
                vector<string> today_cash_flow;
                string Date=convertDateFormat(table1["DATE"][j]);
                today_cash_flow.push_back(Date);
                today_cash_flow.push_back(to_string(cash));
                cash_flow.push_back(today_cash_flow);
                vector<string> today_statistics1;
                Date=convertDateFormat(table1["DATE"][j]);
                today_statistics1.push_back(Date);
                today_statistics1.push_back("BUY");
                today_statistics1.push_back("1");
                today_statistics1.push_back(table1["CLOSE"][j]);
                order_statictics1.push_back(today_statistics1);
                vector<string> today_statistics2;
                Date=convertDateFormat(table1["DATE"][j]);
                today_statistics2.push_back(Date);
                today_statistics2.push_back("SELL");
                today_statistics2.push_back("1");
                today_statistics2.push_back(table2["CLOSE"][j]);
                order_statictics2.push_back(today_statistics2);
            }

            else{
                vector<string> today_cash_flow;
                string Date=convertDateFormat(table1["DATE"][j]);
                today_cash_flow.push_back(Date);
                today_cash_flow.push_back(to_string(cash));
                cash_flow.push_back(today_cash_flow);
            }

            

        }
        cash+=(stod(table2["CLOSE"][table2["CLOSE"].size()-1])-stod(table1["CLOSE"][table1["CLOSE"].size()-1]))*holding;
        
       

        writeCSV("order_statistics_1.csv",order_statictics1);
        writeCSV("order_statistics_2.csv",order_statictics2);
        writeCSV("daily_cashflow.csv",cash_flow);
        
        
        write_pnl("final_pnl.txt",cash);
        

    }

    else{
        
        vector<vector<double> > daily_pay;
        
        string command = "python3 Data.py "+ symbol1 + " " + start_date + " " + end_date + " " + to_string(n) ;
        
        system( command.c_str());
        int holding=0;
        map<string,vector<string> > table1;
        table1=parse_data(symbol1);

        command = "python3 Data.py "+ symbol2 + " " + start_date + " " + end_date + " " + to_string(n) ;
        
        system( command.c_str());
    
        map<string,vector<string> > table2;
        table2=parse_data(symbol2);
        cout<<"vector<double > date={";
        for(int i=0;i<table1["DATE"].size();i++){
            cout<<"\""<<convertDateFormat(table1["DATE"][i])<<"\",";
        }
        cout<<"};"<<endl;
        double cash=0;
        
        vector<vector<string> > cash_flow;
        vector<vector<string> > order_statictics1;
        vector<vector<string> > order_statictics2;
        for(int j=n-1;j<table1["DATE"].size();j++){
            

            
            double mean=0;
            double SD=0;
            
        
            for(int i=n-1;i>=0;i--){
                
                mean+=stod(table1["CLOSE"][j-i])-stod(table2["CLOSE"][j-i]);
                
            }

            double number_of_days=n;
            mean/=number_of_days;
            for(int i=n-1;i>=0;i--){
                
                SD+=((stod(table1["CLOSE"][j-i])-stod(table2["CLOSE"][j-i])-mean)*(stod(table1["CLOSE"][j-i])-stod(table2["CLOSE"][j-i])-mean));
                
            }
            
            if(daily_pay.size()>0){
                vector<vector<double > > temp;
                for(int i=0;i<daily_pay.size();i++){
                    double mean_prev=daily_pay[i][0];
                    double sd_prev=daily_pay[i][1];
                    double prev_z_score=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j])-mean_prev)/sd_prev;
                    if(prev_z_score>stop_loss_threshold){
                        if(holding>0){
                            holding--;
                            cash+=(-stod(table1["CLOSE"][j])+stod(table2["CLOSE"][j]));
                            vector<string> today_cash_flow;
                            string Date=convertDateFormat(table1["DATE"][j]);
                            today_cash_flow.push_back(Date);
                            today_cash_flow.push_back(to_string(cash));
                            cash_flow.push_back(today_cash_flow);
                            vector<string> today_statistics1;
                            Date=convertDateFormat(table1["DATE"][j]);
                            today_statistics1.push_back(Date);
                            today_statistics1.push_back("SELL");
                            today_statistics1.push_back("1");
                            order_statictics1.push_back(today_statistics1);
                            vector<string> today_statistics2;
                            Date=convertDateFormat(table1["DATE"][j]);
                            today_statistics2.push_back(Date);
                            today_statistics2.push_back("BUY");
                            today_statistics2.push_back("1");
                            order_statictics2.push_back(today_statistics2);
                            
                        }
                        else{
                            holding++;
                            cash+=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j]));
                            vector<string> today_cash_flow;
                            string Date=convertDateFormat(table1["DATE"][j]);
                            today_cash_flow.push_back(Date);
                            today_cash_flow.push_back(to_string(cash));
                            cash_flow.push_back(today_cash_flow);
                            vector<string> today_statistics1;
                            Date=convertDateFormat(table1["DATE"][j]);
                            today_statistics1.push_back(Date);
                            today_statistics1.push_back("SELL");
                            today_statistics1.push_back("1");
                            order_statictics1.push_back(today_statistics1);
                            vector<string> today_statistics2;
                            Date=convertDateFormat(table1["DATE"][j]);
                            today_statistics2.push_back(Date);
                            today_statistics2.push_back("BUY");
                            today_statistics2.push_back("1");
                            order_statictics2.push_back(today_statistics2);

                        }
                    }
                    else{
                        temp.push_back(daily_pay[i]);
                    }

                }
                daily_pay.clear();
                for(int i=0;i<temp.size();i++){
                    daily_pay.push_back(temp[i]);
                }

            }

            SD=sqrt(SD/number_of_days);
            double z_score=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j])-mean)/SD;
        
            if( z_score>threshold && holding<x){
                if(holding>0){
                    holding--;
                    cash+=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j]));
                    vector<string> today_cash_flow;
                    string Date=convertDateFormat(table1["DATE"][j]);
                    today_cash_flow.push_back(Date);
                    today_cash_flow.push_back(to_string(cash));
                    cash_flow.push_back(today_cash_flow);
                    vector<string> today_statistics1;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics1.push_back(Date);
                    today_statistics1.push_back("SELL");
                    today_statistics1.push_back("1");
                    order_statictics1.push_back(today_statistics1);
                    vector<string> today_statistics2;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics2.push_back(Date);
                    today_statistics2.push_back("BUY");
                    today_statistics2.push_back("1");
                    order_statictics2.push_back(today_statistics2);
                    vector<double> temp;
                    
                    if(daily_pay.size()>0){
                        vector<vector<double > > temp;
                        int r=0;
                        double val=-100000000.0;
                        for(int i=0;i<daily_pay.size();i++){
                            double mean_prev=daily_pay[i][0];
                            double sd_prev=daily_pay[i][1];
                            double prev_z_score=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j])-mean_prev)/sd_prev;
                            if(val<prev_z_score){
                                val=prev_z_score;
                                r=i;
                            }
                    

                        }
                        for(int i=0;i<daily_pay.size();i++){
                            if(i!=r){
                                temp.push_back(daily_pay[i]);
                            }
                        }
                        daily_pay.clear();
                        for(int i=0;i<temp.size();i++){
                            daily_pay.push_back(temp[i]);
                        }
                    }
                
                }
                else{
                    holding++;
                    cash+=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j]));
                    vector<string> today_cash_flow;
                    string Date=convertDateFormat(table1["DATE"][j]);
                    today_cash_flow.push_back(Date);
                    today_cash_flow.push_back(to_string(cash));
                    cash_flow.push_back(today_cash_flow);
                    vector<string> today_statistics1;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics1.push_back(Date);
                    today_statistics1.push_back("SELL");
                    today_statistics1.push_back("1");
                    order_statictics1.push_back(today_statistics1);
                    vector<string> today_statistics2;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics2.push_back(Date);
                    today_statistics2.push_back("BUY");
                    today_statistics2.push_back("1");
                    order_statictics2.push_back(today_statistics2);
                    vector<double> temp;
                    temp.push_back(mean);
                    temp.push_back(SD);
                    daily_pay.push_back(temp);

                    
                }

            }
            if( z_score<-threshold && holding>-x){
                if(holding<0){
                    holding++;
                    cash+=(-stod(table1["CLOSE"][j])+stod(table2["CLOSE"][j]));
                    vector<string> today_cash_flow;
                    string Date=convertDateFormat(table1["DATE"][j]);
                    today_cash_flow.push_back(Date);
                    today_cash_flow.push_back(to_string(cash));
                    cash_flow.push_back(today_cash_flow);
                    vector<string> today_statistics1;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics1.push_back(Date);
                    today_statistics1.push_back("BUY");
                    today_statistics1.push_back("1");
                    order_statictics1.push_back(today_statistics1);
                    vector<string> today_statistics2;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics2.push_back(Date);
                    today_statistics2.push_back("SELL");
                    today_statistics2.push_back("1");
                    order_statictics2.push_back(today_statistics2);
                    vector<double> temp;
                    
                    
                    if(daily_pay.size()>0){
                        vector<vector<double > > temp;
                        int r=0;
                        double val=-100000000.0;
                        for(int i=0;i<daily_pay.size();i++){
                            double mean_prev=daily_pay[i][0];
                            double sd_prev=daily_pay[i][1];
                            double prev_z_score=(stod(table1["CLOSE"][j])-stod(table2["CLOSE"][j])-mean_prev)/sd_prev;
                            if(val<prev_z_score){
                                val=prev_z_score;
                                r=i;
                            }
                    

                        }
                        for(int i=0;i<daily_pay.size();i++){
                            if(i!=r){
                                temp.push_back(daily_pay[i]);
                            }
                        }
                        daily_pay.clear();
                        for(int i=0;i<temp.size();i++){
                            daily_pay.push_back(temp[i]);
                        }
                    }

                }
                else{
                    holding--;
                    cash+=(-stod(table1["CLOSE"][j])+stod(table2["CLOSE"][j]));
                    vector<string> today_cash_flow;
                    string Date=convertDateFormat(table1["DATE"][j]);
                    today_cash_flow.push_back(Date);
                    today_cash_flow.push_back(to_string(cash));
                    cash_flow.push_back(today_cash_flow);
                    vector<string> today_statistics1;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics1.push_back(Date);
                    today_statistics1.push_back("BUY");
                    today_statistics1.push_back("1");
                    order_statictics1.push_back(today_statistics1);
                    vector<string> today_statistics2;
                    Date=convertDateFormat(table1["DATE"][j]);
                    today_statistics2.push_back(Date);
                    today_statistics2.push_back("SELL");
                    today_statistics2.push_back("1");
                    order_statictics2.push_back(today_statistics2);
                    vector<double> temp;
                    temp.push_back(mean);
                    temp.push_back(SD);
                    daily_pay.push_back(temp);
                    
                    

                }
                
            }

            

        }
        cash+=(stod(table2["CLOSE"][table2["CLOSE"].size()-1])-stod(table1["CLOSE"][table1["CLOSE"].size()-1]))*holding;
        writeCSV("order_statictics1.csv",order_statictics1);
        writeCSV("order_statictics2.csv",order_statictics2);
        writeCSV("daily_cashflow.csv",cash_flow);



    }
    remove((symbol1+ ".csv").c_str());
    remove((symbol2+ ".csv").c_str());
}
