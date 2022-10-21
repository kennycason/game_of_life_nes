# NES - Jon Conway's Game of Life

Programmed in C using NES Lib.

#### Pinwheel
<img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pinwheel_1.png?raw=true" width="25%" title="Pinwheel"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pinwheel_2.png?raw=true" width="25%" title="Pinwheel"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pinwheel_3.png?raw=true" width="25%" title="Pinwheel"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pinwheel_4.png?raw=true" width="25%" title="Pinwheel"/>

#### Pulsar
<img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pulsar_1.png?raw=true" width="33%" title="Pulsar"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pulsar_2.png?raw=true" width="33%" title="Pulsar"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_pulsar_3.png?raw=true" width="33%" title="Pulsar"/>

#### Glider
<img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_glider_1.png?raw=true" width="20%" title="Glider"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_glider_2.png?raw=true" width="20%" title="Glider"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_glider_3.png?raw=true" width="20%" title="Glider"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_glider_4.png?raw=true" width="20%" title="Glider"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_glider_5.png?raw=true" width="20%" title="Glider"/>

#### Random
<img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_1.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_2.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_3.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_4.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_5.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_6.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_7.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_1_8.png?raw=true" width="25%" title="Random"/>

#### Random
<img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_1.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_2.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_3.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_4.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_5.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_6.png?raw=true" width="25%" title="Random"/><img src="https://github.com/kennycason/game_of_life_nes/blob/main/output/image/game_of_life_2_7.png?raw=true" width="25%" title="Random"/>

Notes:
- NES limits the number of tiles that can be rendered to the screen. This causes some tiles to disappear between renders. The state is still preserved.
- V2 will use VRAM for better graphics performance.

## Tools

- cc65
- chr2png
- png2chr
- nsf2data
- test2data

### Build Toolchain + NES File

#### Checkout the cc65 submodule
```
git submodule update --init --recursive`
```

#### Build cc65
```
cd tools/cc65
make
```

#### Build the other tools
```
cd tools
make
```

#### Build the NES ROM
```
make
```

#### Open NES ROM with FCEUX NES Emulator
```
fceux game_of_life.nes
```

## Resources + Referenced Projects

- https://github.com/slembcke/neslib-template
- https://github.com/sebastiandine/openNES-Snake
- http://shiru.untergrund.net/articles/programming_nes_games_in_c.htm
- https://nesdoug.com/
