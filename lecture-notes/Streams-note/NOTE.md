<div align="center">

# Stream Note
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

| 操作 | 读取位置 | 写入位置 |
| :--: | :--: | :--: |
| 查看当前位置 | `tellg()` | `tellp()` | 
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