#include "ADX.h"
#include "BASIC.h"
#include "DMA.h"
#include "DMA++.h"
#include "MACD.h"
#include "RSI.h"
#include "LinearRegression.h"
#include "PAIRS.h"
#include "BEST_OF_ALL.h"

#include <iostream>
using namespace std;


int main( int argc , char* files[] ) {

    string strategy                     = files[1] ;
    string symbol                       = files[2] ;
    string symbol1                      = files[3] ;
    string symbol2                      = files[4] ;
    string String_n                     = files[5] ;
    string String_x                     = files[6] ;
    string String_p                     = files[7] ;
    string String_c1                    = files[8] ;
    string String_c2                    = files[9] ;
    string start_date                   = files[10];
    string end_date                     = files[11];
    string train_start_date             = files[12];
    string train_end_date               = files[13];
    string String_max_hold_days         = files[14];
    string String_oversold_threshold    = files[15];
    string String_overbought_threshold  = files[16];
    string String_threshold             = files[17];
    string String_adx_threshold         = files[18];
    string String_stop_loss_threshold   = files[19];
    int n , x ,max_hold_days ;
    n=stoi(String_n);
    x=stoi(String_x);
    max_hold_days =stoi(String_max_hold_days );
  
    double oversold_threshold , overbought_threshold ,p,  threshold , adx_threshold , stop_loss_threshold , c1 , c2 ;
    p=stod(String_p);
    oversold_threshold=stod(String_oversold_threshold);
    overbought_threshold=stod(String_overbought_threshold);
    threshold=stod(String_threshold);
    adx_threshold=stod(String_adx_threshold);
    stop_loss_threshold =stod(String_stop_loss_threshold );
    c1=stod(String_c1);
    c2=stod(String_c2);
    cout<<strategy<<endl;

    if ( strategy == "ADX" ){

        ADX( symbol , n , x , adx_threshold , start_date , end_date ) ;
    }
    else if ( strategy == "BASIC" ){

        BASIC( symbol , n , x , start_date , end_date ) ;
    }
    else if ( strategy == "DMA" ){

        DMA( symbol, n , x , p , start_date , end_date) ;
    }
    else if ( strategy == "DMA++" ){

        DMAPP(symbol, n, x , p , max_hold_days , c1 , c2 , start_date , end_date ) ;
    }
    else if ( strategy == "LINEAR_REGRESSION" ){
        
        LinearRegeression( symbol , x , p ,train_start_date , train_end_date , start_date , end_date ,true) ;
    }

    else if ( strategy == "MACD" ){

        MACD( symbol , x , start_date , end_date ) ;
    }
    else if ( strategy == "RSI" ){

        RSI( symbol , n , x , oversold_threshold , overbought_threshold , start_date, end_date ) ;
    }
    else if ( strategy == "BEST_OF_ALL" ){
        
        bestOfAll(symbol,start_date,end_date) ;
    }
    else if( strategy =="PAIRS"){

        PAIRS(symbol1, symbol2 , n , x , start_date , end_date , threshold , stop_loss_threshold );
    }
    else {
        cout<<" Invalid Strategy Name " <<endl;
    }

    return 0;
}   