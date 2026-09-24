FROM ghcr.io/wiiu-env/devkitppc AS build

RUN dkp-pacman -Syu --needed --noconfirm wut \
 && dkp-pacman -Scc --noconfirm

WORKDIR /work
COPY . .
RUN make

FROM scratch AS artifact
COPY --from=build /work/build/drc-latency-test.rpx /
