# TK-Filter 方案

我们提出了一种新型TK-filter构造，是比Conj-Filter构造泄漏更少的联合查询方案。这是方案的c++实现，使用到gRPC和CryptoPP库。

## 说明

文件目录说明：

| 文件/目录名                  | 文件/目录具体内容        |
|-------------------------|------------------|
| binary (未生成)            | 可执行文件            |
| idl                     | proto3定义文件       |
| include                 | 项目头文件            |
| scripts                 | 脚本文件（生成可测试的并集文件） |
| src                     | 项目源码             |
| testData                | 可测试的并集文件         |
| test                    | 单元测试文件           |
| CMakeLists.txt和common.cmake | cmake配置文件        |

## 编译&安装

### 安装环境（linux环境）

1. 安装 g++ 和 cmake (> 3.5.0) 环境
    ```
    $ sudo apt update && sudo apt upgrade -y

    $ sudo apt install build-essential

    $ sudo apt install cmake
    ```
2. 安装 CryptoPP 密码库
    ```
    $ sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils

    $ apt-cache pkgnames | grep -i crypto++ 
    ```
3. 安装 grpc 环境
    ```
    $ git clone https://github.com/grpc/grpc.git 

    $ cd grpc && git submodule update --init

    $ mkdir build && cd build

    $ cmake ..
   
    $ make -j

    $ sudo make install
    ```
### 编译命令
```
$ mkdir build && cd build

$ cmake ..

$ make -j
```
开启测试则需要替换cmake命令为：
```
$ cmake -DTEST=1 ..
```
其中可执行文件server（服务端）和client（客户端）在当前binary目录下

## 运行命令

### 服务端
```
$ ./server
```

### 客户端
```
$ ./client
```

client下可执行命令包括：setup、token、resolve和stop
```
# setup: 初始化EMMt和Xset表
$ setup -f ${file-path} -k ${key-lenght}
$ setup -fpath ${file-path} -keylen ${key-lenght}

# token: 生成token
$ token -w ${word1} ${word2} ... ${wordn}
$ token -word ${word1} ${word2} ... ${wordn}

# resolve: 在服务端搜索并求解
$ resolve

# stop: 停止程序
$ stop

# conj: 执行conj-filter方案
$ conj f ${file-path} -k ${key-lenght} -w ${word1} ${word2} ... ${wordn}
$ conj -fpath ${file-path} -keylen ${key-lenght} -word ${word1} ${word2} ... ${wordn}

# mlemm: 执行mlemm方案
$ mlemm f ${file-path} -k ${key-lenght} -w ${word1} ${word2} ... ${wordn}
$ mlemm -fpath ${file-path} -keylen ${key-lenght} -word ${word1} ${word2} ... ${wordn}
```
- file-path 并集文件路径（相对于client）
- key-lenght 密钥长度（整数）
- wordn 若干个关键字(需多于两个)

## 测试

### 服务端
```
$ ./server
```

### 客户端
```
$ ./client
```
接下来继续输入：
```
$ setup -f ../testData/test_MM.txt -k 32

$ token -w name age sex

$ resolve

$ conj -f ../testData/test_MM.txt -k 32 -w name age sex

$ mlemm -f ../testData/test_MM_w3.txt -k 32 -w name age sex grade

$ stop
```

### 单元测试
测试文件在当前目录下./test/*，生成的可执行测试文件在当前目录./binary/下。