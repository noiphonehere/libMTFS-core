mkdir -p output
g++ -shared -fPIC -o output/libMTFS-core.so -Iinclude src/Text/*.cpp src/Button/*.cpp src/Camera/*.cpp src/Texture/*.cpp src/Wavefront/*.cpp src/Object/Object.cpp src/Surface/*.cpp -lSDL2_image -lSDL2 `pkg-config --libs --cflags pango cairo pangocairo` -O2 `sdl2-config --cflags` -lGL -DUSE_ARRAYS -DX86_64 -Wno-deprecated-declarations -Wno-unused-result -DNO_DISPLAY_LISTS -DUSE_PRELOADER -DUSE_TEXOBJ_PRELOADING -DUSE_SDL2