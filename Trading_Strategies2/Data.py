from jugaad_data.nse import stock_df
from datetime import datetime, timedelta
import os
import sys

def getData1( Start_date , End_date , SYMBOL ,n):

    start_date= datetime.strptime(Start_date, "%d/%m/%Y")
    start_date=start_date-timedelta(days=int(n)+int(n))
    
    end_date= datetime.strptime(End_date, "%d/%m/%Y")
    while(True):
        try:
        
            df = stock_df( symbol = SYMBOL , from_date = start_date , to_date = end_date, series = "EQ" )
            break
        except:
            start_date=start_date+timedelta(days=1)
            print("ERROR in Duration")
  


    now=len(df["DATE"])-1
    
    while(df["DATE"][now]<datetime.strptime(Start_date, "%d/%m/%Y")):
       
        now-=1
    
    df=df[:now+int(n)]
    
    df=df[::-1]
    df.to_csv( SYMBOL + '.csv' , index = False , sep = ',' )





if __name__ == "__main__":

    n=sys.argv[4]
    end_date = sys.argv[3]
    start_date = sys.argv[2]
    symbol = sys.argv[1]
    
    getData1( start_date, end_date , symbol ,n)
