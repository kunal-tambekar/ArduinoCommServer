function deleteSensor(e) {
  let modelType= $(e).data("modeltype");
  var del = confirm("Are you sure you want to delete this sensor [Model type: " + modelType + " ]?");
    
  if(del) {
    var durl = 'http://localhost:3000/api/sensor/delete/' + modelType;
    $.get( durl,
      function (data) {
        window.open("../../../sensor","_self");
        alert("Deleted "+modelType+" sensor.");
    });  
  }

};