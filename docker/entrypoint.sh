#!/bin/bash
set -euxo pipefail

if [[ -z "${SYSTEMC_CI_TARGET}" ]]; then
  echo "SYSTEMC_CI_TARGET environment variable needs to be set"
  exit 1
fi

: ${SYSTEMC_SRC_PATH:="/app"}

# Build with -Werror by default
CXX_FLAGS="-Werror"

case "$SYSTEMC_CI_TARGET" in
  gcc-shared)
    CC=gcc
    CXX=g++
    BUILD_SHARED_LIBRARY=true
    BUILD_REGRESSIONS=false
    ;;
  gcc-static)
    CC=gcc
    CXX=g++
    BUILD_SHARED_LIBRARY=false
    BUILD_REGRESSIONS=false
    ;;
  gcc-shared-regression)
    CC=gcc
    CXX=g++
    BUILD_SHARED_LIBRARY=true
    BUILD_REGRESSIONS=true
    ;;
  clang-shared)
    CC=clang
    CXX=clang++
    BUILD_SHARED_LIBRARY=true
    BUILD_REGRESSIONS=false
    ;;
  clang-static)
    CC=clang
    CXX=clang++
    BUILD_SHARED_LIBRARY=false
    BUILD_REGRESSIONS=false
    ;;
  clang-shared-regression)
    CC=clang
    CXX=clang++
    BUILD_SHARED_LIBRARY=true
    BUILD_REGRESSIONS=true
    ;;
  clang-shared-regression-asan)
    CC=clang
    CXX=clang++
    BUILD_SHARED_LIBRARY=true
    BUILD_REGRESSIONS=true
    CXX_FLAGS="$CXX_FLAGS -fsanitize=address"
    ;;
  clang-shared-regression-ubsan)
    CC=clang
    CXX=clang++
    BUILD_SHARED_LIBRARY=true
    BUILD_REGRESSIONS=true
    CXX_FLAGS="$CXX_FLAGS -fsanitize=undefined"
    ;;
  *)
    echo "Error: Unknown SYSTEMC_CI_TARGET '$SYSTEMC_CI_TARGET'"
    exit 1
    ;;
esac

cd $SYSTEMC_SRC_PATH
cmake -B BUILD/RELEASE-${SYSTEMC_CI_TARGET}/BUILD \
      -DCMAKE_INSTALL_PREFIX=${SYSTEMC_SRC_PATH}/BUILD/${SYSTEMC_CI_TARGET} \
      -DCMAKE_CXX_FLAGS="$CXX_FLAGS" \
      -DCMAKE_C_COMPILER=$CC \
      -DCMAKE_CXX_COMPILER=$CXX \
      -DENABLE_REGRESSION=$BUILD_REGRESSIONS \
      -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBRARY .
cmake --build BUILD/RELEASE-${SYSTEMC_CI_TARGET}/BUILD/ --parallel
cmake --install BUILD/RELEASE-${SYSTEMC_CI_TARGET}/BUILD/
make -j `getconf _NPROCESSORS_ONLN` -C BUILD/RELEASE-${SYSTEMC_CI_TARGET}/BUILD/ check

exit 0
