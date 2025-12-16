FROM ubuntu:25.10 AS base

RUN apt-get update && apt-get install -y \
  cmake \
  g++-15 \
  make \
  git \
  wget \
  curl \
  ccache \
  ninja-build \
  && rm -rf /val/lib/apt/lists/*

WORKDIR /tmp
RUN curl -O https://archives.boost.io/release/1.88.0/source/boost_1_88_0.tar.gz
RUN tar -xzf boost_1_88_0.tar.gz
RUN mv boost_1_88_0/boost /usr/local/include/boost

WORKDIR /workspace
