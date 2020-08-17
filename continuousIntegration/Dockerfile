FROM ubuntu:18.04

# Create jenkins
RUN groupadd -g 10000 jenkins && \
        useradd -c "Jenkins user" -d /home/jenkins -u 10000 -g 10000 -m jenkins
# Give jenkins user root privileges
RUN usermod -aG sudo jenkins
RUN su jenkins
WORKDIR /home/jenkins
ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
# update and install dependencies
RUN         apt-get update \
                && apt-get install -y \
                   apt-utils\
                    software-properties-common \
                    g++\
                    ninja-build \
                    vim \
                    git \
                    cmake \
                    qt5-default \
                    libeigen3-dev \
                    python3-dev \
                    python3 \
                    python3-tk \
                    python3-numpy \
                    python3-matplotlib \
                    python3-pytest

RUN mkdir googleTest ; cd googleTest ; mkdir googletest-src googletest-build
RUN cd googleTest/googletest-src; git clone https://github.com/google/googletest.git
RUN cd googleTest/googletest-build ; cmake -G Ninja -DCMAKE_INSTALL_PREFIX=${PWD}/../install ../googletest-src/googletest
RUN cd googleTest/googletest-build ; ninja all install

# Declare here the ENV variables that you wish to be able to overwrite in build time
ARG GTEST_ROOT=/home/jenkins/googleTest/install
ARG GMOCK_ROOT=/home/jenkins/googleTest/install

# Here the sets up the variables to be overwritten if specified value on build time.
ENV GTEST_ROOT=$GTEST_ROOT
ENV GMOCK_ROOT=$GMOCK_ROOT
