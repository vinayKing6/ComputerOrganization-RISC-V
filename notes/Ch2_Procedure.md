# Introduction

## Instruction set

被计算机体系结构所理解的命令的词汇表 （指令集）

## Stored-program concept

在这种概念里，指令以及相关数据都被存储在内存中（Memory）

## 相关指令集如下：

<img src="img\risv_instructions.png" style="zoom: 33%;" />

# 2.2 计算机硬件的操作(Operations of Computer Hardware)

任何计算机都能进行运算操作。

在RISC-V中，例如加法运算可用如下指令表示：

```apl
a=b+c
d=a-e
```

```assembly
add a, b, c //a=b+c
sub d, a, e //d=a-e
```

```apl
f=(g+h)-(i+j)
```

```assembly
add t0, g, h //将运算结果存储在临时变量t0中
add t1, i, j
sub f, t0, t1
```

很明显，运算操作中，每一条指令都有三个操作数，两个用于计算，一个用于存储结果。这种思想体现了一种原则：有规律的往往是简单的 （Simplicity favors regularity）

# 2.3 计算机硬件的操作数 (Operands of Computer Hardware)

## Operands

用于进行计算操作的数

这些操作数必须被读取到寄存器中进行运算（register），在RISC-V中，register的大小是64 bits。RISC-V中有32个register，operands 必须取自32个64bits的register之中。

## double word

64 bits

## word

32 bits

## 用寄存器的指令集

```apl
f=(g+h)-(i+j)
```

假设变量f,g,h,i,j分别存储在x19,x20,x21,x22,x23

```assembly
add x5, x20, x21 //临时变量存储在寄存器x5 x6中
add x6, x22, x23
sub x19, x5, x6
```

## 内存操作数 （Memory Oprands）

由于寄存器的数量很少，并且运算只能在寄存器上进行，所以一些结构体数据和数组数据都存储在内存中（Memory）。RISC-V提供了在寄存器和内存中传输数据的指令，要获得内存中的数据，指令必须有数据在内存中的地址（操作数之一）。内存是一个很大的一维的数组，地址相当于对数组的引用，从0开始，在RISC-V中每个单元代表一个byte (8 bits)。

*数据从内存传输到寄存器叫做 load* ，加载doubleword 的指令是 ld 。例如：

```apl
g=h+A[8]
```

 假设变量g,h存储在x20,21，数组的地址存储在x22

```assembly
ld x9, 64(x22) // 先将A[8]从内存中读取到寄存器x8中作为临时变量，由于doubleword是8字节，8个doubleword需要64字节
add x20, x21, x9 //g=h+A[8]
```

*数据从寄存器存储到内存中叫做 store*，存储doubleword的指令是 sd。例如：

```apl
A[12]=h+A[8]
```

假设如上

```assembly
ld x9, 64(x22) //临时变量存储到x9
add x9, x9, x21
sd x9, 96(x22) //A[12]=h+A[8]
```

## 操作数之常量和立即数(Constant or Immediate Operands)

寄存器中读取数据和操作数据是最快的，有的时候指令中需要常数，但是常数存储在内存中，这样会大大降低效率，例如：

```assembly
ld x9, ConstantOffset(x3) //x9=constant constant在内存中
add x22, x22, x9 //x22=x22+x9 x9相当于是常数
```

RISC-V提供了可以直接运算常量的指令，例如add immediate ==> addi，例如:

```assembly
addi x22, x22, 4 //x22=x22+4
```

常量0是寄存器x0，x0被硬接线为0。

# 2.4 符号数和无符号数（Signed and Unsigned Numbers）

## binary digit

简称bit，二进制其中之一，0或1

## least significant bit

在RISC-V中，一个doubleword中最右边的bit是*least significant bit*

## most significant bit

同样的，一个doubleword中最左边的bit是*most significant bit*

一个doubleword可以表示0～$$2^{64}-1$$的数，n位的二进制可以表示0~$$2^{n}-1$$的数，可以通过等比数列求和计算得出。

