#include <fstream>
#include <iostream>
#include "EasyImg.h"
using namespace std;

void writeUint32(unsigned char *where, uint32_t toWrite) {
  where[0] = toWrite >> 0;
  where[1] = toWrite >> 8;  
  where[2] = toWrite >> 16;
  where[3] = toWrite >> 24;
}
void writeUint16(unsigned char *where, uint16_t toWrite) {
  where[0] = toWrite >> 0;
  where[1] = toWrite >> 8;
}

BMPImage::BMPImage() {
  width = height = 0;
  red = green = blue = NULL;
}
BMPImage::BMPImage(int w, int h) {
  width = w;
  height = h;
  red = new char*[h];
  green = new char*[h];
  blue = new char*[h];
  for(int i=0; i<h; i++) {
    red[i] = new char[w];
    green[i] = new char[w];
    blue[i] = new char[w];
  }
}
BMPImage::~BMPImage() {
  for(int i=0; i<height; i++) {
    delete[] red[i];
    delete[] green[i];
    delete[] blue[i];
  }
  delete[] red;
  delete[] green;
  delete[] blue;
}

void BMPImage::setPixel(int x, int y, char r, char g, char b) {
  if(red && green && blue &&
     x >= 0 && x < width &&
     y >= 0 && y < height) {
    red[y][x] = r;
    green[y][x] = g;
    blue[y][x] = b;
  }
}

void BMPImage::save(string filename) {
  ofstream outfile;
  outfile.open(filename, ios::out | ios::binary | ios::trunc);

  //header will hold both the bitmap file header
  //and the DIB header
  unsigned char *header = new unsigned char[54];
  //======Bitmap file header=====
  header[0] = 'B';
  header[1] = 'M';
  uint32_t fileSize = 54+height*(3*width+(3*width)%4);
  writeUint32(header+2, fileSize);
  writeUint32(header+10, 54);              //pixel array offset

  //==========DIB header=========
  writeUint32(header+14, 40); //dib header size
  writeUint32(header+18, width);  //width of image
  writeUint32(header+22, height);  //height of image
  writeUint16(header+26, 1);  //color planes (must be 1?)
  writeUint16(header+28, 24); //bpp, using 3 chars per pixel means 24
  writeUint32(header+30, 0);  //compression 0=none
  writeUint32(header+34, 0);  //size of pixels, can use 0 when no compression
  writeUint32(header+38, 0);  //pixels per meter, horizontal
  writeUint32(header+42, 0);  //pixels per meter, vertical
  writeUint32(header+46, 0);  //colors in color palette
  writeUint32(header+50, 0);  //number of important colors, 0 for all
  outfile.write((const char*)header, 54);
  delete[] header;
  char zero[] = {0, 0, 0, 0};
  for(int row=0; row<height; row++) {
    int col;
    for(col=0; col<width; col++) {
      outfile.write((const char*)&blue[row][col], 1);
      outfile.write((const char*)&green[row][col], 1);
      outfile.write((const char*)&red[row][col], 1);
    }
    outfile.write(zero, (col*3)%4);
  }
  
  outfile.close();
}

