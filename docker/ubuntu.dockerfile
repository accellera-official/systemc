ARG UBUNTU_VERSION=20.04
ARG IMAGE=ubuntu:$UBUNTU_VERSION

FROM $IMAGE

WORKDIR /app

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y \
    cmake \
    gcc \
    g++ \
    clang \
    gcovr \
    && rm -rf /var/lib/apt/lists/*

# Copy the current directory contents into the container at /app
COPY . /app
# Setup entrypoint CI script
COPY ./docker/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
