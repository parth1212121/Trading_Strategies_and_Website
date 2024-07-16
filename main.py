import pandas as pd
from datetime import datetime
from dateutil.relativedelta import relativedelta
import time
import os
from jugaad_data.nse import stock_df
import matplotlib.pyplot as plt
import numpy as np
import argparse


def retrieve_stock_data(token_symbol, num_years):
    present_date = datetime.now().date()
    previous_date = present_date - relativedelta(years=num_years)
    stock_data = stock_df(symbol=token_symbol, from_date=previous_date, to_date=present_date, series="EQ")
    select = ['VALUE','LOW','DATE','LTP','HIGH','VOLUME','NO OF TRADES','CLOSE','OPEN']
    stock_data=stock_data[select]
    return stock_data


def file_time_size(stock_data, file_format, stock):
    file_path = stock+'.'+file_format
    
    start_time = time.time()
    
    if file_format == 'csv':
        stock_data.to_csv(file_path, index=False)
    elif file_format == 'txt':
        stock_data.to_csv(file_path, sep='\t', index=False)
    elif file_format == 'parquet':
        stock_data.to_parquet(file_path, index=False)
    elif file_format == 'pickle':
        stock_data.to_pickle(file_path)
    elif file_format == 'feather':
        stock_data.to_feather(file_path)

    
    end_time = time.time()
    time_taken = end_time - start_time

    # Get the size of the generated file
    file_size = os.path.getsize(file_path) / (1024 * 1024) 
    return time_taken, file_size


def plt_graph_data_writing_comparison(file_formats, time_taken_list, file_size_list, stock,x):
    
    bar_width = 0.35

    index = np.arange(len(file_formats))
    

    fig, ax1 = plt.subplots(figsize=(12, 8))
    fig.set_size_inches(12, 8)
    color = 'tab:green'
    ax1.set_xlabel('File Format')
    ax1.set_ylabel('Time Taken (seconds)', color=color)
    ax1.bar(index - bar_width/2, time_taken_list, bar_width, label='Time Taken', color=color)
    ax1.tick_params(axis='y', labelcolor=color)
    
    
    ax2 = ax1.twinx()
    color = 'tab:red'
    ax2.set_ylabel('File Size (MB)', color=color)
    ax2.bar(index + bar_width/2, file_size_list, bar_width, label='File Size', color=color, alpha=0.7)
    ax2.tick_params(axis='y', labelcolor=color)
    ax1_legend = ax1.legend(loc='upper left', bbox_to_anchor=(0, 1), fontsize='medium')
    ax2_legend = ax2.legend(loc='upper right', bbox_to_anchor=(0, 0.85), fontsize='medium')


    fig.tight_layout()
    plt.title('Comparison of data writing in various File Formats [ Stock='+stock+', Num_years='+ str(x)+']')
    plt.xticks(index, file_formats)  # Adjust x-ticks
    
    plt.subplots_adjust(right=0.89)
    plt.subplots_adjust(top=0.95)        
    plt.legend()    
       
    plt.savefig(stock+'.png',bbox_inches='tight')
    plt.show()
    
    
    
def main():   

    parser = argparse.ArgumentParser(description='Generate data')
    parser.add_argument('--token_symbol', type=str, required=True)
    parser.add_argument('--num_years', type=int,  required=True)

    args = parser.parse_args()

    x = args.num_years
    stock = args.token_symbol

    dataframe = retrieve_stock_data(stock,x)

    file_formats = ['csv', 'txt', 'parquet', 'feather','pickle']

    time_taken_list = []
    file_size_list = []

    for file_format in file_formats:
    
        time_taken,file_size=file_time_size(dataframe,file_format,stock) 	    
        time_taken_list.append(time_taken)
        file_size_list.append(file_size)
        
    plt_graph_data_writing_comparison(file_formats, time_taken_list, file_size_list, stock,x)
        
    return 

if __name__ == "__main__":
    main()
