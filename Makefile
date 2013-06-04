CC = g++
IFLAGS = -I. -I$(DGEN) -I$(MISC) -I$(CHAR) -I$(WORL) -I$(ASCII)
CFLAGS = -Wall -Wno-unused-variable -Wno-sign-compare $(IFLAGS) -g -c 
LDFLAGS = -Wall -Wno-unused-variable -Wno-sign-compare $(IFLAGS) -g

ASCII = lib/SDL-ASCII-Template
WORL = src/World
DGEN = src/World/dungeon_gen
MISC = src/misc_classes
CHAR = src/character_classes
PATHS = $(DGEN) $(MISC) $(CHAR) $(WORL) $(ASCII)
VPATH= $(DGEN):$(MISC):$(CHAR):$(WORL):$(ASCII)
LIBS =  -lSDLmain -lSDL

SRC = $(foreach path,$(PATHS),$(wildcard $(path)/*.cpp)) src/main.cpp
HEADERS = $(foreach path,$(PATHS),$(wildcard $(path)/*.h))
OBJS = $(patsubst %.cpp,build/%.o,$(notdir $(SRC)))


all : gdbroguelike
	touch roguelike; echo "#!/bin/bash" > roguelike;
	echo "echo run | gdb gdbroguelike" >> roguelike; 
	chmod +x roguelike

$(OBJS): | build

build :
	mkdir build

gdbroguelike : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

build/main.o : src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp -o $@


build/ASCII_Lib.o : def.h ASCII_Lib.h ASCII_Lib.cpp
	$(CC) $(CFLAGS) $(ASCII)/ASCII_Lib.cpp -o $@

build/int_point.o : int_point.h int_point.cpp
	$(CC) $(CFLAGS) $(MISC)/int_point.cpp -o $@

build/DungeonBuilder.o : terrain_defs.h def.h ASCII_Lib.h ASCII_Lib.cpp DungeonBuilder.h DungeonBuilder.cpp int_point.h Room.h Dungeon.h
	$(CC) $(CFLAGS) $(DGEN)/DungeonBuilder.cpp -o $@

build/Room.o : Room.h Room.cpp int_point.h
	$(CC) $(CFLAGS) $(DGEN)/Room.cpp -o $@

build/Character.o : Dungeon.h ASCII_Lib.h ASCII_Lib.cpp def.h Character.h Character.cpp 
	$(CC) $(CFLAGS) $(CHAR)/Character.cpp -o $@

build/Main_Character.o : def.h ASCII_Lib.h ASCII_Lib.cpp Character.h Main_Character.cpp
	$(CC) $(CFLAGS) $(CHAR)/Main_Character.cpp -o $@

build/Dungeon.o :  Dungeon.h Dungeon.cpp int_point.h Room.h terrain_defs.h
	$(CC) $(CFLAGS) $(DGEN)/Dungeon.cpp -o $@

clean :
	rm -r build roguelike gdbroguelike

what :
	echo $(OBJS)
