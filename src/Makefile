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
	ssh -p $(DEPLOY_PORT) $(DEPLOY_TARGET) "st-flash --reset --format ihex write $(notdir $<)"
rtail:
	ssh -p $(DEPLOY_PORT) $(DEPLOY_TARGET) "stty -F /dev/ttyACM0 9600 && cat /dev/ttyACM0"
clean:
	-rm -f ../Makefile
	-rm -rf $(BUILD_DIR)
	-rm -rf $(FLASH_DIR)

$(GEN_MAKEFILE):
	cd .. && mbed export -i gcc_arm --profile config/develop.json
$(BUILD_DIR)/$(PROJECT).hex: $(GEN_MAKEFILE) $(SOURCES)
	$(MAKE) -C .. -j 8

# Artifacts for ci build
.PHONY: artifacts cideploy ciflash citail
artifacts: $(FLASH_DIR)/$(PROJECT).hex
$(FLASH_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).hex
	mkdir -p $(FLASH_DIR)
	cp $< $(FLASH_DIR)/
	cp $(basename $<).bin $(FLASH_DIR)/
	cp $(basename $<).elf $(FLASH_DIR)/

cideploy: ciflash citail
ciflash:
	scp -P $(DEPLOY_PORT) $(FLASH_DIR)/$(PROJECT).hex $(DEPLOY_TARGET):
	ssh -p $(DEPLOY_PORT) $(DEPLOY_TARGET) "st-flash --reset --format ihex write $(PROJECT).hex"
citail:
	timeout --preserve-status --foreground 20 ssh -p $(DEPLOY_PORT) $(DEPLOY_TARGET) "stty -F /dev/ttyACM0 9600 && cat /dev/ttyACM0"
