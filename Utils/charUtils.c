/*
* Paulo Vasconcelos
* paulobvasconcelos@gmail.com
* 2018 march
*/

char toLowerCase(char c) { if ('A' <= c && c <= 'Z') { c += 32; } return c; }

char toUpperCase(char c) { if ('a' <= c && c <= 'z') { c -= 32; } return c; }
