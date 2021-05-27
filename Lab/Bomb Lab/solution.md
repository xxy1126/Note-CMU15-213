# phase_1

```
gdb-peda$ disas phase_1
Dump of assembler code for function phase_1:
   0x0000000000400ee0 <+0>:	sub    rsp,0x8
   0x0000000000400ee4 <+4>:	mov    esi,0x402400
   0x0000000000400ee9 <+9>:	call   0x401338 <strings_not_equal>
   0x0000000000400eee <+14>:	test   eax,eax
   0x0000000000400ef0 <+16>:	je     0x400ef7 <phase_1+23>
   0x0000000000400ef2 <+18>:	call   0x40143a <explode_bomb>
   0x0000000000400ef7 <+23>:	add    rsp,0x8
   0x0000000000400efb <+27>:	ret    
End of assembler dump.
```

只有<string_no_equal>的返回值%eax为0时，炸弹才不会爆炸，猜测可能要输入相同的字符串。

猜测字符串存放在0x402400处

`x/s 0x402400` 发现字符串`Border relations with Canada have never been better.`.

输入为：`Border relations with Canada have never been better.`

# phase_2

```
gdb-peda$ disas phase_2
Dump of assembler code for function phase_2:
   0x0000000000400efc <+0>:	push   rbp
   0x0000000000400efd <+1>:	push   rbx
   0x0000000000400efe <+2>:	sub    rsp,0x28
   0x0000000000400f02 <+6>:	mov    rsi,rsp
   0x0000000000400f05 <+9>:	call   0x40145c <read_six_numbers>
   0x0000000000400f0a <+14>:	cmp    DWORD PTR [rsp],0x1
   0x0000000000400f0e <+18>:	je     0x400f30 <phase_2+52>
   0x0000000000400f10 <+20>:	call   0x40143a <explode_bomb>
   0x0000000000400f15 <+25>:	jmp    0x400f30 <phase_2+52>
   0x0000000000400f17 <+27>:	mov    eax,DWORD PTR [rbx-0x4]
   0x0000000000400f1a <+30>:	add    eax,eax
   0x0000000000400f1c <+32>:	cmp    DWORD PTR [rbx],eax
   0x0000000000400f1e <+34>:	je     0x400f25 <phase_2+41>
   0x0000000000400f20 <+36>:	call   0x40143a <explode_bomb>
   0x0000000000400f25 <+41>:	add    rbx,0x4
   0x0000000000400f29 <+45>:	cmp    rbx,rbp
   0x0000000000400f2c <+48>:	jne    0x400f17 <phase_2+27>
   0x0000000000400f2e <+50>:	jmp    0x400f3c <phase_2+64>
   0x0000000000400f30 <+52>:	lea    rbx,[rsp+0x4]
   0x0000000000400f35 <+57>:	lea    rbp,[rsp+0x18]
   0x0000000000400f3a <+62>:	jmp    0x400f17 <phase_2+27>
   0x0000000000400f3c <+64>:	add    rsp,0x28
   0x0000000000400f40 <+68>:	pop    rbx
   0x0000000000400f41 <+69>:	pop    rbp
   0x0000000000400f42 <+70>:	ret    
End of assembler dump.
```

需要注意的是peda插件反汇编出来的是`mov d s`,把s放到d

首先调用一个<read_six_numbers>函数，根据函数名推测读入6个数。

先输入`1 2 3 4 5 6`,发现<+18>处成功跳转，查看寄存器rsp处地址为`0x7fffffffe490`

查看peda插件 stack部分

```
[------------------------------------stack-------------------------------------]
0000| 0x7fffffffe490 --> 0x200000001 
0008| 0x7fffffffe498 --> 0x400000003 
0016| 0x7fffffffe4a0 --> 0x600000005 
0024| 0x7fffffffe4a8 --> 0x401431 (<skip+56>:	test   eax,eax)
0032| 0x7fffffffe4b0 --> 0x402210 (<__libc_csu_init>:	mov    QWORD PTR [rsp-0x28],rbp)
0040| 0x7fffffffe4b8 --> 0x402210 (<__libc_csu_init>:	mov    QWORD PTR [rsp-0x28],rbp)
0048| 0x7fffffffe4c0 --> 0x0 
0056| 0x7fffffffe4c8 --> 0x400e5b (<main+187>:	call   0x4015c4 <phase_defused>)
```

