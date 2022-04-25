# CProcessing

Just processing, but in C++   
Most sketches are compatible, but library not fully ported   
Uses own OpenGL 3.3 renderer (beta)  
Write and test in QtCreator  

### Dependencies

 - OpenGL (GL/glew.h)
 - GLFW   (GLFW/glfw3.h)
 - glm    (glm/glm.hpp)

### What's work

 - [x] Window making
 - [x] OpenGL making
 - [ ] Drawing (partially)
   - [ ] 2D
     - [x] Simple shapes (lines,rectangle,circle,arc,ellipse)
     - [x] Complex shapes (via beginShape()/endShape())  (partially)
     - [ ] Textures/Images
   - [ ] 3D
     - [ ] Simple 3D shapes (Box,Sphere)
     - [ ] Textures/Images
   - [ ] Font Rendering 
 - [ ] Input  (partially)
   - [x] Mouse input (partially)
   - [x] Keyboard input (partially)
 - [ ] Output
   - [ ] print some data into stdout
   

### Limitations

 Sketch must have setup() and draw() functions 

### Credits

I use this repos for modernize my library

  - https://github.com/whackashoe/cprocessing
  - https://github.com/cchaine/processing-cpp
 
## Author
  
  Maks Makuta ``` maksmakuta ```  2022 (C)     
  MIT Licence   
