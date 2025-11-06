#!/bin/bash
set -eo pipefail
ctest --test-dir build/develop --output-on-failure
ctest --test-dir build/release --output-on-failure
