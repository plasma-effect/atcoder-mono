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

FROM base AS build_acl

WORKDIR /workspace
COPY ac-library ./ac-library

RUN cmake -S ac-library -B build/original -DCMAKE_BUILD_TYPE="Original" \
    -G Ninja \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
RUN cmake -S ac-library -B build/custom -DCMAKE_BUILD_TYPE="Custom" \
    -G Ninja \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
RUN cmake --build build/original --parallel
RUN cmake --build build/custom --parallel

FROM base AS build_competitive

WORKDIR /workspace

COPY ac-library ./ac-library
COPY CMakeLists.txt .
COPY include ./include
COPY unittest ./unittest
COPY scripts ./scripts

RUN cmake -S . -B build/develop -DCOMPETITIVE_BUILD_MODE="Develop" \
    -G Ninja \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
RUN cmake -S . -B build/release -DCOMPETITIVE_BUILD_MODE="Release" \
    -G Ninja \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
RUN cmake --build build/develop --parallel
RUN cmake --build build/release --parallel

FROM ubuntu:25.10 AS runner
RUN apt-get update && apt-get install -y cmake ninja-build
COPY --from=build_acl /workspace/build/original /workspace/build/original
COPY --from=build_acl /workspace/build/custom /workspace/build/custom
COPY --from=build_competitive /workspace/build/develop /workspace/build/develop
COPY --from=build_competitive /workspace/build/release /workspace/build/release

WORKDIR /workspace
CMD ["/bin/bash"]
