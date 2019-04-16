$(function () {
  
  $.getJSON( "../api/sensor", function( data ) {
    // console.log(JSON.stringify(data));
    let modelTypePinLabelMap = new Object();
    $.each(data.sensors,(i,el) => {
      modelTypePinLabelMap[el.model_type]=el.pin_label;
    });

    $('select[name="sensors"]').change((ev)=>{
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

  $("#num_of_pins").on('input', (ev) => {
    let nop = $("#num_of_pins").val();
    $("#pins_layout").empty();
    if (Number.isInteger(Number.parseInt(nop))) {
      for (let x = 0; x < nop; x++) {

        let pin_input_layout = `<div class='mdl-cell--4-col mdl-cell--3-col-desktop border-rc m2'>
          <div class='mdl-textfield mdl-js-textfield pin_labels'>
          <span style="top:4px; font-size:10px;"> Pin Label: </span>
          <input class='mdl-textfield__input mdl-cell--12-col' type='text' pattern='-?^[a-zA-Z0-9_]*?' placeholder='e.g.: GPIO_02' title="Use only characters, numbers or '_' !" name='pin_label' id='pin_label${x}' required> 
          </div>
          </div>`;
        $("#pins_layout").append(pin_input_layout);
      }
    } else {

    }
  });

  $("#btnInitOTA").on("click",(ev)=>{
    let mac = $("#mac").val();
    $.getJSON( "../api/esp/push_ota?mac="+mac, function( data ) {

    });
  });


});