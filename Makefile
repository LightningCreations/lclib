CXX := clang++
CC := clang

INCLUDE_PATH := include

CXXHEAD := $(foreach ipath,$(INCLUDE_PATH),$(wildcard $(ipath)/**/*.hpp) $(wildcard $(ipath)/**/*.inl))
CCHEAD := $(foreach ipath,$(INCLUDE_PATH),$(wildcard $(ipath)/**/*.h))



OBJECT_FILES := out/impl/linux/LinuxSocketImpl.o out/impl/linux/LinuxTerminal.o
OBJECT_FILES += out/json/json_reader.o out/json/json_value.o out/json/json_writer.o
OBJECT_FILES += out/Hash.o out/IOWrapper.o out/JTime.o
OBJECT_FILES += out/Menu.o out/Random.o out/ShadowRandom.o
OBJECT_FILES += out/SocketCommon.o out/Terminal.o
OBJECT_FILES += out/TextComponent.o out/UUID.o out/Version.o
OBJECT_FILES += out/UI/GraphicsBase.o out/UI/Shape.o
OBJECT_FILES += out/Database.o out/ThreadLocalRandom.o
OBJECT_FILES += out/UniqueRandomEngine.o

LINFO_OBJ := out/LibraryInfo.o

CC_FLAGS = -g -fvisibility-inlines-hidden -fvisibility=default -std=c11 -w -fwrapv
COMPILE_FLAGS = -g -fvisibility-inlines-hidden -fvisibility=default -std=c++17 -w -fpermissive -fwrapv
LINKER_FLAGS = -static-libstdc++ -shared -flinker-output=dyn -pthread
LIBS = -lssl
OUTPUT = liblc.so
INCLUDE = $(foreach ipath,$(INCLUDE_PATH),-I$(ipath))
DEFINES = -DLCLIB_CXX_DEFINITION

LIBNAME = -Wl,-soname,liblc.so

DIRS := out/ out/UI/ out/impl/ out/impl/linux/

.PHONY: all FORCE install uninstall clean rebuild relink cxxheaders cheaders out version
.DEFAULT: all
.IGNORE: $(DIRS)
.PRECIOUS: Makefile $(DIRS)
.DELETE_ON_ERROR:

all: $(OUTPUT)

version:
	@echo $(CXX) version
	@$(CXX) --version
	@echo $(CC) verison
	@$(CC) --version

FORCE: ;

$(OUTPUT): $(OBJECT_FILES) $(LINFO_OBJ)
	$(CXX) $(LINKER_FLAGS) $(LIBNAME) -o $@ $(LIBS) $^

out/: 
	mkdir $@/
	$(MAKE) $(DIRS)
	
out/%/: out
	mkdir $@



install:$(OUTPUT)
	install $(OUTPUT) /usr/lib/
	install --mode=755 -d -v /usr/include/lclib include/lclib
	cp -R include/lclib /usr/include
	chmod -R 755 /usr/include/lclib
	cp -R include/json /usr/include
	install --mode=755 -d -v /usr/include/json include/json
	chmod -R 755 /usr/include/json
	install --mode=755 -d -v /usr/include/detail include/lclib/detail
	cp -R include/detail /usr/include
	chmod -R 755 /usr/include/detail

uninstall:
	rm -rf /usr/include/lclib
	rm -rf /usr/lib/$(OUTPUT)

relink:
	rm -rf $(OUTPUT)
	$(MAKE) $(OUTPUT)


clean:
	rm -rf $(OBJECT_FILES)
	rm -rf $(LINFO_OBJ)
	rm -f $(OUTPUT)
	rm -f init

rebuild:
	$(MAKE) clean
	$(MAKE) $(OUTPUT)

cxxheaders: cheaders $(INCLUDE_PATH) $(CXXHEAD)

cheaders: $(INCLUDE_PATH) $(CCHEAD)

out/%.o: src/%.cpp $(D@) cxxheaders
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<

out/%.o: src/%.c $(D@) cheaders
	$(CC) $(CC_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<
	
$(LINFO_OBJ): $(LINFO_OBJ:out/%.o=src/%.cpp) cxxheaders out/ $(OBJECT_FILES)
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<

