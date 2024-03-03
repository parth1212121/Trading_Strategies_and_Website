import pandas as pd
from datetime import datetime, timedelta
from dateutil.relativedelta import relativedelta
import time
from jugaad_data.nse import stock_df
import numpy as np


data_base1=pd.read_csv('ind_nifty50list.csv')
nifty_stocks=data_base1['Symbol']


def Volume_Filter(threshold):
    try:

        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        vol={}
        for stock in nifty_stocks:
        
            test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
            volume=test_data['VOLUME']
            #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            if(volume[0]>threshold):
                vol[stock]=volume[0] 
        
        sorted_stocks = (sorted(vol.items(), key=lambda item: item[1], reverse=True))   
        s=""
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s 
    except:
        today = datetime.now().date()
        yesterday = today - timedelta(days=4)
        vol={}
        for stock in nifty_stocks:
        
            test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
            volume=test_data['VOLUME']
            #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            if(volume[0]>threshold):
                vol[stock]=volume[0] 
        
        sorted_stocks = (sorted(vol.items(), key=lambda item: item[1], reverse=True))   
        s=""
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s
         # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).
     
def Price_filter(threshold):
    try:
        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        clp={}
        for stock in nifty_stocks:
         
            test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
            close_price=test_data['CLOSE']
            #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            if(close_price[0]>threshold):
                clp[stock]=close_price[0]  

        sorted_stocks = (sorted(clp.items(), key=lambda item: item[1], reverse=True))     
        s=""
        
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).  
    except:
        today = datetime.now().date()
        yesterday = today - timedelta(days=4)
        clp={}
        for stock in nifty_stocks:
            try :
                test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
                close_price=test_data['CLOSE']
                #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
                if(close_price[0]>threshold):
                    clp[stock]=close_price[0]  

            except:
                continue
        sorted_stocks = (sorted(clp.items(), key=lambda item: item[1], reverse=True))     
        s=""
        
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s


def Range_52W_filter(minimum,maximum):
    try:
        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        dic={}
        for stock in nifty_stocks:
            
            test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
            high=test_data['52W H']
            low=test_data['52W L']
            #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            if(high[0]<=maximum and low[0]>=minimum):
                dic[stock]=(high[0],low[0]) 
            
        sorted_stocks = (sorted(dic.items(), key=lambda item: item[1], reverse=True))     
        s=""
        s+="<table><tr><th style='width:200px'>"+"STOCKS"+"<th style='width:200px'>"+"52W HIGH"+"<th style='width:200px'>"+"52W LOW"+"<th>"
        s+="</tr></table><br>"
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        l=Sort(l)[::-1]
        r=len(l)-len(l)%2
        for i in range(0,(len(l)),3):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1][0],3))+"<th style='width:200px'>"+str(round(data_stock1[1][1],3))
            
        
            s+="</tr></table><br>"   
        return   s   # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change). 
    except:
        print("failed 52W")
        today = datetime.now().date()
        yesterday = today - timedelta(days=4)
        clp={}
        for stock in nifty_stocks:
            try :
                test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
                high=test_data['52W H']
                low=test_data['52W L']
                #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
                if(high[0]<=maximum and low[0]>=minimum):
                    dic[stock]=(high[0],low[0])   

            except:
                continue
        sorted_stocks = (sorted(clp.items(), key=lambda item: item[1], reverse=True))     
        s=""
        s+="<tr><th>"+"STOCKS"+"<th>"+"52W HIGH"+"<th>"+"52W LOW"+"<th>"
        s+="</tr>"
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        l=Sort(l)[::-1]
        r=len(l)-len(l)%2
        for i in range(0,(len(l)),3):
            data_stock1=l[i]
            

            s+="<tr><th>"+str(data_stock1[0])+"<th>"+str(round(data_stock1[1][0],3))+"<th>"+str(round(data_stock1[1][1],3))
            
        
            s+="</tr>"   
        return   s 

def Value_filter(threshold):    # REpresented in scientific notatipn in my PC , therfore cannot check. (Pretty high values)
    try:
        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        dic={}
        for stock in nifty_stocks:
            

            test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
            value=test_data['VALUE']
            #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            if(value[0]>threshold):
                dic[stock]=value[0]  
            
        sorted_stocks = (sorted(dic.items(), key=lambda item: item[1], reverse=True))     
        s=""
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).
    except:
        today = datetime.now().date()
        yesterday = today - timedelta(days=4)
        dic={}
        for stock in nifty_stocks:
            try:

                test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
                value=test_data['VALUE']
                #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
                if(value[0]>threshold):
                    dic[stock]=value[0]  
            except:
                continue
        sorted_stocks = (sorted(dic.items(), key=lambda item: item[1], reverse=True))     
        s=""
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).
          

def Average_Price_filter(threshold):
    try:
        today = datetime.now().date()
        yesterday = today - timedelta(days=1)
        dic={}
        for stock in nifty_stocks:
            
            test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
            avg_p=test_data['VWAP']
            #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
            if(avg_p[0]>threshold):
                dic[stock]=avg_p[0]  
            
        sorted_stocks = (sorted(dic.items(), key=lambda item: item[1], reverse=True))     

        s=""
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s  # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).
    except:
        today = datetime.now().date()
        yesterday = today - timedelta(days=4)
        dic={}
        for stock in nifty_stocks:
            try:
                test_data = stock_df(symbol=stock, from_date=yesterday, to_date=today, series="EQ")
                avg_p=test_data['VWAP']
                #daily_return=((cl_price[0]-cl_price[1])/cl_price[1])*100
                if(avg_p[0]>threshold):
                    dic[stock]=avg_p[0]  
            except:
                continue
        sorted_stocks = (sorted(dic.items(), key=lambda item: item[1], reverse=True))     

        s=""
        l=sorted_stocks
        def Sort(sub_li):
            sub_li.sort(key = lambda x: x[1])
            return sub_li
        for i in range(0,len(l),1):
            data_stock1=l[i]
            

            s+="<table><tr><th style='width:200px'>"+str(data_stock1[0])+"<th style='width:200px'>"+str(round(data_stock1[1],3))
            
        
            s+="</tr></table><br>"   
        return   s   # RETURNS A SORTED LIST OF TUPLES WITH (Stock, percentage_change).

      