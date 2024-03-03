var stock_names=[]
var noOfYears =31;
var now="one_month"
var columnName="OPEN"
var CN="OPEN"
function setTime(V){
    let month = document.getElementById(now);
    month.style.backgroundColor="#CA9849";
    month.style.color="#ffffff"
    if(V==7){
        now="one_week"
    }
    if(V==31){
        now="one_month"
    }
    if(V==93){
        now="three_month"
    }
    if(V==186){
        now="six_month"
    }
    if(V==365){
        now="one_year"
    }
    if(V==1825){
        now="five_year"
    }
    if(V==3650){
        now="max"
    }
    let month_now = document.getElementById(now);
    month_now.style.backgroundColor="#1F1F1F";
    month_now.style.color="#CA9849"
    
    noOfYears =V;
    generateGraph();
    
}
function setcolumn(V){
    let columnname = document.getElementById(CN);
    columnname.style.backgroundColor="#CA9849";
    columnname.style.color="#ffffff"
    CN=V
    columnName=V;
    let columnname1 = document.getElementById(CN);
    columnname1.style.backgroundColor="#1F1F1F";
    columnname1.style.color="#CA9849"
    if(columnName=="NO_OF_TRADES"){
        columnName="NO OF TRADES"
    }
    generateGraph();
}

function remove_stock_name(V){
    var d=document.getElementById(V);
    d.parentNode.removeChild(d);
    var temp_stock_names=[]
    for(var i=0;i<stock_names.length;i++){
        if(stock_names[i]!=V){
            temp_stock_names.push(stock_names[i])
        }
    }
    stock_names=[]
    stock_names=temp_stock_names
    generateGraph()

    
}
function add_stock_name(){
    
    var stock_Name=document.getElementById("stock_name").value
    var found=false;
    for(var i=0;i<stock_names.length;i++){
        if(stock_names[i]==stock_Name){
            found=true
        }
    }

    if(stock_Name=="" || found){
        return;
    }
    stock_names.push(stock_Name)
    var b=document.createElement('button');
    b.style.position="relative";
    b.style.top="0px"
    b.style.width="auto";
    b.style.height="auto"
    b.style.fontSize="22px"
    b.style.borderRadius="10px"

    b.style.border="1px solid"
    b.style.padding="10px"

    b.style.background="#CA9849";
    b.onclick=function(){
        remove_stock_name(stock_Name);
    }
    b.id=stock_Name

    b.textContent=stock_Name;

    var show_posn=document.getElementById("show_names")

    show_posn.appendChild(b)
    generateGraph()
}
function generateGraph() {
    
    
    
    var numStocks = stock_names.length; // Number of stocks to plot, change this as needed
    
    var datasets = []; // Array to store datasets for each stock

    // Make AJAX requests for each stock
    for (var i = 0; i <= numStocks-1; i++) {
        var stockName = stock_names[i];
        console.log(stockName)
        // var stockName = document.getElementById('stock' + i + '_name').value;
        makeAjaxRequest(stockName, noOfYears, columnName, function(responseData, stockName) {
            // Callback function to handle response and create dataset
            datasets.push({
                label: stockName ,
                data: responseData.y_values,
                borderColor: getRandomColor(),
                borderWidth: 2,
                pointRadius: 0, // Set pointRadius to 0 to hide points
                fill: false
            });

            // Check if all AJAX requests are complete
            if (datasets.length === numStocks) {
                // Access the canvas element
               
                var ctx = document.getElementById("myChart").getContext('2d');

                // Destroy the existing chart instance if it exists
                var existingChart = ctx.chart;
                if (existingChart) {
                    existingChart.destroy();
                }

                // Initialize the new chart for all stocks
                var newChart = new Chart(ctx, {
                    type: 'line',
                    data: {
                        labels: responseData.x_values,
                        datasets: datasets
                    },
                    options: {
                        scales: {
                            x: [{
                                type: 'linear',
                                position: 'bottom',
                                scaleLabel: {
                                    display: true,
                                    labelString: 'Date'
                                }
                            }],
                            y: [{
                                scaleLabel: {
                                    display: true,
                                    labelString: 'Column'
                                }
                            }]
                        },
                        legend: {
                            display: true,
                            position: 'bottom'
                        },
                        layout: {
                            padding: {
                                left: 10,
                                right: 10,
                                top: 10,
                                bottom: 10
                            }
                        },
                        responsive: true,
                        maintainAspectRatio: true,
                    }
                });
                
                // Store the new chart instance in the canvas element
                ctx.chart = newChart;
            }
        });
    }
}

// Function to make AJAX request for a single stock
function makeAjaxRequest(stockName, noOfYears, columnName, callback) {
    var xhr = new XMLHttpRequest();
    xhr.open('MULTIPEL', '/compare', true);
    xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr.onreadystatechange = function () {
        if (xhr.readyState == 4 && xhr.status == 200) {
            var responseData = JSON.parse(xhr.responseText);
            callback(responseData, stockName);
        }
    };
    xhr.send('stock_name=' + stockName + '&no_of_years=' + noOfYears + '&column_name=' + columnName);
}

// Function to generate random color
function getRandomColor() {
    var letters = '0123456789ABCDEF';
    var color = '#';
    for (var i = 0; i < 6; i++) {
        color += letters[Math.floor(Math.random() * 16)];
    }
    return color;
}