CXX := g++-8
CC := gcc-8

OBJECT_FILES := out/impl/linux/LinuxSocketImpl.o out/impl/linux/LinuxTerminal.o
OBJECT_FILES += out/json/json_reader.o out/json/json_value.o out/json/json_writer.o
OBJECT_FILES += out/Hash.o out/IOWrapper.o out/JTime.o
OBJECT_FILES += out/Menu.o out/Random.o out/ShadowRandom.o
OBJECT_FILES += out/SocketCommon.o out/Terminal.o
OBJECT_FILES += out/TextComponent.o out/UUID.o out/Version.o
OBJECT_FILES += out/UI/GraphicsBase.o out/UI/Shape.o
OBJECT_FILES += out/Database.o out/ThreadLocalRandom.o out/LibraryInfo.o

CC_FLAGS = -g -fvisibility-inlines-hidden -fvisibility=default -std=c11 -fpic -w -fwrapv
COMPILE_FLAGS = -g -fvisibility-inlines-hidden -fvisibility=default -std=c++2a -fpic -w -fpermissive -fwrapv
LINKER_FLAGS = -static-libstdc++ -shared -fpic -flinker-output=dyn -pthread 
LIBS = -lssl
OUTPUT = liblc-cxx.so
INCLUDE = -I./ -I./include
DEFINES = -DLCLIB_CXX_DEFINITION

LIBNAME = -Wl,-soname,liblc-cxx.so

DIRS := out/ out/impl/linux out/json out/UI

all: $(OUTPUT)

out:
	mkdir -p $(DIRS)

$(OUTPUT): out $(OBJECT_FILES)
	$(CXX) $(LINKER_FLAGS) $(LIBNAME) -o $@ $(LIBS) $(OBJECT_FILES) 

install:$(OUTPUT)
	install $(OUTPUT) /usr/lib/
	install --mode=755 -d -v /usr/include/lclib-cxx include/lclib-cxx
	install --mode=755 -d -v /usr/include/json include/json
	cp -R include/lclib-cxx /usr/include
	cp -R include/json /usr/include
	chmod -R 755 /usr/include/json
	chmod -R 755 /usr/include/lclib-cxx

uninstall:
	rm -rf /usr/include/lclib-cxx
	rm -rf /usr/lib/$(OUTPUT)
	rm -f ./install

relink:
	rm -rf $(OUTPUT)
	make $(OUTPUT)


clean:
	rm -rf out
	rm -f $(OUTPUT)
	rm -f init

rebuild:
	make clean
	make $(OUTPUT)

out/%.o: src/%.cpp
	$(CXX) $(COMPILE_FLAGS) -c $(INCLUDE) -o $@ $<

out/%.o: src/%.c
	$(CC) $(CC_FLAGS) -c $(INCLUDE) -o $@ $<
	
	
