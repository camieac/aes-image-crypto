#ifndef IC_IMAGE_H
#define IC_IMAGE_H

struct pixel {
  int red; //0 -> 255
  int green; //0 -> 255
  int blue; //0 -> 255
};

struct image_properties {
  struct pixel **pixels; //2D array of pixels
  unsigned width;
  unsigned height;
};

struct image_properties * image_properties_allocate_pixels(struct image_properties * image);

#endif //ifdef IIMAGE_H