发现rsp存的地址处就是我们输入的6个数字，`x/6w 0x7fffffffe490 `查看从`0x7fffffffe490`（rsp处的地址）开始的6个4字节的地址

跳转后来到<+52>,rbx被赋值为[rsp+0x4]也就是我们输入的第二个数记为arg2。6个参数从rsp到rsp+0x17,rbp可能是一个终止条件。

跳转到<+27>.eax的值被设为第一个参数记为arg1,然后eax=2arg1

<+32>处判断eax和rbx的值是否相等，也就是2arg1==arg2，之后给rbx一个增量，rbx变成arg3，而由于2arg1=arg2，所以eax的值为arg2，直到6个参数循环完。

所以一共输入6个数，第一个数是1，后面每一个数都是前一个的两倍

输入为：`1 2 4 8 16 32`

# phase_3

```
gdb-peda$ disas phase_3
Dump of assembler code for function phase_3:
   0x0000000000400f43 <+0>:	sub    rsp,0x18
   0x0000000000400f47 <+4>:	lea    rcx,[rsp+0xc]
   0x0000000000400f4c <+9>:	lea    rdx,[rsp+0x8]
   0x0000000000400f51 <+14>:	mov    esi,0x4025cf
   0x0000000000400f56 <+19>:	mov    eax,0x0
   0x0000000000400f5b <+24>:	call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000400f60 <+29>:	cmp    eax,0x1
   0x0000000000400f63 <+32>:	jg     0x400f6a <phase_3+39>
   0x0000000000400f65 <+34>:	call   0x40143a <explode_bomb>
   0x0000000000400f6a <+39>:	cmp    DWORD PTR [rsp+0x8],0x7
   0x0000000000400f6f <+44>:	ja     0x400fad <phase_3+106>
   0x0000000000400f71 <+46>:	mov    eax,DWORD PTR [rsp+0x8]
   0x0000000000400f75 <+50>:	jmp    QWORD PTR [rax*8+0x402470]
   0x0000000000400f7c <+57>:	mov    eax,0xcf
   0x0000000000400f81 <+62>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f83 <+64>:	mov    eax,0x2c3
   0x0000000000400f88 <+69>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f8a <+71>:	mov    eax,0x100
   0x0000000000400f8f <+76>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f91 <+78>:	mov    eax,0x185
   0x0000000000400f96 <+83>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f98 <+85>:	mov    eax,0xce
   0x0000000000400f9d <+90>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400f9f <+92>:	mov    eax,0x2aa
   0x0000000000400fa4 <+97>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fa6 <+99>:	mov    eax,0x147
   0x0000000000400fab <+104>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fad <+106>:	call   0x40143a <explode_bomb>
   0x0000000000400fb2 <+111>:	mov    eax,0x0
   0x0000000000400fb7 <+116>:	jmp    0x400fbe <phase_3+123>
   0x0000000000400fb9 <+118>:	mov    eax,0x137
   0x0000000000400fbe <+123>:	cmp    eax,DWORD PTR [rsp+0xc]
   0x0000000000400fc2 <+127>:	je     0x400fc9 <phase_3+134>
   0x0000000000400fc4 <+129>:	call   0x40143a <explode_bomb>
   0x0000000000400fc9 <+134>:	add    rsp,0x18
   0x0000000000400fcd <+138>:	ret    
End of assembler dump.
```

看到有很多jmp，mov，jmp的地址都是<+123> 

<+123>把上边放到eax中的值和 [rsp+0xc]进行比对，如果相同就是成功。

