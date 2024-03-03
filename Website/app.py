from flask import Flask, render_template, request, redirect, url_for, flash, session
from flask_sqlalchemy import SQLAlchemy
from werkzeug.security import generate_password_hash, check_password_hash
import pandas as pd
from datetime import datetime, timedelta
from dateutil.relativedelta import relativedelta
import time
from jugaad_data.nse import stock_df
import numpy as np
from flask import Flask, request,render_template, redirect, jsonify
import random
import finnhub

from filters import (
    Volume_Filter,
    Price_filter,
    Range_52W_filter,
    Value_filter,
    Average_Price_filter
)
from utils import (
    news_list,
    retrieve_stock_data,
    TRENDING_HIGH,
    news,
)
from indicators import (
    track_rolling_mean,
    track_ewm,
    track_RSI,
    track_BB,
    track_ATR
)


UserName="DEFAULT"
History=""
Profit="NONE"


app = Flask(__name__)
app.secret_key = 'your_secret_key'  # Replace with your actual secret key

# Database Configuration
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///users.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

# User Model
class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(100), unique=True, nullable=False)
    password_hash = db.Column(db.String(200), nullable=False)
    

# Initialize Database within Application Context
with app.app_context():
    db.create_all()

@app.route('/')
def index():
    return render_template('login_page.html')

@app.route('/register', methods=['GET', 'POST'])
def register():

    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        hashed_password = generate_password_hash(password, method='pbkdf2:sha256')

        new_user = User(username=username, password_hash=hashed_password)
        db.session.add(new_user)
        db.session.commit()
        
        flash('Registration successful! Please login.')
        return render_template('login_page.html')

    return render_template('register_page.html')

@app.route('/login', methods=['POST'])
def login():
    username = request.form['username']
    password = request.form['password']
    user = User.query.filter_by(username=username).first()

    if user and check_password_hash(user.password_hash, password):
        session['user_id'] = user.id
        session['username'] = user.username
       
        global UserName
        UserName=user.username
        return render_template('home_page.html',TrendingData=TRENDING_HIGH(),NEWS=news())
        return render_template('home_page.html')
        
    else:
        flash('Invalid username or password')
        return redirect(url_for('index'))

@app.route('/dashboard')
def dashboard():
    if 'user_id' in session:
        return render_template('welcome.html', username=session['username'])
    else:
        return redirect(url_for('index'))

@app.route('/logout')
def logout():
    session.pop('user_id', None)
    session.pop('username', None)
    return redirect(url_for('index'))

@app.route('/home')
def home_page():
    if 'user_id' in session:
        return render_template('home_page.html',TrendingData=TRENDING_HIGH(),NEWS=news())
    else:
        return redirect(url_for('index'))


@app.route('/compare',methods = ['GET', 'POST','MULTIPEL'])
def compare_page():
    if 'user_id' in session:
        
    
        if request.method=='MULTIPEL':
            
            stock_name = request.form['stock_name']
            no_of_years = int(request.form['no_of_years'])
            column_name = request.form['column_name']
            try:
                db= retrieve_stock_data(stock_name,int(no_of_years))
                
        
                x_values = db["DATE"].tolist()  
                y_values = db[column_name].tolist()
        
                # Return the data as JSON
                return jsonify({'x_values': x_values, 'y_values': y_values})
            except:
                db = retrieve_stock_data("SBIN",int(no_of_years))
                x_values = db["DATE"].tolist()  
                y_values = db[column_name].tolist()
        
                # Return the data as JSON
                return jsonify({'x_values': x_values, 'y_values': y_values})
                
        return render_template('compare_page.html') 
        
    else:
        return redirect(url_for('index'))

@app.route('/profile',methods = ['GET', 'POST'])
def profile_page():
    if 'user_id' in session:
        global History
        global UserName
        global Profit
        History="<br><br><br>"+History
       
        return render_template('profile_page.html',str=UserName,history=History,Profit=Profit)
    else:
        return redirect(url_for('index'))
        
@app.route('/login',methods = ['GET', 'POST'])
def login_page():
    return render_template('login_page.html')

