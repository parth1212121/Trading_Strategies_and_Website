import pandas as pd
from datetime import datetime, timedelta
from dateutil.relativedelta import relativedelta
import time
from jugaad_data.nse import stock_df
import numpy as np
import random
import finnhub

data_base1=pd.read_csv('ind_nifty50list.csv')
nifty_stocks=data_base1['Symbol']


def news_list(api):
    finnhub_client = finnhub.Client(api_key=api)
    db = finnhub_client.general_news('general', min_id=0)
    return db


def retrieve_stock_data(token_symbol, num_days):
    present_date = datetime.now().date()
    previous_date = present_date - relativedelta(days=num_days)
    
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")

    for date in stock_data['DATE']:
        date = date.strftime('%Y-%m-%d')        #Didn't worked...
    stock_data = stock_data[::-1]
    return stock_data


data_base1=pd.read_csv('ind_nifty50list.csv')
nifty_stocks=data_base1['Symbol']


# ----->  TRENDING  <-------
def TRENDING_HIGH(): 
    try:  
        percent_change={}
        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        before_yesterday = today - timedelta(days=4)  # MARKET WAS CLOSE ON MONDAY,SUNDAY,SATURDAY (otherwise should be "=2").
        for stock in nifty_stocks:
            test_data = stock_df(symbol=stock, from_date=before_yesterday, to_date=yesterday, series="EQ")
            cl_price=test_data['CLOSE']
            daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            percent_change[stock]=daily_return
        sorted_stocks = (sorted(percent_change.items(), key=lambda item: item[1], reverse=True)) 
        s=""

        l=sorted_stocks
        random.shuffle(l)
        
        for i in range(0,min(int(len(l)),44),2):
            data_stock=l[i]

            s+="<tr><th>"+str(data_stock[0])+"<th>"+str(round(data_stock[1],3))+"<th>"

            if(round(data_stock[1],3)<0):
                s+="<img height='10' width='10' src='/static/img/down.png' >"
            else:
                s+="<img height='10' width='10' src='/static/img/up.png' >"
            s+="<th>   <th>"
            data_stock=l[i+1]
            s+="<th>"+str(data_stock[0])+"<th>"+str(round(data_stock[1],3))+"<th>"

            if(round(data_stock[1],3)<0):
                s+="<img height='10' width='10' src='/static/img/down.png' >"
            else:
                s+="<img height='10' width='10' src='/static/img/up.png' >"

            s+="</tr>"   
        return   s 
    except :
        percent_change={}
        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        before_yesterday = today - timedelta(days=7)  # MARKET WAS CLOSE ON MONDAY,SUNDAY,SATURDAY (otherwise should be "=2").
        for stock in nifty_stocks:
            test_data = stock_df(symbol=stock, from_date=before_yesterday, to_date=yesterday, series="EQ")
            cl_price=test_data['CLOSE']
            daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            percent_change[stock]=daily_return
        sorted_stocks = (sorted(percent_change.items(), key=lambda item: item[1], reverse=True)) 
        s=""

        l=sorted_stocks
        random.shuffle(l)
        
        for i in range(0,min(int(len(l)),44),2):
            data_stock=l[i]

            s+="<tr><th>"+str(data_stock[0])+"<th>"+str(round(data_stock[1],3))+"<th>"

            if(round(data_stock[1],3)<0):
                s+="<img height='10' width='10' src='/static/img/down.png' >"
            else:
                s+="<img height='10' width='10' src='/static/img/up.png' >"
            s+="<th>   <th>"
            data_stock=l[i+1]
            s+="<th>"+str(data_stock[0])+"<th>"+str(round(data_stock[1],3))+"<th>"

            if(round(data_stock[1],3)<0):
                s+="<img height='10' width='10' src='/static/img/down.png' >"
            else:
                s+="<img height='10' width='10' src='/static/img/up.png' >"

            s+="</tr>"   
        return   s 
         # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).

def news():
    ne=""
    db=news_list("cmp42f1r01qjn678ebo0cmp42f1r01qjn678ebog")
    i=0
    j=0
    while(i<7):
        p=db[i]
        head=""
        for k in range(0,len(p['headline'])):
            if((k+1)%50==0):
                head+="<br>"
            head+=p['headline'][k]
        head+=" "
        ne+="<tr><td width :'100px' word-wrap:'break-word'>"+head+"<a style='text-decoration:none' href="+p['url']+">CLick Here</a><td><img  width='100' height='100' src='"+p['image']+"'><td></tr>"
        i+=1
        j+=1
    
   
    return ne