## Two's complement (二的补码)

为了使计算机能够运算负数，并且为了简单化，最终的解决方法就是使用*Two's complement*，即把doubleword的*leading 0s*（第63位及往左都是0）表示正数，*leading 1s*（第63位及往左都是1）表示负数。例如：
$$
\begin{aligned}
00000000\,00000000\,00000000\,00000000\,00000000\,00000001&=1_{ten}\\
10000000\,00000000\,00000000\,00000000\,00000000\,00000001&=-2^{63}+1_{ten}\\
11111111\,11111111\,11111111\,11111111\,11111111\,11111101&=-2^{63}+2^{63}-1-2^{1}=-3_{ten}
\end{aligned}
$$
计算时将第63位的数字乘以负数加上62~0位的所有数字。*signed doubleword*能表示 $$-2^{63}$$~$$2^{63}-1$$的数。

## 计算符号数字的简便方法

### 倒置法 （invert 0 to  1 and 1 to 0）

在符号数字中111…111表示-1，所以$$\bar{x}+x=-1$$，因此$$\bar{x}+1=-x$$ ($$\bar{x}$$表示$$x$$的倒置)。例如：
$$
\begin{aligned}
x=&00000000\,00000000\,00000000\,00000000\,00000000\,00000010=2_{ten}\\
\bar{x}=&11111111\,11111111\,11111111\,11111111\,11111111\,11111101\\
\bar{x}+1=&11111111\,11111111\,11111111\,11111111\,11111111\,11111101\\+\\
&00000000\,00000000\,00000000\,00000000\,00000000\,00000001\\
=&11111111\,11111111\,11111111\,11111111\,11111111\,11111110\\
=&-2_{ten}
\end{aligned}
$$

# 2.5 在计算机中表示指令集(Representing Instructions in the Computer)

指令集在计算机中以一系列高低电信号村粗。指令集的每一部分都可以被看作一个独立的数字，将这些数字放在一起就能形成指令。RISC-V中的32个寄存器可以从0~31引用。

## 将汇编翻译成机器指令

例如：

```assembly
add x9, x20, x21
```

十进制表示法是

```apl
0 21 20 0 9 51
```

该指令的每一个数字成为一个字段（field），第一个，第四个，第六个字段（0 0 51）一起告诉计算机该指令执行加法操作（addition）。第二个字段（21）表示第二个源操作数寄存器的数字（x21），第三个字段（20）表示另外一个源操作数寄存器的数字，第五个字段（9）表示存放最终结果的寄存器的数字（x9）。该指令也可以表示为二进制数字：

| 0000000 | 10101  | 10100  |  000   | 01001  | 0110011 |
| :-----: | :----: | :----: | :----: | :----: | :-----: |
| 7 bits  | 5 bits | 5 bits | 3 bits | 5 bits | 7 bits  |
|    0    |   21   |   20   |   0    |   9    |   51    |
| funct7  |  rs2   |  rs1   | funct3 |   rd   | opcode  |

RISC-V的每一条指令都是32bits大小，上图表格中的指令布局称为指令格式（instruction format）。

### Machine language

为了与汇编语言区分，二进制表示的指令称为机器语言。

## RISC-V字段（field）

以上面的加法指令为例子，其中的字段可以表示成：

| funct7 |  rs2   |  rs1   | funct3 |   rd   | opcode |
| :----: | :----: | :----: | :----: | :----: | :----: |
| 7 bits | 5 bits | 5 bits | 3 bits | 5 bits | 7 bits |

以上字段的名字的意义是：

- **opcode** ：指令的基本操作
- **rd**：最终目标寄存器操作数，能够得到指令操作的结果
- **funct3**：额外的 opcode 字段
- **rs1**：第一个寄存器源操作数
- **rs2**：第二个寄存器源操作数
- funct7：额外的 opcode 字段

### opcode

一种能够指明操作和指令格式的字段

## R-type format (for register) 

