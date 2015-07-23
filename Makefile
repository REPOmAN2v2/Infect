PACKAGE = infect
VERSION = 2.0.2
DATE = $(shell date "+%b%y")

EXE = $(PACKAGE)

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CXXFLAGS = -Wall -Wextra -g -std=c++11
else
	CXXFLAGS = -O2 -march=native -std=c++11
endif

LDFLAGS = -lncurses -lconfig++
INCLUDES = -I"src/" -I"Cursed-engine/"

CXXFILES = $(shell find src -type f -name '*.cpp')
OBJECTS = $(CXXFILES:.cpp=.o)

ENGINE_DIR = Cursed-engine/engine
ENGINE_CXXFILES = $(shell find $(ENGINE_DIR) -type f -name '*.cpp')
ENGINE_OBJECTS = $(ENGINE_CXXFILES:.cpp=.o)

DEFINES = -DVERSION=\""$(VERSION)"\" \
		  -DPACKAGE=\""$(PACKAGE)"\" \
		  -DDATE=\""$(DATE)"\"

all: dirs $(EXE)

run: all
	./bin/$(EXE)

dirs: 
	@test -d bin || mkdir bin

$(EXE): $(OBJECTS) $(ENGINE_OBJECTS)
	$(CXX) $(OBJECTS) $(ENGINE_OBJECTS) -o bin/$(EXE) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@ $(DEFINES) $(INCLUDES)

$(ENGINE_DIR)/%.o: $(ENGINE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@ $(DEFINES) $(INCLUDES)

clean:
	rm -f $(OBJECTS) $(ENGINE_OBJECTS)
	rm -f bin/$(EXE)

.PHONY: default clean check dist distcheck install rebuild uninstall
