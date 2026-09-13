<div align="center">

# Stream Note and Type Note
</div>

---
本笔记为对 CS106L 中 Lecture 2：Stream 部分扩展知识点的记录

---

## Part 1 stringstream 的读写
课程截图：
[stringstream position fubctions](./note-images/stringstream-position-functions.png)

该截图讲解的是**如何查看、移动字符串流中的读写位置**
先记住两个字母：
- `g = get`：读取位置，比如 `istringstream`
- `p = put`：写入位置，比如 `ostringstream`

|      操作      |   读取位置   |   写入位置   |
| :------------: | :----------: | :----------: |
|  查看当前位置  |  `tellg()`   |  `tellp()`   |
| 移动到指定位置 | `seekg(pos)` | `seekp(pos)` |

example:
```txt
字符：  A B C D E
位置：  0 1 2 3 4 5
             ↑
           seekg(3)
```

1. `tellg()` 和 `seekg(pos)`：查看、移动读取位置
  ```c++
    std::istringstream iss("ABCDE");

    char ch;
    iss.get(ch);                 // 读到 'A'
    auto pos = iss.tellg();      // 当前读取位置是 1，下次读 'B'

    iss.seekg(3);                // 移动到位置 3
    iss.get(ch);                 // 读到 'D'

    iss.seekg(pos);              // 回到之前保存的位置 1
    iss.get(ch);                 // 读到 'B'
  ```

2. `tellp()` 和 `seekp(pos)`：查看移动写入位置
   ```c++
    std::ostringstream oss;

    oss << "ABCDE";
    auto pos = oss.tellp();      // 写入位置是 5

    oss.seekp(1);                // 移到 'B' 的位置
    oss << "xy";

    std::cout << oss.str();      // 输出 AxyDE
   ```

3. `streamoff`：表示**移动多少**
   `streamoff(n)` 表示**偏移量**
   ```c++
    std::istringstream iss("ABCDE");

    iss.seekg(1);                    // 当前在 'B' 前
    auto pos = iss.tellg();

    iss.seekg(pos + std::streamoff(2)); // 向后移动 2，来到 'D' 前

    char ch;
    iss.get(ch);                     // 读到 'D'
   ```

## Part 2 State Bits 状态位
状态位分为 4 种：
1. Good bit：准备进行读/写
2. Fail bit：先前的操作**失败**了，**将来所有的操作都被冻结**
3. EOF bit：先前的操作碰见了 `EOF`，到达了缓冲区的末尾
4. Bad bit：底层错误，通常难以恢复

**注意**：
1. `failbit` 表示**这次的格式化读取无法按照目标类型成功完成**，并不代表底层的流坏了，一般的触发条件如下：
    - 没有可读取的类型对象
    - 数字格式不完整
    - 超出类型表示的范围
    - 流已经处于失败的状态

2. 在流读取的时候，对于整数对象，`-`只允许作为整数开头的符号，不能出现在已经读到数字之后

流本身支持**链式赋值**，也就是说：
```cpp
istringstream iss;
iss >> result1 >> result2;   // result* have been declared.
```

输入流和输出流表达式的返回值都是**流本身**，这也是链式操作的背后原理，同时流本身可以被重新解释为 `bool` 类型：
```cpp
if (!(iss >> integer))
    throw std::domain_error("Fail to abstract an integer!")
```

## Part 3 flush 缓冲区刷新
在 C++ 输出流中，数据通常不会每次 `<<` 都立刻写到终端或文件，而是先进入缓冲区，积累到一定程度后再统一输出，以提高性能。

`std::flush`：
`std::flush` 只刷新输出缓冲区，不会输出任何字符：
```cpp
std::cout << "Loading..." >> std::flush;
```
适合需要立即显示内容，但不想换行的场景。

`std::endl`：
`std::endl` 会做两件事：
1. 输出换行符 `\n`
2. 立即刷新输出缓冲区

