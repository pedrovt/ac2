// --------------------------------
// Nearest Power Of 2 Function
// Computer Architecture II
// Source: https://stackoverflow.com/questions/758001/log2-not-found-in-my-math-h
// --------------------------------

unsigned int log2Integers(unsigned int x) {
    unsigned int ans = 0;
    while (x >>= 1)
        ans++;
    return ans;
}
