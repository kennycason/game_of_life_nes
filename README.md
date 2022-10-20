# NES Dev

Misc NES Projects

## Programs:

- Jon Conway's Game of Life


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


## Resources + Referenced Projects

- https://github.com/slembcke/neslib-template
- https://github.com/sebastiandine/openNES-Snake
- http://shiru.untergrund.net/articles/programming_nes_games_in_c.htm