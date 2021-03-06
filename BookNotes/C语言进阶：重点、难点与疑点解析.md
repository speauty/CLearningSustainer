# C语言进阶：重点、难点与疑点解析(牟海军著)

## 核心概念

### 堆栈

栈, 是硬件, 主要作用表现为一种数据结构, 是只能在一端(栈顶, 另一端为栈底)插入和删除数据的特殊线性表, 在计算机系统的动态内存区域.  关于栈, 有后进先出/先进后出两个说法, 其实都是正确的, 只是关注点不一样, 一个是后入栈的数据, 一个是先入栈的数据. 根据栈的特性, 后入栈的数据总在先入栈的数据上方, 因为栈仅有一个口(出口和入口是统一的), 先入栈的数据要出栈的话, 就必须把在这之后的数据全部出栈.  有点类似从羽毛球桶里面取球和放球的操作, 当然, 另一头是封死的.

从书中看到, "压栈的操作使栈顶的地址减小，弹出的操作使栈顶的地址增大", 这句是存疑的. 也就是说栈的数据从高地址向低地址的走向, 但是现在是虚拟内存地址, 总感觉这里怪怪的, 栈顶的走向应该体系架构相关的.

堆, 是一种动态存储结构, 实际上就是数据段中的自由存储区, 常常用于存储、分配动态数据. 堆中存入的数据地址向增加方向变动. 关于堆, 有先进先出的特性, 也就是一边是入口, 一边是出口, 像管道一样, 两头通, 只是限制每头的入和出. 堆内存通常`malloc()` , `calloc()`, `realloc()`三个函数来分配, 使用`free()`释放.

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *a;
    char *b;
    char *c;
    a = (char *)malloc(sizeof(char));
    b = (char *)malloc(sizeof(char));
    c = (char *)malloc(sizeof(char));
    printf("%p\n", a); // 0x55a165fba2a0
    printf("%p\n", b); // 0x55a165fba2c0
    printf("%p\n", c); // 0x55a165fba2e0
    return 0;
}
```

就实际输出来看(可能这里显示是虚拟内存地址), 这堆内存的分配是从低地址向高地址进行的.  还句话来说, 堆和栈的生长方向相反的, 栈向低地址方向生长, 而堆向高地址方向生长. 

由于这里不懂汇编, 只能从C中的树组打印出来的地址观察一下, 栈的生长方向, 首先需要说一下, 树组的压栈顺序是从右到左.

```c
#include <stdio.h>

int main() {
    int a[] = {1, 2, 3, 4,5 ,66};
    for (int i = 0; i < 6; ++i) {
        printf("%p\n", &a[i]);
        // 0x7fffab21b330
        // 0x7fffab21b334
        // 0x7fffab21b338
        // 0x7fffab21b33c
        // 0x7fffab21b340
        // 0x7fffab21b344
    }
    return 0;
}

