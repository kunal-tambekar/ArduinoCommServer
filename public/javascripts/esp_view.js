
var color = Chart.helpers.color;
var config = {
  type: 'line',
  data: {
    datasets: [{
      label: 'Dataset with string point data',
      backgroundColor: color(window.chartColors.red).alpha(0.5).rgbString(),
      borderColor: window.chartColors.red,
      fill: false,
      spanGaps :true,
      data: [{
        x: newDateString(0),
        y: randomScalingFactor()
      }, {
        x: newDateString(2),
        y: randomScalingFactor()
      }, {
        x: newDateString(4),
        y: null
      }, {
        x: newDateString(5),
        y: randomScalingFactor()
      }],
    }]
  },
  options: {
    responsive: true,
    title: {
      display: true,
      text: 'Displaying TEMPERATURE data'
    },
    scales: {
      xAxes: [{
        type:'time',
        display: true,
        scaleLabel: {
          display: true,
          labelString: 'Date'
        },
        ticks: {
          major: {
            fontStyle: 'bold',
            fontColor: '#FF0000'
          }
        }
      }],
      yAxes: [{
        display: true,
        scaleLabel: {
          display: true,
          labelString: 'value'
        }
      }]
    }
  }
};



$('#fetchData').on('click', function() {

  let pg = $("#page").val();
  let size = $("#page_size").val();
  let par = $("#parameter").val();
  let mac = $("#eid").data("mac");

  let body = new Object();
  body.page = pg;
  body.page_size = size;
  body.parameter = par;
  body.mac = mac;
  console.log(config.data.datasets);
  console.log("Page: "+pg+" Size: "+size+" Parameter: "+par+" MAC: "+mac);
  $.post( "../api/data", body,function( data ) {
    config.data.datasets[0].data = [];
    data.forEach(obj=>{
      config.data.datasets[0].data.push({x:obj.timestamp,y:obj.value});
    });
    //  = [{x:0,y:0},{x:10,y:-10},{x:20,y:20},{x:30,y:-30},{x:40,y:40},{x:50,y:-50}]
    window.myLine.update();
  });

});

$('#randomizeData').on('click', function() {
  config.data.datasets.forEach(function(dataset) {
    dataset.data.forEach(function(dataObj) {
      dataObj.y = randomScalingFactor();
    });
  });

  window.myLine.update();
});

$('#addData').on('click', function() {
  if (config.data.datasets.length > 0) {
    config.data.datasets[0].data.push({
      x: newDateString(config.data.datasets[0].data.length + 2),
      y: randomScalingFactor()
    });
    window.myLine.update();
  }
});

$('#removeData').on('click', function() {
  config.data.datasets.forEach(function(dataset) {
    dataset.data.pop();
  });

  window.myLine.update();
});

$(function () {
  
  var ctx = document.getElementById('canvas').getContext('2d');
  window.myLine = new Chart(ctx, config);

});