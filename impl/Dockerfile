FROM ubuntu:bionic
LABEL maintainer "kusmirekwiktor@gmail.com"

RUN apt-get update

RUN apt-get install -y -q \
    mercurial \
    g++ scons git \
    libpq-dev libsparsehash-dev \
    wget cmake \
    python3 python3-pip python3-dev \
    zlib1g-dev \
    libboost-all-dev

WORKDIR /tmp

RUN wget http://sourceforge.net/projects/mt4cpp/files/0.03/mt4cpp_0.3.76-1.tar.gz

RUN tar -xzf mt4cpp_0.3.76-1.tar.gz

RUN mv mt4* /usr/local/include

RUN wget https://github.com/google/googletest/archive/release-1.8.0.tar.gz

RUN tar -xzf release-1.8.0.tar.gz

WORKDIR /tmp/googletest-release-1.8.0

RUN mkdir build; \
    cd build; \
    cmake -DBUILD_SHARED_LIBS=ON -G"Unix Makefiles" ..; \
    make; \
    cp -r ../googlemock/include/gmock /usr/local/include; \
    cp -r ../googletest/include/gtest /usr/local/include; \
    cp googlemock/libgmock.so /usr/local/lib;

WORKDIR /tmp

COPY . /tmp/dnaasm/impl

WORKDIR /tmp/dnaasm/impl

RUN cp build_default.ini build_custom.ini

RUN sed -i 's/python3.4/python3.5/g' build_custom.ini

WORKDIR /tmp/dnaasm/impl

# RUN rm cpp/src/olc/SConscript

# RUN rm cpp/src/scfr/SConscript

RUN find /usr/include -name pyconfig.h

RUN scons

RUN cp cpp/dnaasm /usr/local/bin/dnaasm

RUN cp cpp/lib/* /usr/local/lib