R-type 格式如以上所示指令格式所示，主要用于寄存器的操作。

## I-type format (operation with one constant operand) 

I-type 格式主要用于有一个常数操作数的指令，例如*addi*、*load* 等等。大致上，I-type 格式如图示：

| immediate |  rs1   | funct3 |   rd   | opcode |
| :-------: | :----: | :----: | :----: | :----: |
|  12 bits  | 5 bits | 3 bits | 5 bits | 7 bits |

12bit 大小的immediate代表二的补码的数值，即可以表示$$-2^{11}$$~$$2^{11}-1$$的十进制数值。当I-type 用于load指令时，immediate代表字节偏移量（byte offset），rs1表示存储内存中起始地址（base address）的寄存器源操作数，相当于在内存地址中寻找*rs1+immediate*。因此*ld* (load doubleword) 指令能够在*rs1*(base address)的基础上寻找地址在区域$$\pm{2^{11}}$$之间的doublewords。

## S-type format (split immediate into two fields)

S-type 格式将immediate分成了两个字段，这样做是为了保持rs1和rs2在所有指令格式中处于相同的位置，如*sd*（store doubleword）。其中的字段如下：

| immediate[11:5] |  rs2   |  rs1   | funct3 | immediate[4:0] | opcode |
| :-------------: | :----: | :----: | :----: | :------------: | :----: |
|     7 bits      | 5 bits | 5 bits | 3 bits |     5 bits     | 7 bits |

同样的，所有指令中opcode 和 funct3也同样在一样的位置，指令的格式通过opcode进行区分，所以opcode位于二进制的第一个位置，硬件首先读取opcode，之后才能知道怎样处理剩下的指令部分。

## 举例

假设数组A的基础地址是x10，变量h存储在x21，翻译成机器语言：

```apl
A[30]=h+A[30]+1;
```

首先翻译成汇编：

```apl
ld x9, 240(x10) //将A[30]从内存读取到临时寄存器x9
add x9, x21, x9 //x9=A[30]+h
addi x9, x9, 1 //x9=x9+1
sd x9, 240(x10) //将最终结果存储到A[30]
```

其次将每条指令翻译成机器语言

*ld x9, 240(x10)*：

|  immediate   |  rs1  | funct3 |  rd   | opcode  |
| :----------: | :---: | :----: | :---: | :-----: |
|     240      |  10   |   3    |   9   |    3    |
| 000011110000 | 01010 |  011   | 01001 | 0000011 |

*add x9, x21, x9*：

| funct7  |  rs2  |  rs1  | funct3 |  rd   | opcode  |
| :-----: | :---: | :---: | :----: | :---: | :-----: |
|    0    |   9   |  21   |   0    |   9   |   51    |
| 0000000 | 01001 | 10101 |  000   | 01001 | 0110011 |

*addi x9, x9, 1*：

|  immediate   |  rs1  | funct3 |  rd   | opcode  |
| :----------: | :---: | :----: | :---: | :-----: |
|      1       |   9   |   0    |   9   |   19    |
| 000000000001 | 01001 |  000   | 01001 | 0010011 |

*sd x9 240(x10)*

| immediate[11:5] |  rs2  |  rs1  | funct3 | immediate[4:0] | opcode  |
| :-------------: | :---: | :---: | :----: | :------------: | :-----: |
|        7        |   9   |  10   |   3    |       16       |   35    |
|     0000111     | 01001 | 01010 |  011   |     10000      | 0100011 |

# 2.6逻辑操作（Logical Operations）

|                逻辑操作                 | C 操作符号 | Java 操作符号 | RISC-V 指令 |
| :-------------------------------------: | :--------: | :-----------: | :---------: |
|            Shift Left (左移)            |     <<     |      <<       |  sll, slli  |
|           Shift Right (右移)            |     >>     |      >>>      |  srl, srli  |
| Shift Right arithmetic (可以计算符号数) |     >>     |      >>       |  sra, srai  |
|           Bit-by-bit AND (与)           |     &      |       &       |  and, andi  |
|           Bit-by-bit OR (或)            |     \|     |      \|       |   or, ori   |
|   Bit-by-bit XOR (exclusive or 异或)    |     ^      |       ^       |  xor, xori  |
|           Bit-by-bit NOT (非)           |     ~      |       ~       |    xori     |

