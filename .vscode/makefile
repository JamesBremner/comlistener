#source file search paths
VPATH = ./src 

	# compiler include search paths
INCS =  \
	-I../windex/include \
	-I../raven-set \

LIBS = \
	-lgdiplus -lgdi32 -lcomdlg32 -lstdc++fs \

static_link_flags = -static-libstdc++ -static-libgcc -static -lComctl32

# folders
ODIR = ./.vscode/obj

OPT = -g
CCflags = -std=c++17 $(OPT) -D_mingw_

# sources
OBJ_comlistener = \
	$(ODIR)/comListener.o 
OBJ_comexplorer = \
	$(ODIR)/comExplorer.o 

### COMPILE

$(ODIR)/%.o: %.cpp  
	g++  $(CCflags) \
	-c -o $@ $< $(INCS)

comlistener: $(OBJ_comlistener)
	g++ \
	-o bin/comListener.exe  \
	$^ $(OPT) $(static_link_flags) $(LIBS)

comexplorer: $(OBJ_comexplorer)
	g++ \
	-o bin/comExplorer.exe  \
	$^ $(OPT) $(static_link_flags) $(LIBS)

.PHONY: clean
clean:
	del /q .vscode\\obj\\*.o

rebuild : clean comlistener comexplorer


