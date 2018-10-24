$(function () {

  $("#num_of_pins").on('input', (ev) => {
    let nop = $("#num_of_pins").val();
    $("#pins_layout").empty();
    if (Number.isInteger(Number.parseInt(nop))) {
      for (let x = 0; x < nop; x++) {

        let pin_input_layout = `<div class='mdl-cell--4-col-phone mdl-cell--8-col-tablet mdl-cell--5-col-desktop border m4'>
          <table class='mdl-table'>
          <tr class="mdl-grid">
          <td class="mdl-cell--4-col-phone mdl-cell--4-col-tablet mdl-cell--6-col"> 
          <div class='mdl-textfield mdl-js-textfield'>
          <spanv style="top:4px; font-size:10px;"> Pin Label: </span>
          <input class='mdl-textfield__input' type='text' pattern='-?^[a-zA-Z0-9_]*?' placeholder='Pin label:' title="Use only characters, numbers or '_' !" name='pin_label' id='pin_label${x}' required> 
          </div>
          </td>

          <td class="mdl-cell--4-col-phone mdl-cell--4-col-tablet mdl-cell--6-col">
          <div class="mdl-textfield mdl-js-textfield mdl-textfield--floating-label" >
          <spanv style="top:4px; font-size:10px;"> Pin Type: </span>
          <select class="mdl-textfield__input" name='pin_type' >
            <option value="Analog" selected>Analog</option>
            <option value="Digital">Digital</option>
          </select>
         </div>
          </td>
          </tr>
         
          </table>
          <br>
          </div>`;
        $("#pins_layout").append(pin_input_layout);
      }
    } else {

    }
  });


});