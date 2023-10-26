FROM ubuntu:20.04

RUN apt-get update && apt-get upgrade -y \
    && apt-get install -y build-essential cmake libcrypto++-dev libcrypto++-doc libcrypto++-utils

RUN git clone https://github.com/grpc/grpc.git \
    && cd grpc && git submodule update --init \
    && mkdir build && cd build \
    && cmake .. \
    && make -j \
    && make install

COPY ./* /home/tk-filter

WORKDIR /home/tk-filter