#include "EasyImg.h"
#include <iostream>

int main(int argc, char* argv[]) {
  {
    //tests saving and deleting an empty img
    BMPImage img;
    img.save("empty.bmp");
  }

  
  BMPImage img(50, 150);
  for(int i=0; i<50; i++) {
    for(int j=0; j<150; j++) {
      if(i ==j/2) {
        img.setPixel(i, j, 0, 0, 255);
      } else {
        img.setPixel(i, j, 255, 0, 0);
      }
    }
  }

  BMPImage img2 = img;
  for(int i=0; i<50; i++) {
    img2.setPixel(i, 30, 0, 255, 0);
  }

  img.save("out.bmp");
  img2.save("out2.bmp");
  std::cout << "Done" << std::endl;
}
