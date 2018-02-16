# int main(void)
# {
# printStr("AC2 – DETPIC32\n"); // system call
# return 0;
# }

      .equ printStr, 8
      .data
str1: .asciiz "AC2 - DETPIC32\n"

      .text
      .globl main

main: la $a0, str1
      ori $v0,$0, printStr
      syscall               # printStr("AC2 – DETPIC32\n");
      ori $v0,$0,0          # return 0;
      jr $ra