```cpp
std::cout << "Hello, world" << std::endl;
```
大致等价于：
```cpp
std::cout << "Hello, world\n" << std::flush;
```

`std::endl` 会主动刷新缓冲区，但是频繁刷新缓冲区会严重影响性能，普通换行可以优先选择 `\n`
再要求输出内容立即可见的时候使用 `std::endl` 或 `std::flush`

## Part 4 Manipulators and Pad
**流操纵符（stream manipulators）** 是插入到 `<<` 或 `>>` 表达式中，用来改变流的行为或格式的工具
```cpp
std::cout << std::hex << 255;   // Output: FF / ff
```

常用的操控符主要位于：
```cpp
#include <iostream>     // endl, flush, hex, dec, boolalpha 等
#include <iomanip>      // setw, setfill, setprecision 等
```

常用的流操纵符包括：
1. 换行与刷新：
   ```cpp
   std::endl    // Output '\n', then flush the buffer
   std::flush   // Flush the buffer
   ```
2. 整数进制：
   ```cpp
   std::cout << std::dec << 255 << '\n';    // 255
   std::cout << std::hex << 255 << '\n';    // ff
   std::cout << std::oct << 255 << '\n';    // 377
   ```
   **注意**：这里的设置会持续生效，除非**显式的重新指定**，默认输出进制为十进制
3. 布尔值格式
   默认情况下，布尔值的输出为 `1` 或 `0`
   ```cpp
   std::cout << true;   // 1
   ```
   使用 `std::boolalpha` 后：
   ```cpp
   std::cout << std::boolalpha << true << ' ' << false; // Output: true false
   ```
   恢复：
   ```cpp
   std::cout << std::noboolalpha;
   ```
4. 浮点数格式
   ```cpp
   double value = 12.34567

   std::cout << std::fixed
             << std::setprecision(2)
             << value;      // 12.35
   ```

   常见的浮点操控符：
   ```cpp
   std::fixed           // Fixed notation
   std::setprecision()  // Set the precision
   std::scientific      // Scientific notation
   std::defaultfloat    // Restore the default notation
   ```

字符串字段的宽度和对应内容的填充都有相应的设置：
- `std::setw(n)`
  宽度设置，设置下一个输出项所占的最小宽度：
  ```cpp
  std::cout << '[' << std::setw(6) << 42 << ']';
  /**
   * Output: [    42]
   * There are 4 whitespace front the '42'
   */
  ```
  “宽度”只是最小宽度。如果内容本身更长，并不会被截断：
  ```cpp
  std::cout << std::setw(3) << "hello";
  ```
  同时，`setw` 通常只对紧接着地一个输出项生效：
  ```cpp
  std::cout << std::setw(5) << 1
            << std::setw(5) << 2;
  ```
- `std::setfill(ch)`
  设置宽度不足时使用的填充字符：
  ```cpp
  std::cout << std::setfill('*')
            << std::setw(6)
            << 42;
  // Output: ****42
  ```
  `setfill` 会持续生效，除非**显式恢复**
- 对齐 `left` 和 `right`
  默认情况下是数字**右对齐**
  ```cpp
  std::cout << std::right << std::setw(8) << 42;
  // "      42"
  ```
  可以使用 `left` 来指定**左对齐**
  ```cpp
  std::cout << std::left << std::setw(8) << 42;
  // "42      "
  ```