@app.route('/filter',methods = ['VOLUME','GET','POST'])
def filter_page():

    if 'user_id' in session:
        if request.method=='POST':

            action = request.form['action']

            if (action=="compute_volume"):
            
                threshold=request.form['volume_threshold_name']           
                s=Volume_Filter(int(threshold))           
                h="VOLUME FILTER : Threshold="
                h+=str(threshold  )

                return render_template('filter_page.html',filter_data=s,HEDAING=h)
        
            elif (action=="compute_price"):
            
                threshold=request.form['price_threshold_name']            
                s=Price_filter(int(threshold))  
                h="PRICE FILTER : Threshold="
                h+=str(threshold  )       
                return render_template('filter_page.html',filter_data=s,HEDAING=h)

            elif (action=="compute_range_52W"):
            
                threshold_minimum=request.form['range_52W_minimum_name']     
                threshold_maximum=request.form['range_52W_maximum_name'] 
                h="RANGE 52W FILTER : Minimum="
                h+=str(threshold_minimum)  +" Maximum"+str(threshold_maximum)          
                s=Range_52W_filter(int(threshold_minimum),int(threshold_maximum))        
                return render_template('filter_page.html',filter_data=s,HEDAING=h)

            elif (action=="compute_Value"):
            
                threshold=request.form['Value_threshold_name']            
                s=Value_filter(int(threshold)) 
                h="VALUE FILTER : Threshold="
                h+=str(threshold  )         
                return render_template('filter_page.html',filter_data=s,HEDAING=h)

            elif (action=="compute_Average_Price"):
            
                threshold=request.form['Average_Price_threshold_name']            
                s=Average_Price_filter(int(threshold))  
                h="AVERAGE PRICE FILTER : Threshold="
                h+=str(threshold  )        
                return render_template('filter_page.html',filter_data=s,HEDAING=h)
    
        return render_template('filter_page.html')

    else:
        return redirect(url_for('index'))

@app.route('/stocks', methods=['GET','POST','TRACK_RSI','ROLLING_MEAN','TRACK_EWM','TRACK_BB2','TRACK_BB1','TRACK_ATR'])
def stock_page():
    if 'user_id' in session:
        if request.method == 'POST':
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
    
            db= retrieve_stock_data(stock_name,int(no_of_years))
            
    
            x_values = db['DATE'].tolist()  
            y_values = db[column_name].tolist()
    
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        elif request.method == 'TRACK_RSI':
            
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
            
            db= track_RSI(stock_name,int(no_of_years))
            
    
            x_values = db['X'].tolist()  
            y_values = db['Y'].tolist()
            
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        elif request.method == 'ROLLING_MEAN':
            
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
            
            db= track_rolling_mean(stock_name,int(no_of_years))
            
    
            x_values = db['DATE'].tolist()  
            y_values = db['CP'].tolist()
            
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        elif request.method == 'TRACK_EWM':
            
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
            
            db= track_ewm(stock_name,int(no_of_years))
            
    
            x_values = db['X'].tolist()  
            y_values = db['Y'].tolist()
            
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        elif request.method == 'TRACK_BB1':
            
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
            db= track_BB(stock_name,int(no_of_years))
            
    
            x_values = db['X'].tolist()  
            y_values = db['Y1'].tolist()
            
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        elif request.method == 'TRACK_BB2':
            
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
            
            db= track_BB(stock_name,int(no_of_years))
            
    
            x_values = db['X'].tolist()  
            y_values = db['Y2'].tolist()
            
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        elif request.method == 'TRACK_ATR':
            
            stock_name = request.form['stock_name']
            no_of_years = request.form['no_of_years']
            column_name = request.form['column_name']
            
            db= track_ATR(stock_name,int(no_of_years))
            
    
            x_values = db['X'].tolist()  
            y_values = db['Y'].tolist()
            
            # Return the data as JSON
            return jsonify({'x_values': x_values, 'y_values': y_values})
        return render_template('stock_page.html')
    else:
        return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(debug=True)

app.run()
