ifneq ($(DEVKITPRO),)
export DEVKITARM := $(DEVKITPRO)/devkitARM
endif

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment")
endif

include $(DEVKITARM)/ds_rules

TARGET   := MBMicroDS
SOURCES  := source
INCLUDES := include

ARCH := -mthumb -mthumb-interwork

CFLAGS := -g -Wall -O2 -march=armv5te -mtune=arm946e-s \
          -fomit-frame-pointer -ffast-math \
          $(ARCH) -I$(CURDIR)/include \
          -I$(DEVKITPRO)/libnds/include \
          -DARM9 -DNDS

CXXFLAGS := $(CFLAGS) -fno-rtti -fno-exceptions

LDFLAGS := -specs=ds_arm9.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS := -lnds9 -lfat

LIBDIRS := $(DEVKITPRO)/libnds

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

CFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export OFILES := $(CFILES:.c=.o) $(CPPFILES:.cpp=.o) $(SFILES:.s=.o)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean

all: $(OUTPUT).nds

$(OUTPUT).nds: $(OUTPUT).arm9
	ndstool -c $@ -9 $<

$(OUTPUT).arm9: $(OUTPUT).elf
	$(OBJCOPY) -O binary $< $@

$(OUTPUT).elf: $(OFILES)
	$(LD) $(LDFLAGS) $(OFILES) $(LIBPATHS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(OUTPUT).elf $(OUTPUT).arm9 $(OUTPUT).nds

