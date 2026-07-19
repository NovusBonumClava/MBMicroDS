TARGET   := MBMicroDS
SOURCES  := source
INCLUDES := include

CC       := arm-none-eabi-gcc
LD       := arm-none-eabi-gcc
OBJCOPY  := arm-none-eabi-objcopy

ARCH     := -mthumb -mthumb-interwork
CFLAGS   := -g -Wall -O2 -mcpu=arm946e-s -fomit-frame-pointer -ffast-math \
            $(ARCH) -I$(CURDIR)/include -I$(DEVKITPRO)/libnds/include \
            -DARM9 -D__NDS__
LDFLAGS  := -specs=ds_arm9.specs -g $(ARCH)
LIBS     := -lnds9

CFILES   := $(wildcard $(SOURCES)/*.c)
OFILES   := $(CFILES:.c=.o)

all: $(TARGET).nds

$(TARGET).nds: $(TARGET).arm9
	ndstool -c $@ -9 $<

$(TARGET).arm9: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OFILES)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(TARGET).elf $(TARGET).arm9 $(TARGET).nds
