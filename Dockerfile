FROM ghcr.io/wiiu-env/devkitppc:20260504@sha256:68785e7d75b070b0aea03272b61f36dd2fddfda4f442cdf0e27b168bd36a09fb AS build

WORKDIR /work
COPY . .
RUN make

FROM scratch AS artifact
COPY --from=build /work/build/drc-latency-test.rpx /
