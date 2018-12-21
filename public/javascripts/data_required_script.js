$(function () {

  // Material design lite has a bug where it mars the 'required' fields as 'dirty' on page load.
  // Using this code we can add the required property after the page has loaded,
  // so instead of marking it as 'required' use data attribute "data-required=true"
  $(window).load(function () {
    $('input[data-required=true]').attr('required', true);
  });

});