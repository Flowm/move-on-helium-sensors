# MOVE-ON Helium Sensors

## Setup

### Requirements
- GNU Arm Embedded Toolchain
- mbed-cli
- stlink

### MacOS
```
brew cask install gcc-arm-embedded
brew install direnv
sudo easy_install pip
pip install virtualenv

echo 'eval "$(direnv hook bash)' > ~/.bashrc
echo 'eval "$(direnv hook zsh)' > ~/.zshrc
```

New shell in project directory:
```
direnv allow
pip install mbed-cli
mbed-cli new .
```

### Linux

## Compiling
In `src` dir:
```
make
```
