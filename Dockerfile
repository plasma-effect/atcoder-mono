FROM ubuntu:25.10

RUN apt-get update && apt-get install -y \
  cmake \
  g++-15 \
  make \
  git \
  wget \
  curl \
  && rm -rf /val/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .
COPY include ./include
COPY unittest ./unittest
COPY scripts ./scripts

RUN cmake -S . -B build/develop -DCOMPETITIVE_BUILD_MODE="Develop"
RUN cmake -S . -B build/release -DCOMPETITIVE_BUILD_MODE="Release"
RUN cmake --build build/develop --parallel
RUN cmake --build build/release --parallel

CMD ["./scripts/execute_test.sh"]