## Shift

Shift 分为*Shift Left*和*Shift Right*，左移(Shift Left)相当于将原先的bits往左移动n个bits，右边空出来的bits用0填充，相当于乘$2^n$；右移则相反，srli (shift right logical immediate) 将左边空出来的bits用0填充，srai (*shift right arighmetic immediate*)将左边空出来的bits用sign bit填充。指令例如：

```apl
slli x11, x19, 4 // x11 = x19 << 4 bits
```

Shift left logical immediate (slli) ，使用 I-type 格式，由于bit移动超过63bits对于64bits的寄存器来说是毫无意义的，所以*slli*只使用immediate的最低的6bits ($2^6-1=63$)。剩下的6bits以额外的opcode字段使用：

| funct6 | immediate | rs1  | funct3 |  rd  | opcode |
| :----: | :-------: | :--: | :----: | :--: | :----: |
|   0    |     4     |  19  |   1    |  11  |   19   |

## AND

与操作，将两个数按位操作，同为1则1，否则为0

## OR

或操作，同样按位操作，如果有1则为1，否则为0

## XOR

异或操作，同样按位操作，如果两个位不同则为1，否则为0

## NOT

非操作，只有一个操作数，将0变成1，1变成0

# 2.7 条件指令 (Instructions for Making Decisions)

RISC-V包括两个decision-making指令：

```apl
beq rs1, rs2, L1
```

**branch if equal**，这条指令的意思是，如果rs1等于rs2，就跳转到标记为L1的声明。

```apl
bne rs1, rs2, L1
```

**branch if not equal** ，与上面的指令相反。

## Conditional branch

一条指令能够测试数值，并且允许通过测试的结果来把控制权转移到程序的新的地址。

例如：

```apl
if (i==j)
	f=g+h;
else
	f=g-h;
```

假设变量 *f g h i j* 存储在 *x19 ~ x23* 中，以上代码的汇编是：

```apl
bne x22, x23, Else // go to Else if i != j
add x19, x20, x21 // f=g+H
beq x0, x0, Exit // 退出，条件永久成立 if 0==0 exit
Else: sub x19, x20, x21 // f=g-h (如果i!=j)
Exit:
```

## Loops

### 编译C中典型的loop：

```apl
while (save[i]==k)
    i+=1;
```

假设 *i k* 存储于 *x22 x24* ，数组 *save* 的 *base address* 存储于 *x25*。

首先需要将save[i]从内存中读取到临时寄存器中，在此之前，需要计算save[i]在内存中的地址，由于每一块地址是一个字节(byte) ，因此一个64bits的doubleword需要8个字节，因此计算地址时，i需要乘8，可以通过左移3实现。然后需要标记为loop的指令，这样可以实现循环。

```apl
Loop: slli x10, x22, 3 // temp reg x10=i<<3 (临时寄存器x10=i*8)
add x10, x10, x25 // x10=address of save[i] , 相当于i*8(x25)
ld x9, 0(x10) // temp reg x9=save[i]
bne x9, x24, Exit // if save[i] != k exit
addi x22, x22, 1 // i+=1
beq x0, x0, Loop // if x0==x0 Loop
Exit:
```

## 其他条件指令

除了对比相等性(bne,beq)以外，RISC-V还提供了其他用于对比两个操作数的指令，例如 *less than ($<$) ，less than or equal ($\leq$)， greater than ($>$) ，greater than or equal ($\geq$)* 。

RISC-V提供了不同的指令用于对比符号数和无符号数。*blt* (branch if less than) 当$rs1<rs2$时跳转，*bge* (branch if greater or equal) 当$rs1\geq{rs2}$ 时跳转。等等，这些指令都用于对比二的补码的数值，即符号数。

