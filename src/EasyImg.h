#ifndef EASYIMG_H
#define EASYIMG_H
#include <string>
#include <stdint.h>

class BMPImage {
 public:
  BMPImage();
  BMPImage(int w, int h);
  BMPImage(const BMPImage&);
  BMPImage& operator= (const BMPImage&);
  ~BMPImage();
  void setPixel(int x, int y, char r, char g, char b);
  void save(std::string filename);
 private:
  void cleanup();
  char **red;
  char **green;
  char **blue;
  int width;
  int height;
};

#endif
