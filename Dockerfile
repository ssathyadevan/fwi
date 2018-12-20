FROM ubuntu:18.04

# Create jenkins
RUN groupadd -g 10000 jenkins && \
        useradd -c "Jenkins user" -d /home/jenkins -u 10000 -g 10000 -m jenkins
# Give jenkins user root privileges
RUN usermod -aG sudo jenkins
RUN su jenkins
WORKDIR /home/jenkins
# update and install dependencies
RUN         apt-get update \
                && apt-get install -y \
                    software-properties-common \
                    wget \
                    g++\
                    make \
                    vim \
                    git \
                    cmake \
                    libeigen3-dev \


#RUN add-apt-repository ppa:ubuntu-toolchain-r/test -y
#RUN apt-get update && apt-get -y install gcc-6 g++-6
#RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
#RUN gcc -v


RUN mkdir googleTest ; cd googleTest ; mkdir googletest-src googletest-build
RUN cd googleTest/googletest-src; git clone https://github.com/google/googletest.git
RUN cd googleTest/googletest-build ; cmake -DCMAKE_INSTALL_PREFIX=${PWD}/../install ../googletest-src/googletest
RUN cd googleTest/googletest-build ; make all install -j8

# Declare here the ENV variables that you wish to be able to overwrite in build time
ARG GTEST_ROOT=/home/jenkins/googleTest/install
ARG EIGEN3_ROOT=/usr/lib/cmake/eigen3

# Here the sets up the variables to be overwritten if specified value on build time.

ENV GTEST_ROOT=$GTEST_ROOT
ENV EIGEN3_ROOT=$EIGEN3_ROOT

