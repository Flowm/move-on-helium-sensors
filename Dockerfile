FROM ubuntu:16.04

RUN set -x \
    && apt-get update \
    && apt-get install -qq -y build-essential gcc-arm-none-eabi git mercurial \
    && apt-get install -qq --no-install-recommends -y python-pip python-dev \
    && apt-get clean -qq
RUN set -x \
    && pip install --upgrade pip \
    && pip install --upgrade setuptools \
    && pip install mbed-cli \
    && apt-get clean -qq
