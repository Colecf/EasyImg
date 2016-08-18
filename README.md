# EasyImg
2 file loading, editing, and saving of BMP files

No other libraries requried, just drop EasyImg.cpp and EasyImg.h into your project. It makes a BMPImage class, that you can use like this:

```
#include <iostream>
#include <assert.h>
#include "EasyImg.h"

int main() {
  BMPImage img1(50, 50); //creates blank 50x50 image                             
  for(int i=0; i<50; i++) {
    for(int j=0; j<50; j++) {
      img1.setPixel(i, j, 255, 0, 0); //set the RGB values of a pixel            
    }
  }

  img1.save("out.bmp"); //save to a file                                         

  BMPImage img2; //Creates empty image                                           
  if(!img2.load("out.bmp")) { //loads from file                                  
    std::cout << img2.getError() << std::endl;
    return 1;
  }

  assert(img2.getRed(0, 0) == (char)255);
  assert(img2.getGreen(0, 0) == (char)0);
  assert(img2.getBlue(0, 0) == (char)0);
  assert(img2.getWidth() == 50);
  assert(img2.getHeight() == 50);
}
```
