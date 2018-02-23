PROJECT := move-on_helium_sensors
SRC_DIR := $(shell pwd)
BUILD_DIR := $(SRC_DIR)/../BUILD
FLASH_DIR := $(SRC_DIR)/../artifacts
GEN_MAKEFILE := ../Makefile
SOURCES := $(shell find . -name '*.cpp' -o -name '*.hpp')

DEPLOY_HOST ?= frcy.org
DEPLOY_PORT ?= 40106
DEPLOY_USER ?= deploy
DEPLOY_TARGET ?= $(DEPLOY_USER)@$(DEPLOY_HOST)

.PHONY: build flash rflash rtail deploy clean

build: $(BUILD_DIR)/$(PROJECT).hex Makefile
flash: $(FLASH_DIR)/$(PROJECT).hex
	st-flash --format ihex write $<

deploy: rflash rtail
rflash: $(FLASH_DIR)/$(PROJECT).hex
	scp -P $(DEPLOY_PORT) $< $(DEPLOY_TARGET):
	ssh -p $(DEPLOY_PORT) $(DEPLOY_TARGET) "st-flash --format ihex write $(notdir $<)"
rtail:
	ssh -p $(DEPLOY_PORT) $(DEPLOY_TARGET) "mqttserial"
clean:
	-rm -f ../Makefile
	-rm -rf $(BUILD_DIR)
	-rm -rf $(FLASH_DIR)

$(GEN_MAKEFILE):
	cd .. && mbed-cli deploy
	cd .. && mbed-cli export -i gcc_arm --profile config/develop.json

$(BUILD_DIR)/$(PROJECT).hex: $(GEN_MAKEFILE) $(SOURCES)
	$(MAKE) -C .. -j 8

# Artifacts for ci build
.PHONY: artifacts
artifacts: $(FLASH_DIR)/$(PROJECT).hex
$(FLASH_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).hex
	mkdir -p $(FLASH_DIR)
	cp $< $(FLASH_DIR)/
	cp $(basename $<).bin $(FLASH_DIR)/
	cp $(basename $<).elf $(FLASH_DIR)/
