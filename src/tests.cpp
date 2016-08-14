#include "EasyImg.h"
#include <iostream>

int main(int argc, char* argv[]) {
  unsigned char **pixels = new unsigned char*[50];
  for(int i=0; i<50; i++) {
    pixels[i] = new unsigned char[50*3];
    for(int j=0; j<50; j++) {
      pixels[i][j*3+0] = 255;
      pixels[i][j*3+1] = 0;
      pixels[i][j*3+2] = 0;
    }
  }
  writeBMP("out.bmp", pixels, 50, 50);
  std::cout << "Done" << std::endl;
}