<+50>可以看出，是根据eax的值来确定跳转到哪里，eax的值在[rsp+0x8]中，其中eax的值要小于等于7.

从<+4><+9>可以看出，scanf读取两个参数，第一个放在rsp+0x8,第二个放在rsp+0xc

第一个数输入4，单步调试条跳转到<+78>,第二个数应该是0x185也就是389

输入：`4 389`（有很多种情况）

# phase_4

```
gdb-peda$ disas phase_4
Dump of assembler code for function phase_4:
   0x000000000040100c <+0>:	sub    rsp,0x18
   0x0000000000401010 <+4>:	lea    rcx,[rsp+0xc]
   0x0000000000401015 <+9>:	lea    rdx,[rsp+0x8]
   0x000000000040101a <+14>:	mov    esi,0x4025cf
   0x000000000040101f <+19>:	mov    eax,0x0
   0x0000000000401024 <+24>:	call   0x400bf0 <__isoc99_sscanf@plt>
   0x0000000000401029 <+29>:	cmp    eax,0x2
   0x000000000040102c <+32>:	jne    0x401035 <phase_4+41>
   0x000000000040102e <+34>:	cmp    DWORD PTR [rsp+0x8],0xe
   0x0000000000401033 <+39>:	jbe    0x40103a <phase_4+46>
   0x0000000000401035 <+41>:	call   0x40143a <explode_bomb>
   0x000000000040103a <+46>:	mov    edx,0xe
   0x000000000040103f <+51>:	mov    esi,0x0
   0x0000000000401044 <+56>:	mov    edi,DWORD PTR [rsp+0x8]
   0x0000000000401048 <+60>:	call   0x400fce <func4>
   0x000000000040104d <+65>:	test   eax,eax
   0x000000000040104f <+67>:	jne    0x401058 <phase_4+76>
   0x0000000000401051 <+69>:	cmp    DWORD PTR [rsp+0xc],0x0
   0x0000000000401056 <+74>:	je     0x40105d <phase_4+81>
   0x0000000000401058 <+76>:	call   0x40143a <explode_bomb>
   0x000000000040105d <+81>:	add    rsp,0x18
   0x0000000000401061 <+85>:	ret    
End of assembler dump.
```

调用了函数func4

```
gdb-peda$ disas func4
Dump of assembler code for function func4:
   0x0000000000400fce <+0>:	sub    rsp,0x8
   0x0000000000400fd2 <+4>:	mov    eax,edx
   0x0000000000400fd4 <+6>:	sub    eax,esi
   0x0000000000400fd6 <+8>:	mov    ecx,eax
   0x0000000000400fd8 <+10>:	shr    ecx,0x1f
   0x0000000000400fdb <+13>:	add    eax,ecx
   0x0000000000400fdd <+15>:	sar    eax,1
   0x0000000000400fdf <+17>:	lea    ecx,[rax+rsi*1]
   0x0000000000400fe2 <+20>:	cmp    ecx,edi
   0x0000000000400fe4 <+22>:	jle    0x400ff2 <func4+36>
   
   0x0000000000400fe6 <+24>:	lea    edx,[rcx-0x1]
   0x0000000000400fe9 <+27>:	call   0x400fce <func4>
   0x0000000000400fee <+32>:	add    eax,eax
   0x0000000000400ff0 <+34>:	jmp    0x401007 <func4+57>
   
   0x0000000000400ff2 <+36>:	mov    eax,0x0
   0x0000000000400ff7 <+41>:	cmp    ecx,edi
   0x0000000000400ff9 <+43>:	jge    0x401007 <func4+57>
   0x0000000000400ffb <+45>:	lea    esi,[rcx+0x1]
   0x0000000000400ffe <+48>:	call   0x400fce <func4>
   0x0000000000401003 <+53>:	lea    eax,[rax+rax*1+0x1]
   0x0000000000401007 <+57>:	add    rsp,0x8
   0x000000000040100b <+61>:	ret    
End of assembler dump.
```

