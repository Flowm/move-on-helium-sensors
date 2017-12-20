SRC_DIR = $(shell pwd)
SOURCES := $(shell find . -name '*.cpp' -o -name '*.hpp')

BUILD_DIR = $(SRC_DIR)/../BUILD/
GEN_MAKEFILE = ../Makefile
PROJECT = move-on-helium-sensors

.PHONY: build flash clean

build: $(BUILD_DIR)/$(PROJECT).hex Makefile

flash: $(BUILD_DIR)/$(PROJECT).hex
	st-flash --format ihex write $<

clean:
	-rm -f ../Makefile
	-rm -f -r $(BUILD_DIR)

$(GEN_MAKEFILE):
	cd .. && mbed export -i gcc_arm --profile config/develop.json

$(BUILD_DIR)/$(PROJECT).hex: $(GEN_MAKEFILE) $(SOURCES)
	$(MAKE) -C .. -j 8
