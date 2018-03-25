// --------------------------------
// Converts a value to BCD 
// Arquitectura de Computadores II
// --------------------------------

unsigned char toBcd(unsigned char value) {
  return ((value / 10) << 4) + (value % 10);
}
