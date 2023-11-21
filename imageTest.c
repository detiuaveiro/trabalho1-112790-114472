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
  Image img2 = ImageCrop(img1, 80, 80, 1, 1);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);

  int px;
  int py;

  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n1, \n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 80, 80, 4, 4);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n10, 10\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 80, 80, 40, 40);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n100, 100\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 80, 80, 100, 100);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n250, 250\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 80, 80, 200, 200);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n100, 100\n");

  InstrPrint();

  //NOVA COLUNAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  InstrReset();

  img2 = ImageCrop(img1, 240, 240, 1, 1);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);

  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n1, \n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 240, 240, 4, 4);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n10, 10\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 240, 240, 40, 40);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n100, 100\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 240, 240, 100, 100);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n250, 250\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 240, 240, 200, 200);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n200, 200\n");

  InstrPrint();

  //NOVA COLUNAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  InstrReset();

  img2 = ImageCrop(img1, 639, 479, 1, 1);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);

  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n1, \n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 636, 476, 4, 4);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n10, 10\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 600, 440, 40, 40);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n100, 100\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 540, 380, 100, 100);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n250, 250\n");

  InstrPrint();

  InstrReset();

  img2 = ImageCrop(img1, 440, 280, 200, 200);
  if (img2 == NULL) {
    error(2, errno, "Rotating img2: %s", ImageErrMsg());
  }

  //ImageNegative(img2);
  //ImageThreshold(img2, 100);
  //ImageBrighten(img2, 1.3);
  
  ImageLocateSubImage(img1, &px, &py, img2);
  if (ImageSave(img2, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  
  /*
  ImageBlur(img1, argv[3], argv[4]);          //img, dx, dy
  if (ImageSave(img1, argv[2]) == 0) {
    error(2, errno, "%s: %s", argv[2], ImageErrMsg());
  }
  */

  printf("\n200, 200\n");

  InstrPrint();



  ImageDestroy(&img1);
  ImageDestroy(&img2);
  return 0;
}

