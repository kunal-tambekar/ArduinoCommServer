
exports.getTimestamp = function () {
  if (!Date.now) {
    Date.now = function() { return new Date().getTime(); }
  }
  return Math.floor(Date.now() / 1000);
}