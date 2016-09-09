#ifndef IC_BMP_IMAGE_H
#define IC_BMP_IMAGE_H

struct bmp_image_s {
  struct image_properties *props;

  unsigned char file_header[14];
  unsigned char info_header[40];
  unsigned char padding[3];
};

#endif
