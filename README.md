# MOVE-ON Helium Sensors

## Setup

### Requirements
- GNU Arm Embedded Toolchain
- mbed-cli
- stlink

### MacOS
```
brew cask install gcc-arm-embedded stlink openocd
brew install direnv
sudo easy_install pip
pip install virtualenv

echo 'eval "$(direnv hook bash)' > ~/.bashrc
echo 'eval "$(direnv hook zsh)' > ~/.zshrc
```

### Linux
```
sudo apt install gcc-arm-none-eabi openocd
sudo apt install python-pip python-dev build-essential direnv
pip install virtualenv

echo 'eval "$(direnv hook bash)' > ~/.bashrc
echo 'eval "$(direnv hook zsh)' > ~/.zshrc
```

### Mbed-cli
New shell in project directory:
```
direnv allow
pip install mbed-cli
mbed-cli deploy
```

## Development
### Compile
In `src` dir:
```
make
```

### Developing in SW4STM32 (Eclipse)
1. `mbed export -i sw4stm32 --profile config/develop.json`
2. `File` -> `Open Projects from File System` (In Eclipse)
3. Choose the `move-on-helium-sensors` project.

## Reference
### Build targets
From the `src` directory:

| Command                 | Description                                             |
| ----------------------- | ------------------------------------------------------- |
| make build (default)    | Generate toplevel makefile and build project sources    |
| make clean              | Remove build artifacts and toplevel makefile            |
| make flash              | Upload firmware to locally attached uC with st-flash    |
| make rflash             | Upload firmware to uC attached to remote ci host        |
| make rtail              | Tail serial output from uC attached to remote ci host   |
| make deploy             | Combines rflash and rtail                               |
