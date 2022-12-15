# Pr_Filter方案 - GRPC交互

## 编译&运行

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
其中build目录下的server为服务端，client为客户端