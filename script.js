function updateBrightness(value) {
  fetch('/set?value=' + value);
}
