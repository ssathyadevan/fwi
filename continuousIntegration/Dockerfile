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
                    python3-pytest \
                    python3-pip \
                    python3-skimage

RUN pip install junit-xml-output