先分析phase_4，phase_4中的<+29>可以判断应该输入两个数字，分别位于[rsp+0x8]和[rsp+0xc]。

<+34>和<+39>可以看到第一个输入的参数要<=0xe,然后为调用func4函数设置参数，三个寄存器rdi=第一个参数,rsi=0,rdx=e=14

调用func4的返回值为eax，eax应该为0，然后输入第二个参数也应该为0。

再分析func4 三个参数rdi,rsi,rdx分别记为a,b,c。eax=c-b,然后ecx=eax=c-b,ecx=(c-b)>>0x1f。(这里ecx其实已经变成0了，所以eax的值实际上是不变的，还是eax)。eax=(c-b)>>1。

ecx=rax+rsi=(c+b)>>1,看a==(c+b)/2

```c++
func(a,b,c)
if(a==(c+b)/2) {
    return 0;
} else if(a<(c+b)/2) {
    return func(a,(c+b)/2+1,c)*2+1; 
} else {
    return func(a,b,(c+b)/2-1)*2;
}
```

我们要调用func(a,0,e)返回值为0，所以最简单的就是a=7，也就是第一个输入参数为7，第二个参数为0（由phase_4 <+69>确定）

输入：`7 0 `

# phase_5

```
gdb-peda$ disas phase_5
Dump of assembler code for function phase_5:
   0x0000000000401062 <+0>:	push   rbx
   0x0000000000401063 <+1>:	sub    rsp,0x20
   0x0000000000401067 <+5>:	mov    rbx,rdi
   0x000000000040106a <+8>:	mov    rax,QWORD PTR fs:0x28
   0x0000000000401073 <+17>:	mov    QWORD PTR [rsp+0x18],rax
   0x0000000000401078 <+22>:	xor    eax,eax
   0x000000000040107a <+24>:	call   0x40131b <string_length>
   0x000000000040107f <+29>:	cmp    eax,0x6//字符串
   0x0000000000401082 <+32>:	je     0x4010d2 <phase_5+112>
   0x0000000000401084 <+34>:	call   0x40143a <explode_bomb>
   0x0000000000401089 <+39>:	jmp    0x4010d2 <phase_5+112>
   
   0x000000000040108b <+41>:	movzx  ecx,BYTE PTR [rbx+rax*1]
   0x000000000040108f <+45>:	mov    BYTE PTR [rsp],cl
   0x0000000000401092 <+48>:	mov    rdx,QWORD PTR [rsp]
   0x0000000000401096 <+52>:	and    edx,0xf
   0x0000000000401099 <+55>:	movzx  edx,BYTE PTR [rdx+0x4024b0]
   0x00000000004010a0 <+62>:	mov    BYTE PTR [rsp+rax*1+0x10],dl
   0x00000000004010a4 <+66>:	add    rax,0x1
   0x00000000004010a8 <+70>:	cmp    rax,0x6
   0x00000000004010ac <+74>:	jne    0x40108b <phase_5+41>
   
   0x00000000004010ae <+76>:	mov    BYTE PTR [rsp+0x16],0x0
   0x00000000004010b3 <+81>:	mov    esi,0x40245e
   0x00000000004010b8 <+86>:	lea    rdi,[rsp+0x10]
   0x00000000004010bd <+91>:	call   0x401338 <strings_not_equal>
   0x00000000004010c2 <+96>:	test   eax,eax
   0x00000000004010c4 <+98>:	je     0x4010d9 <phase_5+119>
   0x00000000004010c6 <+100>:	call   0x40143a <explode_bomb>
   0x00000000004010cb <+105>:	nop    DWORD PTR [rax+rax*1+0x0]
   0x00000000004010d0 <+110>:	jmp    0x4010d9 <phase_5+119>
   0x00000000004010d2 <+112>:	mov    eax,0x0
   0x00000000004010d7 <+117>:	jmp    0x40108b <phase_5+41>
   0x00000000004010d9 <+119>:	mov    rax,QWORD PTR [rsp+0x18]
   0x00000000004010de <+124>:	xor    rax,QWORD PTR fs:0x28
   0x00000000004010e7 <+133>:	je     0x4010ee <phase_5+140>
   0x00000000004010e9 <+135>:	call   0x400b30 <__stack_chk_fail@plt>
   0x00000000004010ee <+140>:	add    rsp,0x20
   0x00000000004010f2 <+144>:	pop    rbx
   0x00000000004010f3 <+145>:	ret    
End of assembler dump.
```

