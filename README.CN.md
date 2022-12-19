# Pr_Filter方案

## 编译&安装

### 安装环境（linux环境）

1. 安装g++和cmake环境
    ```
    $ sudo apt update && sudo apt upgrade -y

    $ sudo apt install build-essential

    $ sudo apt install cmake
    ```
2. 安装CryptoPP密码库
    ```
    $ sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils

    $ apt-cache pkgnames | grep -i crypto++ 
    ```
3. 安装grpc环境
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
$ mkdir build & cd build

$ cmake ..

$ make
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
```
- file-path 并集文件路径（相对于client）
- key-lenght 密钥长度（整数）
- wordn 若干个关键字