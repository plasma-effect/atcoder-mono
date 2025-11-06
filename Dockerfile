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

RUN cmake -S . -B build && cmake --build build --parallel
CMD ["ctest", "--test-dir", "build", "--output-on-failure"]
