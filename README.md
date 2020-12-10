# Stepmania Advance
![Title screen](scrot.png)
### setup
1. make sure you have libgba and libtonc installed from devkitpro
```
echo -e '[dkp-libs]\nServer = https://downloads.devkitpro.org/packages' | sudo tee -a /etc/pacman.conf > /dev/null
echo -e '[dkp-linux]\nServer = https://downloads.devkitpro.org/packages/linux' | sudo tee -a /etc/pacman.conf > /dev/null
sudo pacman -Sy; and sudo pacman -S libtonc libgba maxmod-gba gba-tools
```
2. run `source devkit/devkit-env.sh` before building
3. run the makefile to build an executable
4. open `stepmania_advance.gba` in mgba or some other GBA emulator
