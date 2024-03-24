strategy ?= NULL
symbol ?= NULL
symbol1 ?= NULL
symbol2 ?= NULL
n ?= -1
x ?= -1
p ?= -1
c1 ?= -1
c2 ?= -1
start_date ?= 00/00/0000
end_date ?= 31/12/9999
train_start_date ?= 00/00/0000
train_end_date ?= 31/12/9999
max_hold_days ?= 0
oversold_threshold ?= 0
overbought_threshold ?= 0
threshold ?= 0
adx_threshold ?= 0
stop_loss_threshold ?=-1 



run :
	
	g++ -c -std=c++17 ADX.cpp -o ADX.o
	g++ -c -std=c++17 BASIC.cpp -o BASIC.o
	g++ -c -std=c++17 DMA.cpp -o DMA.o
	g++ -c -std=c++17 DMA++.cpp -o DMA++.o
	g++ -c -std=c++17 MACD.cpp -o MACD.o
	g++ -c -std=c++17 RSI.cpp  -o RSI.o
	g++ -c -std=c++17 PAIRS.cpp -o pairs.o
	g++ -c -std=c++17 main.cpp -o main.o
	g++ -c -std=c++17 BEST_OF_ALL.cpp -o BEST_OF_ALL.o
	g++ -c -std=c++17 LinearRegression.cpp -o LinearRegression.o

	g++ -o myprogram main.o ADX.o BASIC.o DMA.o DMA++.o MACD.o RSI.o pairs.o LinearRegression.o BEST_OF_ALL.o
	./myprogram $< $(strategy) $(symbol) $(symbol1) $(symbol2) $(n) $(x) $(p) $(c1) $(c2) $(start_date) $(end_date) $(train_start_date) $(train_end_date) $(max_hold_days) $(oversold_threshold) $(overbought_threshold) $(threshold) $(adx_threshold) $(stop_loss_threshold)