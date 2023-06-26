######################################
# target
######################################
TARGET = init

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C & C++ sources
SOURCES = $(wildcard Src/*) \
$(wildcard Drivers/STM32F1xx_HAL_Driver/Src/*)
C_SOURCES = $(filter %.c,$(SOURCES))
CXX_SOURCES = $(filter %.cpp,$(SOURCES))

# ASM sources
ASM_SOURCES =  \
startup_stm32f103xb.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
CXX = $(GCC_PATH)/$(PREFIX)g++
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# FPU = -mfpu=vfp
FPU = -D__SOFTFP__

# float-abi
# FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# defines
AS_DEFS =
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xB
CXX_DEFS = $(C_DEFS)

# includes
AS_INCLUDES =
C_INCLUDES =  \
-IInc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include
CXX_INCLUDES = $(C_INCLUDES)

# compile gcc flags
ASFLAGS = \
$(MCU) $(AS_DEFS) $(AS_INCLUDES) \
-Wall -fdata-sections -ffunction-sections

CFLAGS += \
$(MCU) $(C_DEFS) $(C_INCLUDES) \
-Wall -fdata-sections -ffunction-sections \
-std=c17

CXXFLAGS += \
$(MCU) $(C_DEFS) $(C_INCLUDES) \
-Wall -fdata-sections -ffunction-sections \
-std=c++2a -fno-exceptions -fno-rtti

# DEBUG = 1
ifdef DEBUG
	CFLAGS += -Og -g -gdwarf-2 -fstack-protector-all -fno-omit-frame-pointer
	CXXFLAGS += -Og -g -gdwarf-2 -fstack-protector-all -fno-omit-frame-pointer
else
	CFLAGS += -Os
	CXXFLAGS += -Os
	LDFLAGS += -s
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103CBTx_FLASH.ld

# libraries
LIBS = -nostdlib -lstdc++
LIBDIR =
LDFLAGS = $(MCU) --specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CXX_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR)
	$(CXX) -c $(CXXFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
c: clean

#######################################
# replication
#######################################
write: all
	st-flash --reset write $(BUILD_DIR)/$(TARGET).bin 0x8000000
w: write

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
