# Scheme of Pr_Filter

[chinese version](./README.CN.md)

## Build & Install

### Execution Environment ( linux )

1. init g++ and cmake
    ```
    $ sudo apt update && sudo apt upgrade -y

    $ sudo apt install build-essential

    $ sudo apt install cmake
    ```
2. init CryptoPP
    ```
    $ sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils

    $ apt-cache pkgnames | grep -i crypto++ 
    ```
3. init grpc
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
The server and client are in binary folder.

## Running Command

### running server
```
$ ./server
```
### running client
```
$ ./client
```

The client contains the following commands：setup, token, resolve and stop.
```
# setup: init EMMt and Xset
$ setup -f ${file-path} -k ${key-lenght}
$ setup -fpath ${file-path} -keylen ${key-lenght}

# token: generate token
$ token -w ${word1} ${word2} ... ${wordn}
$ token -word ${word1} ${word2} ... ${wordn}

# resolve: search in server and resolve
$ resolve
```
- file-path: union file path (relative to client).
- key-length: the length of key (integer).
- wordn: several words (more than two).