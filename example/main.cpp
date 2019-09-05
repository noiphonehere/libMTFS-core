/*
 Simple example for how one might go about using libMTFS-core
 
Copyright (c) 2019 Carlo Retta

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 */

#include <SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <TextButton.h>


// Used to break out of the event loop
bool running = true;

// Callback for the button
void buttonCallback(Button *btn) {
  (void)btn;
  running = false;
}

int main(int argc, char **argv) {
  // Initialization for SDL and OpenGL
  SDL_Init(SDL_INIT_VIDEO);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef USE_SDL2
  SDL_Window *window = SDL_CreateWindow("libMTFS-core Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);
  (void) SDL_GL_CreateContext(window);
#else
  SDL_SetVideoMode(800, 600, 0, SDL_OPENGL);
#endif

  //Set up a 3D projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 800.0, 600.0, 0.0);
  glMatrixMode(GL_MODELVIEW);

  // Enable 2D textures and vertex arrays
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_VERTEX_ARRAY);

  // Handle SDL events
  SDL_Event event;

  // Make and set up a text button
  TextButton myButton("Quit");
  myButton.setPosition(300, 300);
  myButton.setSize(50, 32); // Quirk: TextButtons don't know their own size
  myButton.setOnClick(buttonCallback);

  // Handle events, passing clicks to the button
  while(running) {
    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) running = false;
      else if(event.type == SDL_MOUSEBUTTONUP) {
        myButton.processClick(event.button.x, event.button.y);
      }
    }

    // Render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    myButton.Draw();

#ifdef USE_SDL2
    SDL_GL_SwapWindow(window);
#else
    SDL_GL_SwapBuffers();
#endif
  }


#ifdef USE_SDL2
  SDL_DestroyWindow(window);
#endif

  // Cleanup when we're done
  SDL_Quit();
}
