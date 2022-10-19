# NES Boxxle

NES version of Gameboy's Boxxle.


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