```

从右到左, 在输出中, 从下到上, 确实是从高地址到低地址生长的. 

### 全局变量和局部变量

全局变量, 也称外部变量, 在函数体外定义, 不是哪一个函数所特有的. 全局变量又可以分为外部全局变量和静态全局变量, 它们之间的最大区别在于, 使用static存储类别的全局变量只能在被定义的源程序文件中使用, 而使用extern存储类别的全局变量不仅可以在被定义的源程序文件中使用, 还可以被其他源文件中的函数引用.

### 生存期和作用域

生存期, 指变量占用内存或者寄存器的时长. 根据变量存储类别的不同, 在编译的时候, 变量将被存放到动态存储区或静态存储区中, 所以其生存期是由声明时的存储类别所决定的

静态存储区, 存放全局变量和静态变量, 在执行程序前分配存储空间, 占据固定的存储单元

动态存储区, 存放的是函数里的局部变量\函数的返回值\形参等, 它在函数被执行的过程中进行动态分配, 在执行完该函数时自动释放

存储类别和变量

- 自动(auto), 非静态变量的局部变量, 其类型说明符为auto. 在C语言中, 将函数内没有存储类别说明的变量均视为自动变量, 即自动变量可以省去说明符auto
- 寄存器(register), 指定了register存储类别的变量. 只有局部变量才可以定义成寄存器变量. 寄存器的数目是有限的
- 静态(static), 生存期是从程序开始运行到程序运行结束. 静态变量不属于动态存储, 是静态存储. 它只能在被定义的源程序文件中使用, 即它只能被本源程序文件的函数调用, 而不能被其他的源程序文件中的函数调用. 静态局部变量和静态全局变量的定义形式都是在数据类型前加上一个静态存储定义符static
- 外部(extern), 外部存储类别定义方式为在全局变量类型前面加上关键字extern, 如果没有指定全局变量的存储类别, 则默认为extern

### 内部函数和外部函数

内部函数, 在一个源文件中定义的函数只能被该文件中的函数所调用, 而不能被同一程序其他文件中的函数调用

```c
static 返回类型 函数名称(参数)
```

外部函数, 一个函数既可以被同一个源文件中的函数调用, 又可以被同一程序其他文件中的函数调用. 如果定义函数时没有加关键字static或者extern, 那么这种函数也是外部函数

```c
extern 返回类型 函数名称(参数)
```

外部函数和内部函数之间的最大区别莫过于它们的作用范围不同, 内部函数的作用范围是它所在的源文件, 而外部函数的作用范围则不局限于它所在的源文件

### 指针变量

指针变量, 存放地址的变量. 可以把指针理解为指向一块内存区域的起始地址, 指向区域的大小视所指变量的类型而定. 

### 指针数组和数组指针

指针数组, 存放指针的树组

```c
类型 *树组名称[树组长度]
```

树组指针, 指向树组的指针

```c
类型 (*树组名称)[树组长度]
```

### 指针函数和函数指针

指针函数其实是一个简称, 是指带指针的函数, 它本质上是一个函数, 只是返回的是某种类型的指针

```c
类型 *函数名称(参数)
```

函数指针, 从本质上说是一个指针, 只是它指向的不是一般的变量, 而是一个函数. 因为每个函数都有一个入口地址, 函数指针指向的就是函数的入口地址

```c
类型 (*函数名称)(参数)
```

### 传值和传址

传值, 函数调用过程中参数传递的是实参的值, 就是把实参传递给形参. 相当于一个对实参备份的操作, 即对形参的修改只是修改实参的备份, 不会影响到实参

传址, 函数调用过程中参数传递的是地址, 形参和实参共用一个空间, 所以对于形参的修改会影响到实参

### 递归和嵌套

嵌套调用, 在一个函数中去调用另外一个函数, 但是要注意, 可以嵌套调用函数, 但不能嵌套定义函数, 因为C语言的各个函数之间是互相平行的关系, 不存在上下级关系, 所以不能在一个函数中定义另外一个函数

递归调用, 函数在调用的过程中自身既是主调函数，又是被调函数

### 结构体

结构体, 由一系列具有相同类型或不同类型的数据构成的数据集合

### 共用体(联合体)

共用体, 由基本数据结构组合而成, 但是共用体和结构体却有本质区别. 因为结构体中的每个成员都占用存储单元, 所以结构体所占用的内存大小为所有成员各自占用的内存大小之和, 而共用体占用的内存大小由其成员中占用内存最大的那个决定, 所有的成员都占用同一个起始地址和同一段内存空间. 对于共用体变量, 在某一时刻, 只能存储其某一成员的信息

### 位域

位域, 把存储空间中的二进制位划分为几个不同的区域, 并说明每个区域的位数, 每个域有一个域名, 允许在程序中按域名进行操作. 默认情况下, 位域结构中的字节对齐方式由其中占用字节数最大的类型所决定

## 预处理

### 文件的包含方式

#### #include "文件名"

编译时都先在当前目录中查找，如果查找失败，再到系统头文件目录中查找，还查找不到则报错

####  #include ＜文件名＞

如果采用VC++6.0进行编译, 那么会先在系统头文件目录中查找, 若查找失败, 再到当前目录中查找, 还查找不到则报错

如果在Linux环境下采用gcc进行编译, 那么仅在系统头文件目录中查找, 查找不到则报错

### 宏

#### 宏定义

宏定义又称为宏替换，简称宏。它是在预处理阶段用预先定义的字符串替代标识符的过程. 宏定义中的标识符都采用大写

```c
#define 标识符 字符串
```

注意点

1. 宏替换不做语法检查
2. 宏替换通常在文件开头部分, 写在函数的花括号外边, 作用域为其后的程序, 直到用#undef命令终止宏定义的作用域
3. 不要在字符串中使用宏, 如果宏名出现在字符串中, 那么将按字符串进行处理

#### 简单宏替换

简单宏替换在编程中通常用来定义常量. 为常量定义一个宏名, 以便只修改赋值语句中的值就可以实现对程序中所有该宏名出现处的值进行修改.

使用简单宏替换有点

1. 减少不必要的修改, 提升程序的可预读性
2. 提升代码的可移植性

#### 带参数的宏替换

```c
#define 宏名(参数列表) 字符串
```

需要注意以下几点

- 宏名和参数表的括号间不能有空格
- 宏替换只做替换, 不做计算和表达式求解
- 函数调用在编译后程序运行时进行, 并且分配内存. 宏替换在编译前进行, 不分配内存
- 宏的哑实结合(哑实结合类似于函数调用过程中实参替代形参的过程)不存在类型, 也没有类型转换
- 宏展开使源程序变长, 而函数调用则不会

####  嵌套宏替换

 嵌套宏替换, 指在一个宏的定义中使用另外一个宏. 在预处理阶段将对其中的每个宏名进行扩展, 直到宏定义中没有宏名为止

### 条件编译指令的使用

预处理程序提供了条件编译的功能, 用户可以选择性地编译程序, 进而产生不同的目标代码文件

- #if...#else...#endif
- #ifdef...#else...#endif
- #ifndef...#else...#endif

### #pragma指令的使用

#pragma, 设置编译器的状态或指示编译器完成一些特定的动作

- #pragma message("消息"), 消息的打印输出
- #pragma once, 保证头文件只被编译一次
- #pragma hdrstop, 编译头文件到此为止
- #pragma pack(), 取消之前设置的字节对齐方式, 采用默认的4字节对齐. #pragma pack(2)设置2字节对齐
- #pragma warning(), 警告控制. `#pragma warning(disable：M N；once：H；error：K)`表示不显示M号和N号的警告信息, H号警告信息只报告一次, 把K号警告信息作为一个错误来处理

## 指针

不同类型的指针变量所指向内存区域的大小并不相同

大小端模式

- 小端模式, 是数据的低位保存在内存的低地址中, 高位保存在内存的高地址中
- 大端模式, 是数据的低位保存在内存的高地址中, 而高位保存在内存的低地址中