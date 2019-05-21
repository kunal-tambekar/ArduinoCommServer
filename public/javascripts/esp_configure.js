$(function () {
  
  $.getJSON( "../api/sensor", function( data ) {
    // console.log(JSON.stringify(data));
    let modelTypePinLabelMap = new Object();
    $.each(data.sensors,(i,el) => {
      modelTypePinLabelMap[el.model_type]=el.pin_label;
    });

    $('select[name="sensor_type"]').change((ev)=>{
      // To check map of sensors and its pins
      // console.log(JSON.stringify(modelTypePinLabelMap));
      let selectedSensor = $(ev.target).find('option:selected').val();
      var $dropdown = $(ev.target).parent().next().find('select[name="sensor_pin"]');
      $($dropdown).find('option').remove().end();
  
      $.each(modelTypePinLabelMap[selectedSensor], function( index, value ) {
          console.log("modelTypePinLabelMap[selectedSensor]"+modelTypePinLabelMap[selectedSensor]+" and Val:"+value);
          $($dropdown).append($("<option />").val(value).text(value));
      });
    });
  });

  $("#btnInitOTA").on("click",(ev)=>{
    let mac = $("#mac").val();
    $.getJSON( "../api/esp/push_ota?mac="+mac, function( data ) {

    });
  });


});