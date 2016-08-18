#ifndef EASYIMG_H
#define EASYIMG_H
#include <string>
#include <stdint.h>

class BMPImage {
 public:
  BMPImage();
  BMPImage(int w, int h);
  BMPImage(const BMPImage&);              //copy constructor
  BMPImage& operator= (const BMPImage&);  //copy assignment
  BMPImage(BMPImage&&);                   //move constructor
  BMPImage& operator= (BMPImage&&);       //move assignment
  ~BMPImage();
  void setPixel(int x, int y, char r, char g, char b);
  void save(std::string filename);
  bool load(std::string filename);
  std::string getError();
 private:
  void cleanup();
  char **red;
  char **green;
  char **blue;
  int width;
  int height;
  std::string error;
};

#endif
