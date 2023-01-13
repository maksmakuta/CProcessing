# CProcessing

Just processing, but in C++   
Most sketches are compatible, but library not fully ported   
Uses own OpenGL 3.3 renderer (beta)  
Write and test in QtCreator, VIM, NeoVIM, VScode, Notepad and more...

### Dependencies

 - OpenGL     
 - GLFW       
 - glm        
 - Freetype2  

### How to run

 ```
 git clone https://github.com/maksmakuta/CProcessing.git
 cd CProcessing
 mkdir build
 cd build
 cmake ..
 make -j8 (or your number of cpus)
 ./sketch
 ```
 
 or
 
 ```
  g++ sketch.cpp -o app -std=c++20 `pkg-config freetype2 --cflags --libs`  -lGL -lGLEW -lglfw
 ```
 
### How to install

 ```
 git clone https://github.com/maksmakuta/CProcessing.git
 cd CProcessing
 mkdir build
 cd build
 cmake ..
 sudo make install
 ```


### What's work

 - [x] Window making
 - [x] OpenGL making
 - [x] Drawing (partially)
   - [x] 2D
     - [x] Simple shapes 
     - [x] Complex shapes (via beginShape()/endShape()) 
     - [x] Textures/Images
     - [x] Matrix functions 
   - [x] Font Rendering (beta 2)
 - [x] Input  (partially)
   - [x] Mouse input (almost DONE)
   - [x] Keyboard input (partially)
 - [x] Output
   - [x] print some data into stdout (beta)

  # What's will not ported

  - PDF rendering
  - 3D rendering 
  - most Processing libraries 
  - JSON and XML parsers
  - any composite types (use types from std)
  - file management (std::fstream works very well)

### Limitations

 Sketch must have setup() and draw() functions  
 For keyboard key listener need write ``` #define USE_KEYS ```  before CProcessing header and implement ``` void keyPressed(){} ``` function  
 For keyboard keycodes use GLFW keycodes

### Credits

I use this repos for modernize my library

  - https://github.com/whackashoe/cprocessing
  - https://github.com/cchaine/processing-cpp
  - https://github.com/processing/processing4
 
## Author
  
  Maks Makuta ``` maksmakuta ```  2023 (C)     
  MIT Licence   
