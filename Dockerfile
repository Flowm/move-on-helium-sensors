FROM ubuntu:16.04

ENV WORKDIR /opt/mbed
ENV GIT_REPOSITORY https://github.com/Flowm/move-on-helium-sensors

WORKDIR /opt/mbed

RUN set -x \
    && apt-get update \
    && apt-get install -qq -y build-essential gcc-arm-none-eabi git mercurial \
    && apt-get install -qq --no-install-recommends -y python-pip python-setuptools python-dev \
    && apt-get clean -qq
RUN set -x \
    && pip install --upgrade pip \
    && pip install --upgrade setuptools \
    && pip install mbed-cli \
    && apt-get clean -qq

COPY . .

RUN mbed-cli deploy
RUN set -x \
    && cd src && make
