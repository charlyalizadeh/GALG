C = g++
CFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++ -lstdc++fs -std=c++17 -lm
VPATH = src inc lib lib/imgui lib/imgui/backends
SRCDIR = src
INCDIR = inc
OBJDIR = obj
BINDIR = bin
OBJ = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(SRCDIR)/*))) # Project object files
IMGUIOBJ = imgui.o imgui_demo.o imgui_draw.o imgui_impl_opengl2.o imgui_widgets.o imgui_tables.o# ImGUI object files

all: GALG
		
GALG : $(OBJ) $(IMGUIOBJ)
	$(CC) -o $(BINDIR)/$@  $(?:%.o=$(OBJDIR)/%.o) $(CFLAGS)

GALG_DEBUG : $(OBJ) $(IMGUIOBJ)
	$(CC) -o $(BINDIR)/$@  $(?:%.o=$(OBJDIR)/%.o) $(CFLAGS) -g

%.o : %.cpp
	$(CC) -c $< -o $(OBJDIR)/$@

clean :
	rm ./obj/*

build:
	mkdir obj
	mkdir bin

debug: GALG_DEBUG 
