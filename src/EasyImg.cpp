#include <fstream>
#include <iostream>
#include <sstream>
#include "EasyImg.h"
using namespace std;

void writeUint32(char *where, uint32_t toWrite) {
  where[0] = toWrite >> 0;
  where[1] = toWrite >> 8;  
  where[2] = toWrite >> 16;
  where[3] = toWrite >> 24;
}
void writeUint16(char *where, uint16_t toWrite) {
  where[0] = toWrite >> 0;
  where[1] = toWrite >> 8;
}
uint32_t readUint32(char *where) {
  return where[0] + (where[1]<<8) + (where[2]<<16) + (where[3]<<24);
}
uint32_t readUint16(char *where) {
  return where[0] + (where[1]<<8);
}

BMPImage::BMPImage() {
  width = height = 0;
  red = green = blue = nullptr;
  error = "No Error";
}
BMPImage::BMPImage(int w, int h) {
  width = w;
  height = h;
  red = new char*[h];
  green = new char*[h];
  blue = new char*[h];
  error = "No Error";
  for(int i=0; i<h; i++) {
    red[i] = new char[w];
    green[i] = new char[w];
    blue[i] = new char[w];
  }
}

BMPImage::BMPImage(const BMPImage& other) {
  width = other.width;
  height = other.height;
  error = other.error;
  red = new char*[height];
  green = new char*[height];
  blue = new char*[height];
  for(int i=0; i<height; i++) {
    red[i] = new char[width];
    green[i] = new char[width];
    blue[i] = new char[width];
    for(int j=0; j<width; j++) {
      red[i][j] = other.red[i][j];
      green[i][j] = other.green[i][j];
      blue[i][j] = other.blue[i][j];
    }
  }
}

BMPImage& BMPImage::operator= (const BMPImage& other) {
  cleanup();
  width = other.width;
  height = other.height;
  error = other.error;
  red = new char*[height];
  green = new char*[height];
  blue = new char*[height];
  for(int i=0; i<height; i++) {
    red[i] = new char[width];
    green[i] = new char[width];
    blue[i] = new char[width];
    for(int j=0; j<width; j++) {
      red[i][j] = other.red[i][j];
      green[i][j] = other.green[i][j];
      blue[i][j] = other.blue[i][j];
    }
  }
  return *this;
}

BMPImage::BMPImage(BMPImage&& other) {
  width = other.width;
  height = other.height;
  red = other.red;
  green = other.green;
  blue = other.blue;
  error = other.error;
  other.width = other.height = 0;
  other.red = other.green = other.blue = nullptr;
}
BMPImage& BMPImage::operator= (BMPImage&& other) {
  cleanup();
  width = other.width;
  height = other.height;
  red = other.red;
  green = other.green;
  blue = other.blue;
  error = other.error;
  other.width = other.height = 0;
  other.red = other.green = other.blue = nullptr;
  return *this;
}


void BMPImage::cleanup() {
  for(int i=0; i<height; i++) {
    delete[] red[i];
    delete[] green[i];
    delete[] blue[i];
  }
  delete[] red;
  delete[] green;
  delete[] blue;
  red = green = blue = nullptr;
  width = 0;
  height = 0;
  error = "No error";
}

BMPImage::~BMPImage() {
  cleanup();
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
  char *header = new char[54];
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

  //=========Data===============
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

bool BMPImage::load(std::string filename) {
  cleanup();
  //ios::ate so we start at the end for reading the file's length
  ifstream file(filename, ios::in | ios::binary | ios::ate);
  if(!file.is_open()) {
    error = "Unable to open file "+filename;
    return false;
  }
  streampos size = file.tellg();
  file.seekg(0, ios::beg);
  if(size < 54) {
    error = "Filesize is not the minimum";
    return false;
  }

  //Verify header feild is bmp
  char* buffer = new char[54];
  file.read(buffer, 54);
  if(buffer[0] != 'B' || buffer[1] != 'M') {
    stringstream ss;
    ss << "Not a BMP img, first 2 bytes are ";
    ss << (+buffer[0]) << ", " <<(+buffer[1]);
    error = ss.str();
    return false;
  }

  //Read pixel offset (skip file size in header, we know that)
  uint32_t pixelOffset = readUint32(buffer+10);
  if(pixelOffset >= size) {
    error = "Filesize smaller than pixel offset";
    return false;
  }

  width = readUint32(buffer+18);
  height = readUint32(buffer+22);
  uint16_t bpp = readUint16(buffer+28);
  uint32_t compression = readUint16(buffer+30);
  if(compression != 0 || bpp != 24 ||
     pixelOffset+height*(3*width+(3*width)%4) < size) {
    width = 0;
    height = 0;
    error = "Compression used, not 24 bpp, or filesize too small";
    return false;
  }

  delete[] buffer;
  buffer = new char[(int)size-pixelOffset];
  file.seekg(pixelOffset, ios::beg);
  file.read(buffer, (int)size-pixelOffset);
  red = new char*[height];
  green = new char*[height];
  blue = new char*[height];
  int rowsize = width*3+(4-(width*3)%4);
  for(int i=0; i<height; i++) {
    red[i] = new char[width];
    green[i] = new char[width];
    blue[i] = new char[width];
    for(int j=0; j<width; j++) {
      red[i][j] = buffer[i*rowsize+j*3+2];
      green[i][j] = buffer[i*rowsize+j*3+1];
      blue[i][j] = buffer[i*rowsize+j*3+0];
    }
  }
  delete[] buffer;
  return true;
}

string BMPImage::getError() {
  return error;
}
