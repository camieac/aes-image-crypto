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

void bmp_clear_padding(struct bmp_padding_byte_s *padding){
  padding->byte = 0x00;
}

int write_rgb_bmp_image(char * filename, struct image_properties * image){
  int r,g,b;
  int x,y;
  int i;
  FILE *f;
  struct bmp_image_s bmp_image;
  struct bmp_padding_byte_s padding;
  bmp_image.props  = image;
  int filesize = 54 + (sizeof(struct pixel) * bmp_image.props->width * bmp_image.props->height);  //w is your image width, h is image height, both int

  bmp_write_file_header(bmp_image.file_header, filesize);
  bmp_write_info_header(bmp_image.info_header, bmp_image.props->width, bmp_image.props->height);
  bmp_clear_padding(&padding);
  LOG_DEBUG("opening\n");
  f = fopen(filename,"wb");
  fwrite(bmp_image.file_header,1,14,f);
  fwrite(bmp_image.info_header,1,40,f);
  LOG_DEBUG("forring\n");

  for (int j = bmp_image.props->height - 1; j >= 0 ; j--){
    size_t padding_bytes = 0;
    for (int i=0; i < bmp_image.props->width; i++){
      //Write pixel to file
      fwrite(&bmp_image.props->pixels[i][j],3,1,f);
      padding_bytes += 3;
      }
      LOG_DEBUG("bytes in row: %d\n", padding_bytes);
      padding_bytes = 4 - (padding_bytes % 4);
      LOG_DEBUG("padding required: %d\n", padding_bytes);
      fwrite(&padding.byte,sizeof(padding.byte),padding_bytes,f);
      LOG_DEBUG("!\n");
  }
  // for(i=0; i < bmp_image.props->height; i++){
  //   fwrite(bmp_image.props->pixels+(bmp_image.props->width*(bmp_image.props->height-i-1)*3),3,bmp_image.props->width,f);
  //   fwrite(bmp_image.padding,1,(4-(bmp_image.props->width*3)%4)%4,f);
  //   LOG_DEBUG("a\n");
  // }
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
  char * filename = "bmp_image.bmp";
  write_rgb_bmp_image(filename, &image);
  LOG_INFO("%s succesfully created.\n", filename);
}
