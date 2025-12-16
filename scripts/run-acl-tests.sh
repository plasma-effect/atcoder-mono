#!/usr/bin/bash
set -eu
cd $(dirname $0)/..

IMAGE_NAME=cpp-test
PROJECT_ROOT=$(pwd)
BUILD_DIR="${PROJECT_ROOT}/build"
CCACHE_DIR="${HOME}/.ccache"

mkdir -p "${BUILD_DIR}"
mkdir -p "${CCACHE_DIR}"

for MODE in 'Original' 'Custom'; do
DIR=$(echo "${MODE}" | tr [A-Z] [a-z])
docker run --rm \
  -v "${PROJECT_ROOT}:/workspace" \
  -v "${BUILD_DIR}:/workspace/build" \
  -v "${CCACHE_DIR}:/root/.ccache" \
  ${IMAGE_NAME} \
    cmake -S ac-library -B build/${DIR} -DCMAKE_BUILD_TYPE="${MODE}" \
    -G Ninja \
    -DCMAKE_C_COMPILER_LAUNCHER=ccache \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache

docker run --rm \
  -v "${PROJECT_ROOT}:/workspace" \
  -v "${BUILD_DIR}:/workspace/build" \
  -v "${CCACHE_DIR}:/root/.ccache" \
  ${IMAGE_NAME} \
  cmake --build build/${DIR} --parallel

docker run --rm  \
  -v "${BUILD_DIR}:/workspace/build" \
  ${IMAGE_NAME} \
  ctest --test-dir build/${DIR} --output-on-failure
done
