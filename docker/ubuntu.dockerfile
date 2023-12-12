ARG UBUNTU_VERSION=20.04
ARG IMAGE=ubuntu:$UBUNTU_VERSION

FROM $IMAGE

WORKDIR /app

RUN apt-get update && \
    apt-get install -y \
    cmake \
    gcc \
    g++ \
    && rm -rf /var/lib/apt/lists/*

# Copy the current directory contents into the container at /app
COPY . /app

RUN cmake -B BUILD/RELEASE/BUILD -DCMAKE_INSTALL_PREFIX=$PWD/BUILD/RELEASE .
RUN cmake --build BUILD/RELEASE/BUILD/ --parallel

CMD ["/bin/bash", "-c", "make -j `nproc` -C BUILD/RELEASE/BUILD/ check"]