输入字符串长度为为6，<+34>到<+74>应该是一个for循环来对字符串进行一些操作。

操作完之后的字符串的首地址为`rsp+0x10`,之后和地址为`0x40245e`处的字符串进行比较，`0x40245e`的字符串为`flyers`

输入字符串存在了`rbx`的位置

对输入字符串操作之后获得一个索引，在下边字符串中进行替换`maduiersnfotvbylSo you think you can stop the bomb with ctrl-c, do you?`

`flyers`的下标分别为`  9 15 14 5 6 7     `

脚本破解为`ionefg`(很多情况)

输入：`ionefg`

# phase_6

```
gdb-peda$ disas phase_6
Dump of assembler code for function phase_6:
   0x00000000004010f4 <+0>:	push   r14
   0x00000000004010f6 <+2>:	push   r13
   0x00000000004010f8 <+4>:	push   r12
   0x00000000004010fa <+6>:	push   rbp
   0x00000000004010fb <+7>:	push   rbx
   0x00000000004010fc <+8>:	sub    rsp,0x50
   0x0000000000401100 <+12>:	mov    r13,rsp
   0x0000000000401103 <+15>:	mov    rsi,rsp
   0x0000000000401106 <+18>:	call   0x40145c <read_six_numbers>
   0x000000000040110b <+23>:	mov    r14,rsp
   0x000000000040110e <+26>:	mov    r12d,0x0
   0x0000000000401114 <+32>:	mov    rbp,r13
   0x0000000000401117 <+35>:	mov    eax,DWORD PTR [r13+0x0]
   0x000000000040111b <+39>:	sub    eax,0x1
   0x000000000040111e <+42>:	cmp    eax,0x5
   0x0000000000401121 <+45>:	jbe    0x401128 <phase_6+52>
   0x0000000000401123 <+47>:	call   0x40143a <explode_bomb>
   0x0000000000401128 <+52>:	add    r12d,0x1
   0x000000000040112c <+56>:	cmp    r12d,0x6
   0x0000000000401130 <+60>:	je     0x401153 <phase_6+95>
   0x0000000000401132 <+62>:	mov    ebx,r12d
   0x0000000000401135 <+65>:	movsxd rax,ebx
   0x0000000000401138 <+68>:	mov    eax,DWORD PTR [rsp+rax*4]
   0x000000000040113b <+71>:	cmp    DWORD PTR [rbp+0x0],eax
   0x000000000040113e <+74>:	jne    0x401145 <phase_6+81>
   0x0000000000401140 <+76>:	call   0x40143a <explode_bomb>
   0x0000000000401145 <+81>:	add    ebx,0x1
   0x0000000000401148 <+84>:	cmp    ebx,0x5
   0x000000000040114b <+87>:	jle    0x401135 <phase_6+65>
   0x000000000040114d <+89>:	add    r13,0x4
   0x0000000000401151 <+93>:	jmp    0x401114 <phase_6+32>
   0x0000000000401153 <+95>:	lea    rsi,[rsp+0x18]
   0x0000000000401158 <+100>:	mov    rax,r14
   0x000000000040115b <+103>:	mov    ecx,0x7
   0x0000000000401160 <+108>:	mov    edx,ecx
   0x0000000000401162 <+110>:	sub    edx,DWORD PTR [rax]
   0x0000000000401164 <+112>:	mov    DWORD PTR [rax],edx
   0x0000000000401166 <+114>:	add    rax,0x4
   0x000000000040116a <+118>:	cmp    rax,rsi
   0x000000000040116d <+121>:	jne    0x401160 <phase_6+108>
   0x000000000040116f <+123>:	mov    esi,0x0
   0x0000000000401174 <+128>:	jmp    0x401197 <phase_6+163>
   0x0000000000401176 <+130>:	mov    rdx,QWORD PTR [rdx+0x8]
   0x000000000040117a <+134>:	add    eax,0x1
   0x000000000040117d <+137>:	cmp    eax,ecx
   0x000000000040117f <+139>:	jne    0x401176 <phase_6+130>
   0x0000000000401181 <+141>:	jmp    0x401188 <phase_6+148>
   0x0000000000401183 <+143>:	mov    edx,0x6032d0
   0x0000000000401188 <+148>:	mov    QWORD PTR [rsp+rsi*2+0x20],rdx
   0x000000000040118d <+153>:	add    rsi,0x4
   0x0000000000401191 <+157>:	cmp    rsi,0x18
   0x0000000000401195 <+161>:	je     0x4011ab <phase_6+183>
   0x0000000000401197 <+163>:	mov    ecx,DWORD PTR [rsp+rsi*1]
   0x000000000040119a <+166>:	cmp    ecx,0x1
   0x000000000040119d <+169>:	jle    0x401183 <phase_6+143>
   0x000000000040119f <+171>:	mov    eax,0x1
   0x00000000004011a4 <+176>:	mov    edx,0x6032d0
   0x00000000004011a9 <+181>:	jmp    0x401176 <phase_6+130>
   0x00000000004011ab <+183>:	mov    rbx,QWORD PTR [rsp+0x20]
   0x00000000004011b0 <+188>:	lea    rax,[rsp+0x28]
   0x00000000004011b5 <+193>:	lea    rsi,[rsp+0x50]
   0x00000000004011ba <+198>:	mov    rcx,rbx
   0x00000000004011bd <+201>:	mov    rdx,QWORD PTR [rax]
   0x00000000004011c0 <+204>:	mov    QWORD PTR [rcx+0x8],rdx
   0x00000000004011c4 <+208>:	add    rax,0x8
   0x00000000004011c8 <+212>:	cmp    rax,rsi
   0x00000000004011cb <+215>:	je     0x4011d2 <phase_6+222>
   0x00000000004011cd <+217>:	mov    rcx,rdx
   0x00000000004011d0 <+220>:	jmp    0x4011bd <phase_6+201>
   0x00000000004011d2 <+222>:	mov    QWORD PTR [rdx+0x8],0x0
   0x00000000004011da <+230>:	mov    ebp,0x5
   0x00000000004011df <+235>:	mov    rax,QWORD PTR [rbx+0x8]
   0x00000000004011e3 <+239>:	mov    eax,DWORD PTR [rax]
   0x00000000004011e5 <+241>:	cmp    DWORD PTR [rbx],eax
   0x00000000004011e7 <+243>:	jge    0x4011ee <phase_6+250>
   0x00000000004011e9 <+245>:	call   0x40143a <explode_bomb>
   0x00000000004011ee <+250>:	mov    rbx,QWORD PTR [rbx+0x8]
   0x00000000004011f2 <+254>:	sub    ebp,0x1
   0x00000000004011f5 <+257>:	jne    0x4011df <phase_6+235>
   0x00000000004011f7 <+259>:	add    rsp,0x50
   0x00000000004011fb <+263>:	pop    rbx
   0x00000000004011fc <+264>:	pop    rbp
   0x00000000004011fd <+265>:	pop    r12
   0x00000000004011ff <+267>:	pop    r13
   0x0000000000401201 <+269>:	pop    r14
   0x0000000000401203 <+271>:	ret    
End of assembler dump.
```

用IDA反汇编可以看出，循环判断两两不相同，大小在1-6之间。

然后对链表排序

输入：`4 3 2 1 6 5`

