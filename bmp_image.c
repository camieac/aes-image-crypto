#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "bmp_image.h"
#include "logging.h"

void bmp_write_file_header(unsigned char * file_header, int filesize){
  memset(file_header, 0x00, sizeof(*file_header));
  file_header[0] = 'B';
  file_header[1] = 'M';
  file_header[2] = (unsigned char) (filesize);
  file_header[3] = (unsigned char) (filesize >> 8);
  file_header[4] = (unsigned char) (filesize >> 6);
  file_header[5] = (unsigned char) (filesize >> 24);
  file_header[10] = 0x40;
}

void bmp_write_info_header(unsigned char * info_header, int width, int height){
  info_header[0] = 0x28;
  info_header[4] = (unsigned char) (width);
  info_header[5] = (unsigned char) (width >> 8);
  info_header[6] = (unsigned char) (width >> 16);
  info_header[7] = (unsigned char) (width >> 24);

  info_header[8] = (unsigned char) (height);
  info_header[9] = (unsigned char) (height >> 8);
  info_header[10] = (unsigned char) (height >> 16);
  info_header[11] = (unsigned char) (height >> 24);
  info_header[12] = 0x01;
  info_header[14] = 0x16;
}

void bmp_write_padding(unsigned int *padding){
  memset(padding, 0x00, sizeof(*padding));
}

int write_rgb_bmp_image(char * filename, struct image_properties * image){
  int r,g,b;
  int x,y;
  int i;
  FILE *f;
  struct bmp_image_s bmp_image;
  bmp_image.props  = image;
  int filesize = 54 + (sizeof(struct pixel) * bmp_image.props->width * bmp_image.props->height);  //w is your image width, h is image height, both int

  LOG_DEBUG("filesize: %d\n", filesize);
  LOG_DEBUG("filesize: %d\n", sizeof(struct bmp_image_s) - sizeof(struct image_properties *) + (sizeof(struct pixel) * bmp_image.props->width * bmp_image.props->height));

  bmp_write_file_header(bmp_image.file_header, filesize);
  bmp_write_info_header(bmp_image.info_header, bmp_image.props->width, bmp_image.props->height);
  bmp_write_padding(bmp_image.padding);

  f = fopen(filename,"wb");
  fwrite(bmp_image.file_header,1,14,f);
  fwrite(bmp_image.info_header,1,40,f);
  for(i=0; i < bmp_image.props->height; i++){
    fwrite(bmp_image.props->pixels+(bmp_image.props->width*(bmp_image.props->height-i-1)*3),3,bmp_image.props->width,f);
    fwrite(bmp_image.padding,1,(4-(bmp_image.props->width*3)%4)%4,f);
  }
  fclose(f);
}
void main() {
  LOG_DEBUG("Creating a bmp file with random greyscale pixels\n");
  struct image_properties image;
  image.width = 255;
  image.height = 255;
  LOG_DEBUG("Allocating\n");
  image_properties_allocate_pixels(&image);
  LOG_DEBUG("Allocated\n");
  int i;
  for (int i=0; i < image.width; i++){
      for (int j=0; j < image.height; j++){
        int r = rand() % 256;
        image.pixels[i][j].red = r;
        image.pixels[i][j].green = r;
        image.pixels[i][j].blue = r;
      }
  }
  LOG_DEBUG("Pixels ready\n");

  write_rgb_bmp_image("bmp_image.bmp", &image);
}
