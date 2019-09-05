g++ main.cpp -o example -I../include ../output/libMTFS-core.so `sdl2-config --libs --cflags` `pkg-config --libs --cflags pango cairo pangocairo` -lGL -lGLU -DUSE_SDL2
