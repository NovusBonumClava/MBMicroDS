TARGET   := MBMicroDS
SOURCES  := source
INCLUDES := include

ARCH := -mthumb -mthumb-interwork

CFLAGS := -g -Wall -O2 -mcpu=arm946e-s \
          -fomit-frame-pointer -ffast-math \
          $(ARCH) -I$(CURDIR)/include \
          -I/usr/include/nds \
          -DARM9 -D__NDS__

LDFLAGS := -specs=ds_arm9.specs -g $(ARCH)

LIBS := -lnds9

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export OFILES := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o) $(SFILES:.s=.o)

.PHONY: all clean

all: $(OUTPUT).nds

$(OUTPUT).nds: $(OUTPUT).arm9
	ndstool -c $@ -9 $<

$(OUTPUT).arm9: $(OUTPUT).elf
	$(OBJCOPY) -O binary $< $@

$(OUTPUT).elf: $(OFILES)
	$(CC) $(CFLAGS) -Wl,-Map,$(notdir $*.map) $(OFILES) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(OUTPUT).elf $(OUTPUT).arm9 $(OUTPUT).nds
