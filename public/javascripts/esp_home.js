function deleteEsp(e) {
  let mac= $(e).data("mac");
  var del = confirm("Are you sure you want to delete this ESP [MAC: " + mac + " ]? Its configuration will also be deleted!");
    
  if(del) {
    var durl = 'http://'+window.location.hostname+':3000/api/esp/delete/' + mac;
    $.get( durl,
      function (data) {
        window.open("../../../esp","_self");
        alert("Deleted "+mac+" ESP.");
    });  
  }

};