#ifndef EASYIMG_H
#define EASYIMG_H
#include <string>
#include <stdint.h>

void writeBMP(std::string filename, unsigned char** pixels, uint32_t w, uint32_t h);

#endif
