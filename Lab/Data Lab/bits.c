/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here> 
 *
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */


#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /* 
  what we want to get is when the same bit is 1 and the different is 0
  like (1010,1100)=(1001) and the we use ~ can get the xor value
  x&(~y) or (~x)&y can make us get the different value is 1 other is 0
  and we ~ it ,we can get the portion different is 0 and the same is 1
  and we & them we can get the same bit is 1 and the different is 0
  ops: 8 
  */
  int v1=x & (~y);
  int v2=(~x) & y;
  int v=(~v1) & (~v2);
  return ~v;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  /* 
  the minimum two's complement is 1000...00
  use left shift to move 000...1 to 1000...00 
  ops: 1
  */
  return 1<<31;

}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /*
  if x is tmax then x+1 is tmin 
  and tmax ^ tmin is -1 
  so we can check whether (x ^ (x+1)) is -1 
  but when x is -1  x ^ (x+1) also is -1;
  so we must check whether x is -1 
  ops: 8
  */
  return !(((x+1)^x)+1) & (!(!(x+1)));
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  /*
  make Base is a number that odd bits are 1 
  use & and ^ check whether x is or not
  ops: 9
  */
  int base1=0xAA;
  int base2=base1<<8;
  int base3=base2<<8;
  int base4=base3<<8;
  int Base=base1 | base2 | base3 | base4;
  return !((x & Base) ^ Base);
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  /*
  easy 
  ops: 2
  */
  return (~x) + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  /*
  0x30 - 0x39 
  0011 0000 - 0011 1001 
  first check whether is 0000 ... 0011 
  and  if the low 4 bit is 0xxx it is ok
  and  the last 2 situations is 0100 and 0101  
  so we shift it right 1bit and check whether it is 0x1C or not
  we can reduce the ops if we use the feature of bit exp
  ops: 10
  */
  int v1=(x>>4) ^ 3;
  int v2=x & 8;
  int v3=(x>>1) ^ 0x1C; 
  return (!v1) & ((!v2) | (!v3));
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /*
  if x is not 0 then y
  else if x is 0 then z
  we use op and ~op to realize the function of "if-else"
  ops: 8
  */
  int negx=(~x) + 1;
  int op=(negx | x)>>31;
  return (op & y) | ((~op) & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*
  we have few situations
  */
  int l=(x & (1<<31));
  int r=(y & (1<<31));
  int v=x+((~y)+1);
  return (!((v & (1<<31))^(1<<31)) | (!v) | (!(!l) & !r)) & ((!(!l)) | !r);
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
  /*
  if x is not 0 then 0
  if x is 0  then 1
  think about the difference between 0 and other nums
  the negative num of 0 is also 0
  but others are not
  */
  int neg=(~x) +  1;
  int v = neg | x;
  return (~(v>>31)) & 1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10 
 *            howManyBits(-5) = 4   
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
  /*
  all transform into positive num
  so we need to the position of the first one 
  we use Bisection method and use >> to select where to check then
  */
  int sign=(x) & (1<<31);
  int op=sign>>31;
  int v_4,v_3,v_2,v_1,v_0,v;
  x=x ^ op;
  v_4=(!(!(x>>16)))<<4;
  x=x>>v_4;
  v_3=(!(!(x>>8)))<<3;
  x=x>>v_3;
  v_2=(!(!(x>>4)))<<2;
  x=x>>v_2;
  v_1=(!(!(x>>2)))<<1;
  x=x>>v_1;
  v_0=(!(!(x>>1)))<<1;
  x=x>>v_0;
  v=(x & 1);
  return v+v_0+v_1+v_2+v_3+v_4+1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned sign=(uf & (1<<31));
//  printf("sign = %d\n",sign);
  unsigned e=(uf>>23) & 0xFFu;
  unsigned frac=((~(0xFF << 23)) & uf ) ^ sign;
  if(e == 0xFFu) {
 //   printf("CAONIMA1");
    return uf;  
  } else if(e == 0) {
    return (frac<<1) | sign;
  } else {
    if(e+1 == (0xFFu)) {
   //   printf("CAONIMA2");
      return 0x7F800000 | sign;
    } else {
      return ((e+1)<<23) | frac | sign;
    }
  }
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
    unsigned sign=uf & (1<<31);
    unsigned exp=(uf>>23) & 0xFFu;
    if(exp==255) {
        return 0x80000000u;
    } else {
        if(exp<=126) {
            return 0u;
        } else if(exp-127>=32){
            return 0x80000000u;
        } else {
            // printf("HERE is 1\n");
            unsigned frac=((~(0xFF << 23)) & uf ) ^ sign;
            unsigned E=exp - 127u;
            int re;
            if(E<=23) {
            //    printf("DD");
            //    printf("E = %d\n",E);
            //    printf("frac = %d\n",frac);
                re=frac >> (23-E);
            //    printf("re = %d\n",re);
                re=re | (1<<E);
            } else {
                re=frac << (E-23);
                re=re | (1<<E);
            }
            if(sign) {
               // printf("FF");
                re = (~re) + 1;
            }
            return re;
        }
    }
}

/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
  if(x<=-150) {
      return 0u;
  } else if(x>=128) {
      return 0x7F800000u;
  } else {
      //printf("HELLO1\n");
      unsigned exp = x + 127;
      if(x>=0) {
          return (exp << 23u);
      } else {
          if(exp>=1) {
              return exp << 23u;
          } else {
              return 1 << (23u+exp);
          }
      }
  }
}
