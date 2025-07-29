CEXAMPLE_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD_CXX += $(CEXAMPLE_MOD_DIR)/modmpu6050.cpp

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(CEXAMPLE_MOD_DIR)
CXXFLAGS_USERMOD += -I$(CEXAMPLE_MOD_DIR) -std=c++11

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++
