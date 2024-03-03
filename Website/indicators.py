import pandas as pd
from datetime import datetime, timedelta
from dateutil.relativedelta import relativedelta
import time
from jugaad_data.nse import stock_df
import numpy as np


def track_rolling_mean(token_symbol, num_days):
    db = pd.DataFrame()
    present_date = datetime.now().date()
    previous_date = present_date - relativedelta(days=num_days) # can be changed..
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")
    stock_data = stock_data[::-1]
    select = ['DATE','CLOSE']
    stock_data=stock_data[select]
    dates=stock_data['DATE']
    cl_price=stock_data['CLOSE']
    daily_return=cl_price.pct_change()
    df=daily_return.rolling(window=10,min_periods=10).mean()

    db["DATE"]=dates[10:]
    db["CP"]=df[10:]
    return db                 # REturn a tuple (df, dates)   [The first  9 value in df are "nan"]
                              # If BT in plotting then remove the first 9 values.....
    
def track_ewm(token_symbol, num_days):

    db = pd.DataFrame()
    present_date = datetime.now().date()
    previous_date = present_date - relativedelta(days=num_days) # can be changed..
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")
    stock_data = stock_data[::-1]
    select = ['DATE','CLOSE']
    stock_data=stock_data[select]
    dates=stock_data['DATE']
    cl_price=stock_data['CLOSE']
    daily_return=cl_price.pct_change()
    df=daily_return.ewm(com=10,min_periods=10).mean()
    db["X"]=dates[10:]
    db["Y"]=df[10:]
    return db           #   [The first 9 value in df are "nan"]

def RSI(DF,n=14):      # HELPER FUNCTION
    df=DF.copy()
    df["change"] = df["CLOSE"]-df["CLOSE"].shift(1)
    df["gain"]=np.where(df["change"]>=0,df["change"],0)
    df["loss"]=np.where(df["change"]<0,(-1)*df["change"],0)
    df["avg_gain"]=df["gain"].ewm(alpha=1/n,min_periods=n).mean()
    df["avg_loss"]=df["loss"].ewm(alpha=1/n,min_periods=n).mean()
    df["rs"]=df["avg_gain"]/df["avg_loss"]
    df["rsi"]=(100-(100/(1+df["rs"])))
    return df["rsi"]

def track_RSI(token_symbol, num_days):
    db = pd.DataFrame()
    present_date = datetime.now().date()
    previous_date = present_date - relativedelta(days=num_days) # can be changed..
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")
    stock_data = stock_data[::-1]
    dates=stock_data['DATE']
    ans=RSI(stock_data)
    db["X"]=dates[14:]
    db["Y"]=ans[14:]
    return db 


def BB(DF,n=14):      # HELPER FUNCTION
    
    df=DF.copy()
    df["MB"]=df["CLOSE"].rolling(n).mean()
    df["UB"]=df["MB"]+(2*(df["CLOSE"].rolling(n).std(ddof=0)))      # Better to use (N-1) than N
    df["LB"]=df["MB"]-(2*(df["CLOSE"].rolling(n).std(ddof=0)))      # Better to use (N-1) than N
    df["BB_Width"]=df["UB"]-df["LB"]
    return df[["MB","UB","LB","BB_Width"]]

def track_BB(token_symbol, num_days):
    
    db = pd.DataFrame()
    present_date = datetime.now().date()

    previous_date = present_date - relativedelta(days=num_days) # can be changed..
 
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")

    stock_data = stock_data[::-1]
    dates=stock_data['DATE']

    ans=BB(stock_data)
    

    db["X"]=dates[14:]
    db["Y1"]=ans["UB"][14:]
    db["Y2"]= ans["LB"][14:]   # Both Y1, Y2 need to be plotted

    return db           
                               # with the stocks graph on need to plot "UB" and "LB" as upper and lower Bollinger Bands.
                              #   [The first 14 values in df are "nan"]                                     
                                
def ATR(DF,n=14):      # HELPER FUNCTION
    
    df=DF.copy()
    df["H-L"]=df["HIGH"]-df["LOW"]
    df["H-PC"]=abs(df["HIGH"]-df["CLOSE"].shift(1))
    df["L-PC"]=abs(df["LOW"]-df["CLOSE"].shift(1))
    df["TR"]=df[["H-L","H-PC","L-PC"]].max(axis=1,skipna=False)
    df["ATR"]=df["TR"].ewm(span=n,min_periods=n).mean()
    return df["ATR"]                               
               
def track_ATR(token_symbol, num_days):
    db = pd.DataFrame()
    present_date = datetime.now().date()
    previous_date = present_date - relativedelta(days=num_days) # can be changed..
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")
    stock_data = stock_data[::-1]
    dates=stock_data['DATE']
    ans=ATR(stock_data)
    db["X"]=dates[14:]
    db["Y"]=ans[14:]
    return db  
             