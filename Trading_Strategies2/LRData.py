from jugaad_data.nse import stock_df
from datetime import datetime, timedelta
import os
import sys

def getData1( Startegy, Start_date , End_date , SYMBOL ,n):

    start_date= datetime.strptime(Start_date, "%d/%m/%Y")
    prev_start_date=start_date-timedelta(days=int(n)+int(n))
    
    end_date= datetime.strptime(End_date, "%d/%m/%Y")
    try:
        df = stock_df( symbol = SYMBOL , from_date = prev_start_date , to_date = end_date, series = "EQ" )
    except:

        print("ERROR in Duration")
  
    now=len(df["DATE"])-1
    
    while(df["DATE"][now]<datetime.strptime(Start_date, "%d/%m/%Y")):
       
        now-=1
    
    df=df[:now+int(n)]
    
   
    df=df[::-1]
    df.to_csv( SYMBOL +'_'+ Startegy+ '.csv' , index = False , sep = ',' )





if __name__ == "__main__":

    Startegy=sys.argv[5]
    n=sys.argv[4]
    end_date = sys.argv[3]
    start_date = sys.argv[2]
    symbol = sys.argv[1]
    
    getData1( Startegy, start_date, end_date , symbol ,n)



