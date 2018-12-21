CC := gcc-8
CXX := g++-8
INCLUDE_PATH := include

CXXHEAD := $(foreach ipath,$(INCLUDE_PATH),$(wildcard $(ipath)/**/*.hpp) $(wildcard $(ipath)/**/*.inl))
CCHEAD := $(foreach ipath,$(INCLUDE_PATH),$(wildcard $(ipath)/**/*.h))



OBJECT_FILES := out/impl/linux/LinuxSocketImpl.o out/impl/linux/LinuxTerminal.o
OBJECT_FILES += out/Hash.o out/IOWrapper.o out/JTime.o
OBJECT_FILES += out/Menu.o out/Random.o out/ShadowRandom.o
OBJECT_FILES += out/SocketCommon.o out/Terminal.o
OBJECT_FILES += out/TextComponent.o out/UUID.o out/Version.o
OBJECT_FILES += out/UI/GraphicsBase.o out/UI/Shape.o
OBJECT_FILES += out/Database.o out/ThreadLocalRandom.o
OBJECT_FILES += out/UniqueRandomEngine.o

LINFO_OBJ := out/LibraryInfo.o

CC_DIALECT = -std=c11
CXX_DIALECT = -std=c++17

CC_FLAGS = -g -fvisibility-inlines-hidden -fvisibility=default -fpic $(CC_DIALECT) -w -fwrapv
COMPILE_FLAGS = -g -fvisibility-inlines-hidden -fvisibility=default -fpic $(CXX_DIALECT) -w -fpermissive -fwrapv
LINKER_FLAGS = -static-libstdc++ -shared -flinker-output=dyn -pthread -fpic
LIBS = -lssl -lcrypto
OUTPUT = liblc.so
INCLUDE = $(foreach ipath,$(INCLUDE_PATH),-I$(ipath))
DEFINES = -DLCLIB_CXX_DEFINITION

LIBNAME = -Wl,-soname,liblc.so

DIRS := out out/UI out/impl out/impl/linux out/json

BASE_DIR := out

define gethead0
$(wordlist 2,$(words $(1)),$(1)) 
endef

define gethead
$(call gethead0,$(shell $(CXX) -MM $(CXX_DIALECT) $(INCLUDE) $(1))) 
endef

.PHONY: all FORCE install uninstall clean rebuild relink version
.DEFAULT: all
.PRECIOUS: Makefile $(DIRS)
.SECONDEXPANSION:


all: $(DIRS) $(OUTPUT)

version:
	@echo $(CXX) version
	@$(CXX) --version
	@echo $(CC) verison
	@$(CC) --version

FORCE: ;

$(OUTPUT): $(OBJECT_FILES) $(LINFO_OBJ)
	$(CXX) $(LINKER_FLAGS) $(LIBNAME) -o $@ $^ $(LIBS)

$(BASE_DIR):
	mkdir $(BASE_DIR)
	
$(BASE_DIR)/%/: $(BASE_DIR)
	mkdir -p $@

install:$(OUTPUT)
	install $(OUTPUT) /usr/lib/
	install --mode=755 -d -v /usr/include/lclib include/lclib
	cp -R include/lclib /usr/include
	chmod -R 755 /usr/include/lclib
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
	rm -f $(CXXHEAD:%.hpp=%.hpp.gch)
	rm -f $(CCHEAD:%.h=%.h.gch)

rebuild:
	$(MAKE) clean
	$(MAKE) $(OUTPUT)


out/%.o: src/%.cpp $$(D@) $(CXXHEAD:%.hpp=%.hpp.gch) $(CCHEAD:%.h=%.h.gch)
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<

out/%.o: src/%.cpp $$(D@) $(CCHEAD:%.h=%.h.gch)
	$(CC) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<

$(LINFO_OBJ): $(LINFO_OBJ:out/%.o=src/%.cpp) out/ $(OBJECT_FILES)
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<

%.hpp.gch: %.hpp
	$(CXX) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<

%.h.gch: %.h 
	$(CC) $(COMPILE_FLAGS) $(DEFINES) -c $(INCLUDE) -o $@ $<
