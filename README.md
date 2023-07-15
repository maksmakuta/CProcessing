# CProcessing

Port Processing language to C++  
Now project are fully rewrited to provides full support of Processing API and support of few rendering APIs, like OpenGL and Vulkan.  
Use [this](https://github.com/maksmakuta/CProcessing/commit/059adef3b518facb7c7068402baa8c4fdcd9bbc9) commit for stable version

### Dependencies

 - OpenGL / Vulkan
 - GLAD (included)
 - GLFW (included)
 - stb  (included)

### What's work

 - [x] Window making
 - [-] Backend
   - [-] OpenGL (3.3)
   - [ ] Vulkan (1.2+, in future)
 - [ ] Drawing
   - [ ] 2D
     - [ ] Simple shapes 
     - [ ] Complex shapes (via beginShape()/endShape()) 
     - [ ] Textures/Images
     - [ ] Matrix functions
   - [ ] Font Rendering
 - [ ] Input
   - [ ] Mouse input
   - [ ] Keyboard input
 - [ ] Output
   - [ ] print some data into stdout (beta)

### Credits

I use this repos for modernize my library

  - https://github.com/whackashoe/cprocessing
  - https://github.com/cchaine/processing-cpp
  - https://github.com/processing/processing4
 
## Author
  
  Maks Makuta ``` maksmakuta ```  2023 (C)     
  MIT Licence   
