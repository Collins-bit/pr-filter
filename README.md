# Scheme of Pr_Filter

[chinese version](./README.CN.md)

## Build & Install

### Environment ( linux )

1. g++ and cmake
    ```
    $ sudo apt update && sudo apt upgrade -y

    $ sudo apt install build-essential

    $ sudo apt install cmake
    ```
2. CryptoPP
    ```
    $ sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils

    $ apt-cache pkgnames | grep -i crypto++ 
    ```
3. grpc
    ```
    $ git clone https://github.com/grpc/grpc.git 

    $ cd grpc && git submodule update --init

    $ mkdir build && cd build

    $ cmake ..

    $ make -j

    $ sudo make install
    ```
### Building
```
$ mkdir build & cd build

$ cmake ..

$ make
```
