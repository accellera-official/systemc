ARG ALMA_VERSION=8
ARG IMAGE=almalinux:$ALMA_VERSION

FROM $IMAGE

WORKDIR /app

RUN dnf install -y \
    cmake \
    gcc \
    gcc-c++ \
    clang \
    libasan \
    libubsan \
    && dnf clean all

# Copy the current directory contents into the container at /app
COPY . /app
# Setup entrypoint CI script
COPY ./docker/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
