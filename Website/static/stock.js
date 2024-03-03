
var noOfYears =31
var now="one_month"
var columnName = "HIGH"
var CN="HIGH"
var indi="NO"
function setcolumn(X){
    let columnname = document.getElementById(CN);
    columnname.style.backgroundColor="#CA9849";
    columnname.style.color="#ffffff"
    CN=X
    if(CN=="NO_OF_TRADES"){
        columnName="NO OF TRADES"
    }
    else{
        columnName=CN
    }
    if(indi=="TRACK_BB"){
        console.log(noOfYears);
        generateGraph2(-1);
    }
    else{
        generateGraph(-1);
    }
    
}

function setTime(V){
    if(indi=="TRACK_BB"){
        
        generateGraph2(V);
    }
    else{
        generateGraph(V);
    }
}

function setIndicator(){
    let indicator=document.getElementById("Indicator").value;
    indi=indicator;
    if(indicator=="TRACK_BB"){
        
        generateGraph2(-1);
    return;
    }
    else{
        
        generateGraph(-1)
    }
}
function generateGraph(V) {
    if(V==-1){
        V=noOfYears
    }
    
    
    let indicator=document.getElementById("Indicator").value;
    
    let month = document.getElementById(now);
    month.style.backgroundColor="#CA9849";
    month.style.color="#ffffff"
    const canvas = document.getElementById("myChart");
    const ctx = canvas.getContext('2d');
    let columnname = document.getElementById(CN);
    columnname.style.backgroundColor="#1F1F1F";
    columnname.style.color="#CA9849"
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  var stockName = document.getElementById('stock_name').value;
  /* var noOfYears = document.getElementById('no_of_years').value;
  var columnName = document.getElementById('column_name').value; */
    noOfYears=V
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
    
  // Make an AJAX request to the Flask server
  var xhr = new XMLHttpRequest();
  
  if(indicator=="NO"){
      
      xhr.open('POST', '/stocks', true);
  }
  else if(indicator=="TRACK_BB"){
    generateGraph2();
    return;
  }
  else{
     
      xhr.open(indicator, '/stocks', true);
  }



  xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  xhr.onreadystatechange = function () {
      if (xhr.readyState == 4 && xhr.status == 200) {
          // Parse the JSON response from the server

          var responseData = JSON.parse(xhr.responseText);

          // Access the canvas element and initialize the chart
          var ctx = document.getElementById('myChart').getContext('2d');
          var ex=ctx.chart;
          if(ex){
            ex.destroy();
          }
          var newChart = new Chart(ctx, {
              type: 'line',
              data: {
                  labels: responseData.x_values,
                  datasets: [{
                      label: stockName + ' for ' + noOfYears + ' DAYS ' + ' [ ' + columnName + ' v/s Dates ]',
                      data: responseData.y_values,
                      borderColor: 'rgba(75, 192, 192, 1)',
                      borderWidth: 2,
                      pointRadius: 0,
                      backgroundColor:"rgba(0,0,255,1.0)",
                      fill: false
                  }]
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
          ctx.chart=newChart;
      }
  };
  xhr.send('stock_name=' + stockName + '&no_of_years=' + noOfYears + '&column_name=' + columnName);
}


function generateGraph2(V) {
    if(V==-1){
        V=noOfYears
    }
    console.log(V);
    
    console.log(noOfYears);
    
    let month = document.getElementById(now);
    month.style.backgroundColor="#CA9849";
    month.style.color="#ffffff"
    const canvas = document.getElementById("myChart");
    const ctx = canvas.getContext('2d');
    let columnname = document.getElementById(CN);
    columnname.style.backgroundColor="#1F1F1F";
    columnname.style.color="#CA9849"
    ctx.clearRect(0, 0, canvas.width, canvas.height);
  
    noOfYears=V
    console.log(noOfYears);
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
    var stock1Name =  document.getElementById('stock_name').value;
    var stock2Name =  document.getElementById('stock_name').value;


    
    // Make an AJAX request to the Flask server for Stock 1
    var xhr1 = new XMLHttpRequest();
    xhr1.open('TRACK_BB1', '/stocks', true);
    xhr1.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xhr1.onreadystatechange = function () {
        if (xhr1.readyState == 4 && xhr1.status == 200) {
            // Parse the JSON response from the server for Stock 1
            var responseData1 = JSON.parse(xhr1.responseText);

            // Make an AJAX request to the Flask server for Stock 2
            var xhr2 = new XMLHttpRequest();
            xhr2.open('TRACK_BB2', '/stocks', true);
            xhr2.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
            xhr2.onreadystatechange = function () {
                if (xhr2.readyState == 4 && xhr2.status == 200) {
                    // Parse the JSON response from the server for Stock 2
                    var responseData2 = JSON.parse(xhr2.responseText);

                    // Access the canvas element
                    var ctx = document.getElementById('myChart').getContext('2d');

                    // Destroy the existing chart instance if it exists
                    var existingChart = ctx.chart;
                    if (existingChart) {
                        existingChart.destroy();
                    }

                    // Initialize the new chart for both stocks
                    var newChart = new Chart(ctx, {
                        type: 'line',
                        data: {
                            labels: responseData1.x_values,
                            datasets: [{
                                label: stock1Name + ' for ' + noOfYears + ' years ' + ' [ ' + indi + ' v/s Dates ]',
                                data: responseData1.y_values,
                                borderColor: 'rgba(75, 192, 192, 1)',
                                borderWidth: 2,
                                pointRadius: 0, // Set pointRadius to 0 to hide points
                                fill: false
                            },
                            {
                                label: stock2Name + ' for ' + noOfYears + ' years ' + ' [ ' + columnName + ' v/s Dates ]',
                                data: responseData2.y_values,
                                borderColor: 'rgba(255, 0, 0, 1)',
                                borderWidth: 2,
                                pointRadius: 0, // Set pointRadius to 0 to hide points
                                fill: false
                            }]
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
            };
            xhr2.send('stock_name=' + stock2Name + '&no_of_years=' + noOfYears + '&column_name=' + columnName);
        }
    };
    xhr1.send('stock_name=' + stock1Name + '&no_of_years=' + noOfYears + '&column_name=' + columnName);
}