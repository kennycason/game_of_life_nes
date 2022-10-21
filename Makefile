PROJECT_NAME = game_of_life
ROM = $(PROJECT_NAME).nes

CC65_ROOT = tools/cc65
CC = $(CC65_ROOT)/bin/cc65
AS = $(CC65_ROOT)/bin/ca65
LD = $(CC65_ROOT)/bin/ld65

CFLAGS = -t nes -Oirs

INCLUDE = $(CC65_ROOT)/include
ASMINC = $(CC65_ROOT)/libsrc/nes
CONFIG = config/ld65.cfg

SRC = ${PROJECT_NAME}.c
ASMSRC = lib/neslib/crt0.s
OBJS = $(ASMSRC:.s=.o) $(SRC:.c=.o) chr_rom.o

$(ROM): $(CONFIG) $(OBJS)
	$(LD) -C $(CONFIG) $(OBJS) nes.lib -m link.log -o ${PROJECT_NAME}.nes

clean:
	rm -f $(OBJS)
	rm -f $(ROM)
	rm -f *.chr
	rm -f link.log

%.s: %.c
	$(CC) $(CFLAGS) $< --add-source -I $(INCLUDE) -o $@

%.o: %.s
	$(AS) $< -I $(ASMINC) -o $@

%.chr: %.png
	tools/png2chr $<

chr_rom.o: tiles.chr

# Cancel built in rule for .c files.
%.o: %.c
