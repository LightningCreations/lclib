CXX = g++
SOURCE_FILES = src/Hash.cpp src/IOWrapper.cpp src/JTime.cpp
SOURCE_FILES += src/SHA256.cpp src/ShadowRandom.cpp src/Random.cpp
SOURCE_FILES += src/Terminal.cpp src/TextComponent.cpp src/UUID.cpp
SOURCE_FILES += src/StringHelper.cpp src/Version.cpp
SOURCE_FILES += src/nbt/NBTBase.cpp src/nbt/NBTCompound.cpp
SOURCE_FILES += src/nbt/NBTList.cpp src/nbt/NBTLoad.cpp
SOURCE_FILES += src/nbt/NBTPrimitive.cpp
FLAGS = -fpic
LIBS = -lssl
OUTPUT = liblc-cxx.so
INCLUDE = -I./ -I./include

all: $(OUTPUT)

$(OUTPUT): $(SOURCE_FILES)
	$(CXX) $(FLAGS) $(INCLUDE) $(OUTPUT) $(SOURCE_FILES) $(LIBS)

