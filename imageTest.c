// imageTest - A program that performs some image processing.
//
// This program is an example use of the image8bit module,
// a programming project for the course AED, DETI / UA.PT
//
// You may freely use and modify this code, NO WARRANTY, blah blah,
// as long as you give proper credit to the original and subsequent authors.
//
// João Manuel Rodrigues <jmr@ua.pt>
// 2023

#include <assert.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image8bit.h"
#include "instrumentation.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    error(1, 0, "Usage: imageTest input.pgm output.pgm");
  }

  ImageInit();
  
  printf("# LOAD image");
  InstrReset(); // to reset instrumentation
  Image img1 = ImageLoad(argv[1]);
  if (img1 == NULL) {
    error(2, errno, "Loading %s: %s", argv[1], ImageErrMsg());
  }
  InstrPrint(); // to print instrumentation

  // Try changing the behaviour of the program by commenting/uncommenting
  // the appropriate lines.

  //img2 = ImageCrop(img1, ImageWidth(img1)/4, ImageHeight(img1)/4, ImageWidth(img1)/2, ImageHeight(img1)/2);
  //Image img2 = ImageRotate(img1);
  //img2 = ImageCrop(img2, 0,0,7,7);
  //Image img2 = ImageCrop(img1, 0, 0, ImageWidth(img1), ImageHeight(img1));
  //Image img2 = ImageCrop(img1, 200, 200, 1, 1);
  //if (img2 == NULL) {
    //error(2, errno, "Rotating img2: %s", ImageErrMsg());
  //}

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  

  InstrReset();
  printf("\n\t1 ; 1\n");
  ImageBlur2(img1, 1, 1);

  InstrPrint();
  InstrReset();
  printf("\n\t3 ; 3\n");
  ImageBlur2(img1, 3, 3);

  InstrPrint();
  InstrReset();
  printf("\n\t5 ; 5\n");
  ImageBlur2(img1, 5, 5);

  InstrPrint();
  InstrReset();

  printf("\n\t10 ; 10\n");
  ImageBlur2(img1, 10, 10);

  InstrPrint();
  InstrReset();

  printf("\n\t50 ; 50\n");
  ImageBlur2(img1, 50, 50);

  InstrPrint();
  InstrReset();


  printf("\n\t100 ; 100\n");
  ImageBlur2(img1, 100, 100);

  InstrPrint();
  InstrReset();

  printf("\n\t500 ; 500\n");
  ImageBlur2(img1, 500, 500);

  InstrPrint();
  InstrReset();


  printf("\n\t1000 ; 1000\n");
  ImageBlur2(img1, 1000, 1000);

  InstrPrint();
  InstrReset();


  


  ImageDestroy(&img1);
  //ImageDestroy(&img2);
  return 0;
}

