#ifndef IC_BMP_IMAGE_H
#define IC_BMP_IMAGE_H

struct bmp_image_s {
  unsigned char file_header[14];
  unsigned char info_header[40];

  struct image_properties *props;
};

struct bmp_padding_byte_s {
  unsigned char byte;
};

#endif
