#include "EasyImg.h"
#include <iostream>

int main(int argc, char* argv[]) {
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
  img.save("out.bmp");
  std::cout << "Done" << std::endl;
}