更多的知识点可以查找[cppreference](https://cppreference.com/cpp/header/ostream)，或看之前的[C++PrimerPlus笔记](https://github.com/hangyu1234/Hakimi-s-Rough-Academic-Journey/blob/main/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%B1%BB/%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80/C%2B%2BPrimerPlus.md)中的第 16.7 节

## Part 5 cin 输入流对象
`cin` 是一个标准输入流对象，通常用于从键盘中读取数据

在 `cin` 工作时，键入的文本会进入 `cin` 的缓冲区中，使用**流插入运算符**来对对应变量进行赋值。

注意：`>>` 会自动忽略所有的前导空白符，读取内容直到到达下一个空白字符之前（这个时候停止位置之后的空白字符仍然在 `cin` 的缓冲区中），也就是说，`cin` 缓冲区中的空白符只有在下一个 `cin` 操作时才会消耗

更详细的内容可以看之前的[C++PrimerPlus笔记](https://github.com/hangyu1234/Hakimi-s-Rough-Academic-Journey/blob/main/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%B1%BB/%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80/C++PrimerPlus.md#168-cin--%E7%9A%84%E8%AF%BB%E5%8F%96%E8%A7%84%E5%88%99)中的 16.8 节。

值得注意的是：
当状态位被设置后，**缓冲区本身不会自动清空**，只是流对象进入了某种错误的状态，后续的读写操作通常会停止

`getline()` 函数：
停止位置在换行符的后面，**并且会自动地删除 `\n`**，更多细节可以看[C++PrimerPlus笔记](https://github.com/hangyu1234/Hakimi-s-Rough-Academic-Journey/blob/main/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%B1%BB/%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80/C%2B%2BPrimerPlus.md)中的 3.4.4 节。
**注意**，`getline()` 很难因为错误而停止，但也不是没有：**如果输入流对象本身出现了问题（被设置了 fail bit），这个时候 `getline()` 会出错而停止**。

## Part 6 pair/tuple 类型
`pair` 和 `tuple` 都是**把若干个类型（可以不同值），打包成一个对象**

### `pair`：固定装两个值
```cpp
#include <utility>
std::pair<double, int> princes{3.4, 5};
```

`pair` 包含两个成员 `pair.first` 和 `pair.second`，分别对应指定的类型的内部对象

`pair` 常用于函数一次返回两个结果：
```cpp
std::pair<int, int> divide(int dividend, int divisor) {
    return {dividend / divisor, dividend % divisor};
}

auto result = divide(17, 5);

std::cout << result.first;   // 3：商
std::cout << result.second;  // 2：余数
```

### `tuple`：可以装任意固定数量的值
```cpp
#include <tuple>
auto values = std::make_tuple(3, 4, "hi");
```
上述代码可以近似理解为:
```cpp
std::tuple<int, int, const char*> values;
```

`tuple` 没有 `.first`，`.second` 成员，只能通过 `std::get<index>` 来访问：
```cpp
std::get<0>(values);  // 3
std::get<1>(values);  // 4
std::get<2>(values);  // "hi"
```

这里的下标必须是**编译期常量**，不能通过 `const + 变量` 的形式。

## Part 7 结构化绑定
**结构化绑定（structured bindings）** 是 C++17 引入的语法，用来把一个**包含多个成员的对象**拆成若干个**具名变量**

### 基本语法
```cpp
auto [variable1, variable2, ...] = expression;
```
比如：
```cpp
std::pair<std::string, int> student{"Alice", 95};

auto [name, score] = student;

std::cout << name << ": " << score << '\n';
```

### auto 的不同使用方式会影响最终效果
1. `auto`：复制
   ```cpp
   std::pair<int, int> point{10, 20};

   auto [x, y] = point;
   x = 100;

   std::cout << point.first;  // 仍然是 10
   ```

2. `auto&`：绑定引用
   ```cpp
   std::pair<int, int> point{10, 20};

   auto& [x, y] = point;
   x = 100;

   std::cout << point.first;    // 100
   ```

3. `const auto&`：只读引用
   ```cpp
   const auto& [x, y] = point;

   std::cout << x << ' ' << y;
   // x = 100;      Error! x is read-only.
   ```

### 可拆解对象
结构化绑定常见的拆解对象有：
1. **数组**（变量数量必须和数组元素一致）
2. `pair`
3. `tuple`
4. **简单结构体**（结构化绑定按照成员的声明顺序进行拆解，因此变量的顺序很重要）