// --------------------------------
// Nearest Power Of 2 Function
// Computer Architecture II
// Source: https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
// --------------------------------

unsigned int nearestPowerOf2 (unsigned int v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;

  return v;
}
