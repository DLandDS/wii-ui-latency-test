ifneq ($(filter docker,$(MAKECMDGOALS)),)

.PHONY: docker

docker:
	@docker build --target artifact --output type=local,dest=build .

else

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

.PHONY: all clean

all:
	$(DEVKITPRO)/portlibs/wiiu/bin/powerpc-eabi-cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
	cmake --build build

clean:
	rm -rf build

endif
