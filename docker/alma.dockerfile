ARG ALMA_VERSION=8
ARG IMAGE=almalinux:$ALMA_VERSION

FROM $IMAGE

WORKDIR /app

RUN dnf install -y \
    cmake \
    gcc \
    gcc-c++ \
    && dnf clean all

# Copy the current directory contents into the container at /app
COPY . /app

RUN cmake -B BUILD/RELEASE/BUILD -DCMAKE_INSTALL_PREFIX=$PWD/BUILD/RELEASE -DCMAKE_CXX_FLAGS="-Werror" .
RUN cmake --build BUILD/RELEASE/BUILD/ --parallel
RUN cmake -B BUILD/RELEASE_STATIC/BUILD -DCMAKE_INSTALL_PREFIX=$PWD/BUILD/RELEASE -DCMAKE_CXX_FLAGS="-Werror" -DBUILD_SHARED_LIBS=OFF .
RUN cmake --build BUILD/RELEASE_STATIC/BUILD/ --parallel

CMD ["/bin/bash", "-c", "make -j `nproc` -C BUILD/RELEASE/BUILD/ check && make -j `nproc` -C BUILD/RELEASE_STATIC/BUILD/ check"]
