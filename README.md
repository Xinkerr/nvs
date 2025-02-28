# 非易失性存储组件 (NVS)

本组件基于Zephyr的NVS实现，经过移植和优化，不再依赖于特定的SDK或芯片平台。它可作为一个独立的模块应用于任何嵌入式系统，提供可靠的非易失性存储支持。

------

## 功能特点

- **FIFO管理的循环缓冲区**：通过FIFO策略管理数据存储，确保高效的空间利用率和数据完整性。
- **支持多种数据类型**：支持存储二进制数据块、字符串、整数、长整型等数据类型及其组合。
- **数据完整性保护**：提供元数据的CRC校验，以及可选的CRC-32校验以确保数据部分的完整性。
- **写入优化**：在写入数据之前检查id-数据对是否有变化，避免不必要的写操作，延长Flash寿命。
- **异常恢复**：初始化过程中检测并忽略元数据不完整或校验错误的数据，确保系统稳定性。
- **Flash保护机制**：避免在可用空间不足的情况下进入频繁擦除的死循环。

------

## 存储机制

### 数据结构

- 每个存储元素由 **元数据**（8字节）和 **数据** 组成。
- 元数据包括：
  - `id`：16位无符号数，用于标识存储元素。
  - `数据偏移`：在扇区中的偏移地址。
  - `数据长度`：数据的字节数。
  - `部分标志`（未使用）。
  - `CRC`：校验元数据的完整性。

### 存储方式

1. 数据写入Flash时，先写入数据部分，再写入元数据部分。
2. 数据在扇区中以以下布局存储：
   - 元数据：从扇区末尾向前存储。
   - 数据：从扇区起始地址向后存储。

### 扇区管理

- 数据会逐个写入当前扇区，直到空间耗尽。
- 当当前扇区满时，组件会准备下一个扇区（先擦除再使用）。
- 在擦除之前，会检查其他扇区中是否存储了当前扇区的id-数据对；如果没有，则会将其拷贝至新扇区。

------

## 使用示例

### 初始化

```c
nvs_init(&nvs_instance, &property, &ops);
```

### 写入数据

```c
nvs_write(&nvs_instance, id, data, data_size);
```

### 读取数据

```c
nvs_read(&nvs_instance, id, buffer, buffer_size);
```

### 删除数据

```c
nvs_delete(&nvs_instance, id);
```

------

欢迎提出建议或贡献代码以优化本组件！