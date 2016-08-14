#include <fstream>
#include <iostream>
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



void writeBMP(string filename, unsigned char** pixels, uint32_t w, uint32_t h) {
  ofstream outfile;
  outfile.open(filename, ios::out | ios::binary | ios::trunc);

  //header will hold both the bitmap file header
  //and the DIB header
  unsigned char *header = new unsigned char[54];
  //======Bitmap file header=====
  header[0] = 'B';
  header[1] = 'M';
  uint32_t fileSize = 54+h*(3*w+(3*w)%4);
  writeUint32(header+2, fileSize);
  uint32_t pixArrayOffset = 54;
  writeUint32(header+10, pixArrayOffset);

  //==========DIB header=========
  writeUint32(header+14, 40); //dib header size
  writeUint32(header+18, w);  //width of image
  writeUint32(header+22, h);  //height of image
  writeUint16(header+26, 1);  //color planes (must be 1?)
  writeUint16(header+28, 8);  //bpp, using 1 char per pixel means 8
  writeUint32(header+30, 0);  //compression 0=none
  writeUint32(header+34, 0);  //size of pixels, can use 0 when no compression
  writeUint32(header+38, 0);  //pixels per meter, horizontal
  writeUint32(header+42, 0);  //pixels per meter, vertical
  writeUint32(header+46, 0);  //colors in color palette
  writeUint32(header+50, 0);  //number of important colors, 0 for all
  outfile.write((const char*)header, 54);
  delete[] header;
  char zero = 0;
  for(int row=0; row<h; row++) {
    int col;
    for(col=0; col<w; col++) {
      outfile.write((const char*)&pixels[col][row+2], 1);
      outfile.write((const char*)&pixels[col][row+1], 1);
      outfile.write((const char*)&pixels[col][row+0], 1);
    }
    while(col%4 != 0) {
      outfile.write(&zero, 1);
      col++;
    }
  }
  
  outfile.close();
}
