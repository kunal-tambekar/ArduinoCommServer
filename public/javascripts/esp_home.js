function deleteEsp(e) {
  let mac= $(e).data("mac");
  var del = confirm("Are you sure you want to delete this ESP [MAC: " + mac + " ]?");
    
  if(del) {
    var durl = 'http://localhost:3000/api/esp/delete/' + mac;
    $.get( durl,
      function (data) {
        window.open("../../../esp","_self");
        alert("Deleted "+mac+" ESP.");
    });  
  }

};