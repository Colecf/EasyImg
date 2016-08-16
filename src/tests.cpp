#include "EasyImg.h"
#include <iostream>

int main(int argc, char* argv[]) {
  unsigned char **pixels = new unsigned char*[150];
  for(int i=0; i<150; i++) {
    pixels[i] = new unsigned char[50*3];
    for(int j=0; j<50; j++) {
      pixels[i][j*3+1] = 0;
      if(i ==j) {
        pixels[i][j*3+0] = 0;
        pixels[i][j*3+2] = 255;
      } else {
        pixels[i][j*3+2] = 0;
        pixels[i][j*3+0] = 255;
      }
    }
  }
  writeBMP("out.bmp", pixels, 150, 50);
  std::cout << "Done" << std::endl;
}
