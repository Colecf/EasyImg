#include "EasyImg.h"
#include <iostream>

BMPImage createImg() {
  BMPImage img(50, 50);
  for(int i=0; i<50; i++) {
    for(int j=0; j<50; j++) {
      if(i==j || i==49-j) {
        img.setPixel(i, j, 0, 0, 0);
      } else {
        img.setPixel(i, j, 255, 255, 255);
      }
    }
  }
  return img;
}

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

  BMPImage move = createImg();
  move.save("move_constructor.bmp");

  BMPImage load;
  if(load.load("move_constructor.bmp")) {
    load.save("loaded.bmp");
  } else {
    std::cout << "Error loading" << std::endl;
    std::cout << load.getError() << std::endl;
  }
  
  std::cout << "Done" << std::endl;
}
