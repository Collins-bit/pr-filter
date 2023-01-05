# Scheme of Pr_Filter

[chinese version](./README.CN.md)

Our work proposing a new construction PR-Filter for conjunctive queries with less leakage than conj-Filter. This is the c++ implementation of the scheme, using gRpc and CryptoPP libraries.

## Build & Install

### Execution Environment ( linux )

1. Init g++ and cmake (> 3.5.0)
    ```
    $ sudo apt update && sudo apt upgrade -y

    $ sudo apt install build-essential

    $ sudo apt install cmake
    ```
2. Init CryptoPP
    ```
    $ sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils

    $ apt-cache pkgnames | grep -i crypto++ 
    ```
3. Init grpc
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
To enable the test, you need to replace cmake with,
```
$ cmake -DTEST=1 ..
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

# stop: Stop the program
$ stop

# conj: execute conj-filter scheme
$ conj -f ${file-path} -k ${key-lenght} -w ${word1} ${word2} ... ${wordn}
$ conj -fpath ${file-path} -keylen ${key-lenght} -word ${word1} ${word2} ... ${wordn}
```
- file-path: union file path (relative to client).
- key-length: the length of key (integer).
- wordn: several words (more than two).

## Testing

### server 
```
$ ./server
```

### client
```
$ ./client
```
continue to enter,
```
$ setup -f ../testData/test.txt -k 32

$ token -w name age sex

$ resolve

$ conj f ../testData/test.txt -k 32 -w name age sex

$ stop
```