*bltu* (branch if less than unsigned) ，*bgeu* (branch if greater than or equal , unsigned) 用于对比无符号数。

## 检查边界溢出的简便方法

当检查$0\leq{x}<y$时 (例如数组的下标)，如果把x当作无符号数看待，那么如果x的最高位是1，则在符号数中x<0，在无符号数中x>y (y的最高位一定是0,内存从0开始);如果x>y则无论是不是以符号数都是边界溢出，所以只需要以下一条指令：

```apl
bgeu x20, x11, IndexOutOfBounds // if x20>=x11 or x20<0, goto IndexOutOfBounds
```

# 2.8 Procedures

一个*Procedure* 或者函数(*Function*)是用来更好地组织程序的工具，让程序更加容易理解和复用。

## Procedure (Function)

一种被存储的子程序，基于给定的参数执行特殊的任务。

执行一个Procedure需要执行以下几个步骤：

1. 父程序(Calling program)将参数放置在Procedure可以获取的地方
2. 将控制转移到Procedure
3. 获取Procedure需要的存储资源
4. 执行任务
5. 将结果放在主程序可以获取的地方
6. 将控制权转移至父程序

RISC-V约定以下寄存器用于Procedure：

- *x10-x17*：8个寄存器用于传递参数或者返回结果
- *x1*：一个返回地址寄存器，用于回到原点(调用Procedure的地方)

除了分配这些寄存器，RISC-V包含一个专门给Procedure的指令：在跳转到一个地址的同时将之后接下来要执行的指令地址存储到x1(即调用完Procedure后要执行的指令的地址)-----> ***jump-and-link instruction*** (**jal**)，例如：

```apl
jal x1, ProcedureAddress // jump to ProcedureAddress and write return address to x1
```

## return address

一个地址用于从Procedure返回到父程序(caller)，存储在x1。在RISC-V中使用*jalr* 返回到caller：

```apl
jalr x0, 0(x1) // just branch to instruction address in x1
```

(有点奇怪，但是似乎这条指令到存储在x1地址中取出了指令，因为有0(x1) )

RISC-V包含一个**Program Counter**(程序计数器)用于存储当前执行的指令的地址（虽然更应该叫*instruction address register*，但书上这么说），由于指令是32bits，所以需要4字节，那么相邻的下一条指令的地址则是PC+4，在跳转至Procedure前，jal将PC+4写入x1，以此设置返回地址。

## 使用更多的寄存器（using more registers）

当返回父程序指令地址时，父程序所使用的寄存器必须恢复在调用Procedure之前存储的数据（不能被Procedure覆盖），由于寄存器数量只有32个，所以只能将数据缓存到内存中。

用于实现这样的思想的数据结构是栈（**stack**）。

## Stack(栈)

stack是一种最后进第一个出的队列（last in first out），stack存储在内存之中，其中存储寄存器中的数据，stack需要一个指针指向stack中最新分配的内存。stack pointer在RISC-V中是x2，用于存储栈顶的地址(stack top)。每存储或恢复（删除）一个寄存器的数据，stack都需要调整一个doubleword大小的内存，因为每一个寄存器都是64bits的。将数据存储到stack叫作**push**，从stack移除数据叫作**pop**。

在RISC-V中，stacks grow from higher addresses to lower addresses，也就是说，每存储一个寄存器的数据，stack pointer的地址就减少8，反之亦然。

## 编译一个Leaf Procedure (不调用其他函数的函数)

```c
long long int leaf_example(long long int g,long long int h
                           , long long int i,long long int j)
{
    long long int f;
    f=(g+h)-(i+j);
    return f;
}
```



## Nested Procedures

```c
long long int fact (long long int n)
{
    if (n<1)
        return 1;
    else
        return n*fact(n-1);
}
```

```assembly

```

# 2.10 

