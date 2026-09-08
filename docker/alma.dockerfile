ARG ALMA_VERSION=8
ARG IMAGE=almalinux:$ALMA_VERSION

FROM $IMAGE

WORKDIR /app

ARG ALMA_VERSION=$ALMA_VERSION
RUN dnf install -y \
    cmake \
    gcc \
    gcc-c++ \
    clang \
    libasan \
    libubsan \
    && dnf clean all

# Installing gtest is version-dependent. 8 is mostly normal, though we need an
# extra repo:
#
# TODO: can these both work via just enabling epel-release?
RUN [ "$ALMA_VERSION" -eq 8 ] && dnf config-manager --set-enabled powertools || exit 0
RUN [ "$ALMA_VERSION" -eq 9 ] && dnf install -y epel-release || exit 0

RUN dnf update -y || exit 0
RUN dnf install -y gtest-devel gmock-devel || exit 0

# Copy the current directory contents into the container at /app
COPY . /app
# Setup entrypoint CI script
COPY ./docker/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
