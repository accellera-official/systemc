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

RUN cmake -B BUILD/RELEASE/BUILD -DCMAKE_INSTALL_PREFIX=$PWD/BUILD/RELEASE .
RUN cmake --build BUILD/RELEASE/BUILD/ --parallel

CMD ["/bin/bash", "-c", "make -j `nproc` -C BUILD/RELEASE/BUILD/ check"]
