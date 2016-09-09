#include "image.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct image_properties * image_properties_allocate_pixels(struct image_properties * image){
    int **arr;
    int i;
    image->pixels = (struct pixel **)malloc(sizeof(struct pixel *) * image->width);
    image->pixels[0] = (struct pixel *)malloc(sizeof(struct pixel) * image->height * image->width);
    for(i = 0; i < image->width; i++)
        image->pixels[i] = (*image->pixels + image->height * i);
  // printf("1\n");
  //   for (i = 0; i < r; i++)
  //       for (j = 0; j < c; j++)
  //           image->pixels[i][j].red = 9;  // OR *(*(arr+i)+j) = ++count
  // printf("1\n");
  //   for (i = 0; i <  r; i++)
  //       for (j = 0; j < c; j++)
  //           printf("%d \n", image->pixels[i][j]);

    // return 0;
  ////////////////////////////////////////
  // printf("1\n");
  // image->pixels = (struct pixel **) malloc(image->width * sizeof(struct pixel *));
  // image->pixels[0] = (struct pixel *) malloc(image->width * image->height * sizeof(struct pixel));
  // printf("3\n");
  //
  // memset(image->pixels,0,sizeof(*image->pixels));
  // printf("4\n");
  // for(i = 0; i < image->width; i++){
  //   for(j = 0; j < image->height; j++){
  //     printf("test[%d][%d]: %d\n", i, j, image->pixels[i][j].red);
  //   }
  // }

  return image;
}
