/// image8bit - A simple image processing module.
///
/// This module is part of a programming project
/// for the course AED, DETI / UA.PT
///
/// You may freely use and modify this code, at your own risk,
/// as long as you give proper credit to the original and subsequent authors.
///
/// João Manuel Rodrigues <jmr@ua.pt>
/// 2013, 2023

// Student authors (fill in below):
// NMec:  Name:
// 112790: João Su
// 114472: José Fernandes
// 
// Date: 2023-11-17
//

#include "image8bit.h"

#include <assert.h>
#include <error.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "instrumentation.h"

// The data structure
//
// An image is stored in a structure containing 3 fields:
// Two integers store the image width and height.
// The other field is a pointer to an array that stores the 8-bit gray
// level of each pixel in the image.  The pixel array is one-dimensional
// and corresponds to a "raster scan" of the image from left to right,
// top to bottom.
// For example, in a 100-pixel wide image (img->width == 100),
//   pixel position (x,y) = (33,0) is stored in img->pixel[33];
//   pixel position (x,y) = (22,1) is stored in img->pixel[122].
// 
// Clients should use images only through variables of type Image,
// which are pointers to the image structure, and should not access the
// structure fields directly.

// Maximum value you can store in a pixel (maximum maxval accepted)
const uint8 PixMax = 255;

// Internal structure for storing 8-bit graymap images
struct image {
  int width;
  int height;
  int maxval;   // maximum gray value (pixels with maxval are pure WHITE)
  uint8* pixel; // pixel data (a raster scan)
};


// This module follows "design-by-contract" principles.
// Read `Design-by-Contract.md` for more details.

/// Error handling functions

// In this module, only functions dealing with memory allocation or file
// (I/O) operations use defensive techniques.
// 
// When one of these functions fails, it signals this by returning an error
// value such as NULL or 0 (see function documentation), and sets an internal
// variable (errCause) to a string indicating the failure cause.
// The errno global variable thoroughly used in the standard library is
// carefully preserved and propagated, and clients can use it together with
// the ImageErrMsg() function to produce informative error messages.
// The use of the GNU standard library error() function is recommended for
// this purpose.
//
// Additional information:  man 3 errno;  man 3 error;

// Variable to preserve errno temporarily
static int errsave = 0;

// Error cause
static char* errCause;

/// Error cause.
/// After some other module function fails (and returns an error code),
/// calling this function retrieves an appropriate message describing the
/// failure cause.  This may be used together with global variable errno
/// to produce informative error messages (using error(), for instance).
///
/// After a successful operation, the result is not garanteed (it might be
/// the previous error cause).  It is not meant to be used in that situation!
char* ImageErrMsg() { ///
  return errCause;
}


// Defensive programming aids
//
// Proper defensive programming in C, which lacks an exception mechanism,
// generally leads to possibly long chains of function calls, error checking,
// cleanup code, and return statements:
//   if ( funA(x) == errorA ) { return errorX; }
//   if ( funB(x) == errorB ) { cleanupForA(); return errorY; }
//   if ( funC(x) == errorC ) { cleanupForB(); cleanupForA(); return errorZ; }
//
// Understanding such chains is difficult, and writing them is boring, messy
// and error-prone.  Programmers tend to overlook the intricate details,
// and end up producing unsafe and sometimes incorrect programs.
//
// In this module, we try to deal with these chains using a somewhat
// unorthodox technique.  It resorts to a very simple internal function
// (check) that is used to wrap the function calls and error tests, and chain
// them into a long Boolean expression that reflects the success of the entire
// operation:
//   success = 
//   check( funA(x) != error , "MsgFailA" ) &&
//   check( funB(x) != error , "MsgFailB" ) &&
//   check( funC(x) != error , "MsgFailC" ) ;
//   if (!success) {
//     conditionalCleanupCode();
//   }
//   return success;
// 
// When a function fails, the chain is interrupted, thanks to the
// short-circuit && operator, and execution jumps to the cleanup code.
// Meanwhile, check() set errCause to an appropriate message.
// 
// This technique has some legibility issues and is not always applicable,
// but it is quite concise, and concentrates cleanup code in a single place.
// 
// See example utilization in ImageLoad and ImageSave.
//
// (You are not required to use this in your code!)


// Check a condition and set errCause to failmsg in case of failure.
// This may be used to chain a sequence of operations and verify its success.
// Propagates the condition.
// Preserves global errno!
static int check(int condition, const char* failmsg) {
  errCause = (char*)(condition ? "" : failmsg);
  return condition;
}


/// Init Image library.  (Call once!)
/// Currently, simply calibrate instrumentation and set names of counters.
void ImageInit(void) { ///
  InstrCalibrate();
  InstrName[0] = "pixmem";       // InstrCount[0] will count pixel array acesses
  InstrName[1] = "Comp";         // InstrCount[1] will count number of comparation
  InstrName[2] = "Sum";          // InstrCount[2] will count number of sums
  InstrName[3] = "Med";          // InstrCount[3] will count number of medias
  InstrName[4] = "SubSum";       // InstrCount[4] will count number of sub/add
  InstrName[5] = "Mul";          // InstrCount[5] will count number of multiplication
  // Name other counters here...
  
}

// Macros to simplify accessing instrumentation counters:
#define PIXMEM InstrCount[0]
#define COMP InstrCount[1]
#define SUM InstrCount[2]
#define MED InstrCount[3]
#define SUBSUM InstrCount[4]
#define MUL InstrCount[5]
// Add more macros here...

// TIP: Search for PIXMEM or InstrCount to see where it is incremented!


/// Image management functions

/// Create a new black image.
///   width, height : the dimensions of the new image.
///   maxval: the maximum gray level (corresponding to white).
/// Requires: width and height must be non-negative, maxval > 0.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageCreate(int width, int height, uint8 maxval) { ///
  assert (width >= 0);
  assert (height >= 0);
  assert (0 < maxval && maxval <= PixMax);

  
  Image image = (Image)malloc(sizeof(struct image));                //arrange space for the image struct
  if(image == NULL){
    errCause = "Falha na alocação de memória (image struct)";
    error(2, errno,  "Creating %s", ImageErrMsg());
    return NULL;
  }

  image->width = width;
  image->height = height;
  image->maxval = maxval;

  int size = width*height;

  image->pixel = (uint8*)malloc(sizeof(uint8) * size);
  if(image->pixel == NULL){
    free(image);
    errCause = "Falha na alocação de memória (pixel array)";
    error(2, errno,  "Creating %s", ImageErrMsg());
    return  NULL;
  }

  for(int i = 0; i < size; i++){
    image->pixel[i] = 0; 
  }

  return image;

  //Done ig
}

/// Destroy the image pointed to by (*imgp).
///   imgp : address of an Image variable.
/// If (*imgp)==NULL, no operation is performed.
/// Ensures: (*imgp)==NULL.
/// Should never fail, and should preserve global errno/errCause.
void ImageDestroy(Image* imgp) { ///
  assert (imgp != NULL);

  free((*imgp)->pixel);
  free(*imgp);
  *imgp = NULL;

  //Done ig
}


/// PGM file operations

// See also:
// PGM format specification: http://netpbm.sourceforge.net/doc/pgm.html

// Match and skip 0 or more comment lines in file f.
// Comments start with a # and continue until the end-of-line, inclusive.
// Returns the number of comments skipped.
static int skipComments(FILE* f) {
  char c;
  int i = 0;
  while (fscanf(f, "#%*[^\n]%c", &c) == 1 && c == '\n') {
    i++;
  }
  return i;
}

/// Load a raw PGM file.
/// Only 8 bit PGM files are accepted.
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageLoad(const char* filename) { ///
  int w, h;
  int maxval;
  char c;
  FILE* f = NULL;
  Image img = NULL;

  int success = 
  check( (f = fopen(filename, "rb")) != NULL, "Open failed" ) &&
  // Parse PGM header
  check( fscanf(f, "P%c ", &c) == 1 && c == '5' , "Invalid file format" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d ", &w) == 1 && w >= 0 , "Invalid width" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d ", &h) == 1 && h >= 0 , "Invalid height" ) &&
  skipComments(f) >= 0 &&
  check( fscanf(f, "%d", &maxval) == 1 && 0 < maxval && maxval <= (int)PixMax , "Invalid maxval" ) &&
  check( fscanf(f, "%c", &c) == 1 && isspace(c) , "Whitespace expected" ) &&
  // Allocate image
  (img = ImageCreate(w, h, (uint8)maxval)) != NULL &&
  // Read pixels
  check( fread(img->pixel, sizeof(uint8), w*h, f) == w*h , "Reading pixels" );
  PIXMEM += (unsigned long)(w*h);  // count pixel memory accesses

  // Cleanup
  if (!success) {
    errsave = errno;
    ImageDestroy(&img);
    errno = errsave;
  }
  if (f != NULL) fclose(f);
  return img;
}

/// Save image to PGM file.
/// On success, returns nonzero.
/// On failure, returns 0, errno/errCause are set appropriately, and
/// a partial and invalid file may be left in the system.
int ImageSave(Image img, const char* filename) { ///
  assert (img != NULL);
  int w = img->width;
  int h = img->height;
  uint8 maxval = img->maxval;
  FILE* f = NULL;

  int success =
  check( (f = fopen(filename, "wb")) != NULL, "Open failed" ) &&
  check( fprintf(f, "P5\n%d %d\n%u\n", w, h, maxval) > 0, "Writing header failed" ) &&
  check( fwrite(img->pixel, sizeof(uint8), w*h, f) == w*h, "Writing pixels failed" ); 
  PIXMEM += (unsigned long)(w*h);  // count pixel memory accesses

  // Cleanup
  if (f != NULL) fclose(f);
  return success;
}


/// Information queries

/// These functions do not modify the image and never fail.

/// Get image width
int ImageWidth(Image img) { ///
  assert (img != NULL);
  return img->width;
}

/// Get image height
int ImageHeight(Image img) { ///
  assert (img != NULL);
  return img->height;
}

/// Get image maximum gray level
int ImageMaxval(Image img) { ///
  assert (img != NULL);
  return img->maxval;
}

/// Pixel stats
/// Find the minimum and maximum gray levels in image.
/// On return,
/// *min is set to the minimum gray level in the image,
/// *max is set to the maximum.
void ImageStats(Image img, uint8* min, uint8* max) { ///
  assert (img != NULL);
  int size = img->width * img->height;
  
  *max = img->pixel[0];
  *min = img->pixel[0];

  //easy way to get max and min values
  for(int i = 0; i < size; i++){
    if(img->pixel[i] < *min){
      *min = img->pixel[i];
    }
    if(img->pixel[i] > *max){
      *max = img->pixel[i];
    }
  }
  
  //Done ig
}

/// Check if pixel position (x,y) is inside img.
int ImageValidPos(Image img, int x, int y) { ///
  assert (img != NULL);
  return (0 <= x && x < img->width) && (0 <= y && y < img->height);
}

/// Check if rectangular area (x,y,w,h) is completely inside img.
int ImageValidRect(Image img, int x, int y, int w, int h) { ///
  assert (img != NULL);
  return (0 <= x && x < img->width) && (0 <= y && y < img->height) 
  && (w >= 0 && w <= img->width) && (h >= 0 && h <= img->height);

  //Done ig
}

/// Pixel get & set operations

/// These are the primitive operations to access and modify a single pixel
/// in the image.
/// These are very simple, but fundamental operations, which may be used to 
/// implement more complex operations.

// Transform (x, y) coords into linear pixel index.
// This internal function is used in ImageGetPixel / ImageSetPixel. 
// The returned index must satisfy (0 <= index < img->width*img->height)
static inline int G(Image img, int x, int y) {
  int index;

  index = (img->width) * y + x;

  assert (0 <= index && index < img->width*img->height);
  return index;

  //done ig
}

/// Get the pixel (level) at position (x,y).
uint8 ImageGetPixel(Image img, int x, int y) { ///
  assert (img != NULL);
  assert (ImageValidPos(img, x, y));
  PIXMEM += 1;  // count one pixel access (read)
  return img->pixel[G(img, x, y)];
} 

/// Set the pixel at position (x,y) to new level.
void ImageSetPixel(Image img, int x, int y, uint8 level) { ///
  assert (img != NULL);
  assert (ImageValidPos(img, x, y));
  PIXMEM += 1;  // count one pixel access (store)
  img->pixel[G(img, x, y)] = level;
} 


/// Pixel transformations

/// These functions modify the pixel levels in an image, but do not change
/// pixel positions or image geometry in any way.
/// All of these functions modify the image in-place: no allocation involved.
/// They never fail.


/// Transform image to negative image.
/// This transforms dark pixels to light pixels and vice-versa,
/// resulting in a "photographic negative" effect.
void ImageNegative(Image img) { ///
  assert (img != NULL);
  
  for (int i = 0; i < img->height; i++){                     // i : eixo dos y
    for(int j = 0; j < img->width; j++){                     // j : eixo dos x
                
      uint8 pixel = ImageGetPixel(img, j, i);                
      uint8 newPixel = img->maxval - pixel;                  // invert the value of the pixel
      ImageSetPixel(img, j, i, newPixel);
    }
  }

  //Done ig
}

/// Apply threshold to image.
/// Transform all pixels with level<thr to black (0) and
/// all pixels with level>=thr to white (maxval).
void ImageThreshold(Image img, uint8 thr) { ///
  assert (img != NULL);

  for(int i = 0; i < img->height; i++){                       
    for(int j = 0; j < img->width; j++){                      
      
      uint8 pixel = ImageGetPixel(img, j, i);
      if(pixel < thr) ImageSetPixel(img, j, i, 0);
      else ImageSetPixel(img, j, i, img->maxval);
    }
  }

  //Done ig
}

/// Brighten image by a factor.
/// Multiply each pixel level by a factor, but saturate at maxval.
/// This will brighten the image if factor>1.0 and
/// darken the image if factor<1.0.
void ImageBrighten(Image img, double factor) { ///
  assert (img != NULL);
  assert (factor >= 0.0);

  for(int i = 0; i < img->height; i++){                       
    for(int j = 0; j < img->width; j++){                        
      
      uint8 pixel = ImageGetPixel(img, j, i);
      double newPixel = pixel * factor + 0.5;                   // +0.5 to help rounding and help convert into uint8

      if(newPixel > img->maxval) newPixel = img->maxval;        // verifica se há overflow
      
      ImageSetPixel(img, j, i, (uint8)newPixel);
    }
  }

  //Done ig
}


/// Geometric transformations

/// These functions apply geometric transformations to an image,
/// returning a new image as a result.
/// 
/// Success and failure are treated as in ImageCreate:
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.

// Implementation hint: 
// Call ImageCreate whenever you need a new image!

/// Rotate an image.
/// Returns a rotated version of the image.
/// The rotation is 90 degrees clockwise.
/// Ensures: The original img is not modified.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageRotate(Image img) { ///
  assert (img != NULL);

  // imagem nova, com altura e largura trocados em relação à img
  Image newImage = ImageCreate(img->height, img->width, img->maxval);  

  for(int i = 0; i < img->height; i++){                                      
    for(int j = 0; j < img->width; j++){                                        
      uint8 pixel = ImageGetPixel(img, j, i);
      ImageSetPixel(newImage, i, img->width - j - 1, pixel);            // img (j, i) -> newImage (i, img->width - j - 1)
    }
  }
  return newImage;

  //Done ig
}

/// Mirror an image = flip left-right.
/// Returns a mirrored version of the image.
/// Ensures: The original img is not modified.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageMirror(Image img) { ///
  assert (img != NULL);

  Image newImage = ImageCreate(img->width, img->height, img->maxval);

  for(int i = 0; i < img->height; i++){                                           
    for(int j = 0; j < img->width/2; j++){                                        
      
      //pixel espelhado obtido a partir do complemento img->width - j - 1
      uint8 temp = ImageGetPixel(img, img->width - j - 1, i);
      ImageSetPixel(newImage, img->width - j - 1, i, ImageGetPixel(img, j, i));
      ImageSetPixel(newImage, j, i, temp);
    }
  }
  return newImage;

  //Done ig
}

/// Crop a rectangular subimage from img.
/// The rectangle is specified by the top left corner coords (x, y) and
/// width w and height h.
/// Requires:
///   The rectangle must be inside the original image.
/// Ensures:
///   The original img is not modified.
///   The returned image has width w and height h.
/// 
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
/// On failure, returns NULL and errno/errCause are set accordingly.
Image ImageCrop(Image img, int x, int y, int w, int h) { ///
  assert (img != NULL);
  assert (ImageValidRect(img, x, y, w, h));

  Image newImage = ImageCreate(w, h, img->maxval);

  //iterar a imagem vazia criada, e em cada iteração colocar o pixel correspondente da img

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      uint8 pixel = ImageGetPixel(img, x + j, y + i);
      ImageSetPixel(newImage, j, i, pixel);
    }
  }

  return newImage;
  
  //Done ig
}


/// Operations on two images

/// Paste an image into a larger image.
/// Paste img2 into position (x, y) of img1.
/// This modifies img1 in-place: no allocation involved.
/// Requires: img2 must fit inside img1 at position (x, y).
void ImagePaste(Image img1, int x, int y, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidRect(img1, x, y, img2->width, img2->height));

  //iterar a img2, obter o pixel em cada iteração e trocá-la na img1

  for(int i = 0; i < img2->height; i++){
    for(int j = 0; j < img2->width; j++){
      uint8 pixel = ImageGetPixel(img2, j, i);
      ImageSetPixel(img1, j+x, i+y, pixel);
    }
  }

  //Done ig
}

/// Blend an image into a larger image.
/// Blend img2 into position (x, y) of img1.
/// This modifies img1 in-place: no allocation involved.
/// Requires: img2 must fit inside img1 at position (x, y).
/// alpha usually is in [0.0, 1.0], but values outside that interval
/// may provide interesting effects.  Over/underflows should saturate.
void ImageBlend(Image img1, int x, int y, Image img2, double alpha) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidRect(img1, x, y, img2->width, img2->height));

  for(int i = 0; i < img2->height; i++){
    for(int j = 0; j < img2->width; j++){

      uint8 pixel1 = ImageGetPixel(img1, j+x, i+y);
      uint8 pixel2 = ImageGetPixel(img2, j, i);

      double newPixel = ((alpha*pixel2) + ((1-alpha)*pixel1))+0.5;              // +0.5 devido ao arredondametno                

      if(newPixel > img1->maxval) newPixel = img1->maxval;                      // overflow               
      else if(newPixel < 0) newPixel = 0;                                       // underflow  

      ImageSetPixel(img1, j+x, i+y, (uint8)newPixel);
    }
  }

  //Done ig
}

/// Compare an image to a subimage of a larger image.
/// Returns 1 (true) if img2 matches subimage of img1 at pos (x, y).
/// Returns 0, otherwise.
int ImageMatchSubImage(Image img1, int x, int y, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);
  assert (ImageValidPos(img1, x, y));
  assert (ImageValidRect(img1, x, y, img2->width, img2->height));

  // iterates through the subimage and compares the pixels

  for(int i = 0; i < img2->height; i++){
    for(int j = 0; j < img2->width; j++){

      uint8 pixel1 = ImageGetPixel(img1, j+x, i+y);
      uint8 pixel2 = ImageGetPixel(img2, j, i);
      
      COMP += 1;
      if(pixel1 != pixel2) return 0;

    }
  }
  return 1;

  //Done ig
}

/// Locate a subimage inside another image.
/// Searches for img2 inside img1.
/// If a match is found, returns 1 and matching position is set in vars (*px, *py).
/// If no match is found, returns 0 and (*px, *py) are left untouched.
int ImageLocateSubImage(Image img1, int* px, int* py, Image img2) { ///
  assert (img1 != NULL);
  assert (img2 != NULL);

  for(int i = 0; i <= img1->height - img2->height; i++){
    for(int j = 0; j <= img1->width - img2->width; j++){
      COMP += 1;
      if(ImageMatchSubImage(img1, j, i, img2)){

        *px = j;
        *py = i;
        
        return 1;
      }
    }
  }
  return 0;

  //Done ig
}


/// Filtering

/// Blur an image by a applying a (2dx+1)x(2dy+1) mean filter.
/// Each pixel is substituted by the mean of the pixels in the rectangle
/// [x-dx, x+dx]x[y-dy, y+dy].
/// The image is changed in-place.
void ImageBlur(Image img, int dx, int dy) { ///
  assert (img != NULL);

  //cria uma copia da imagem temporária de forma a não alterar
  //os valores da imagem final com o blur da imagem inteira
  Image copy = ImageCreate(img->width, img->height, img->maxval);         
  for(int i = 0; i < img->height; i++){                                  
    for(int j = 0; j < img->width; j++){
      uint8 pixel = ImageGetPixel(img, j, i);
      ImageSetPixel(copy, j, i, pixel);
    }
  }

  for(int i = 0; i < img->height ; i++){
    for(int j = 0; j < img->width ; j++){
      int pixel = 0;
      int count = 0;                                                      // contar número de pixeis do blur
      // iterar o quadrilátero do blur
      for(int k = i-dy; k <= i+dy; k++){  
        for(int l = j-dx; l <= j+dx; l++){  
          COMP++;
          if (ImageValidPos(copy, l, k)) {                                // verifica se dx e dy são válidos
            pixel += ImageGetPixel(copy, l, k);                           // necessário devido aos edge cases
            count++;
            SUM += 2;                                                    
          }
        }
      }

      int media = (pixel + count/2)/count;                                // media = (soma pixeis + npixeis/2) /n pixeis
      MED++;                                                              // número de medias
      ImageSetPixel(img, j, i, (uint8)media);                             // npixeis/2 devido ao arredondamento
    }
  }
  ImageDestroy(&copy);

  //Done ig
}


void ImageBlur2(Image img, int dx, int dy){
  assert (img != NULL);
  
  int arraySum[img->height][img->width];
  arraySum[0][0] = ImageGetPixel(img, 0,0);

  //inicializar o array com as áreas dos pixeis, com coordenadas x = 0 ou y = 0
  for(int i = 1; i < img->width; i++){
    arraySum[0][i] = arraySum[0][i-1] + ImageGetPixel(img, i, 0);
    SUBSUM++;
  }

  for(int i = 1; i < img->height; i++){
    arraySum[i][0] = arraySum[i-1][0] + ImageGetPixel(img, 0, i);
    SUBSUM++;
  }

  //completar o array com as áreas, com os restantes valores
  for(int i = 1; i < img->height; i++){
    for(int j = 1; j < img->width; j++){
      arraySum[i][j] =  ImageGetPixel(img, j, i) + arraySum[i-1][j] + arraySum[i][j-1] - arraySum[i-1][j-1];
      SUBSUM += 3;
    }
  }

  //Aplicar o blur
  for(int i = 0; i < img-> height; i++){
    for(int j = 0; j < img-> width; j++){

      // Coordenadas dos cantos do quadrilátero
      int x1 = j - dx < 0 ? 0 : j - dx;
      int y1 = i - dy < 0 ? 0 : i - dy;
      int x2 = j + dx >= img->width ? img->width - 1 : j + dx;
      int y2 = i + dy >= img->height ? img->height - 1 : i + dy;
      SUBSUM += 4;
      // Coordenadas obtidas:
      // top-left     -> (x1, y1)
      // top-right    -> (x2, y1)
      // bottom-left  -> (x1, y2)
      // bottom-right -> (x2, y2)

      int nPixels = (x2 - x1 + 1) * (y2 - y1 + 1);
      MUL++;

      int sum = arraySum[y2][x2];

      //Condições necessárias, por causa dos Edge Cases
      if(y1 > 0){
        sum -= arraySum[y1-1][x2];
        SUBSUM++;
      }
      if(x1 > 0){
        sum -= arraySum[y2][x1-1];
        SUBSUM++;
      }
      if(x1 > 0 && y1 > 0){
        sum +=arraySum[y1-1][x1-1];
        SUBSUM++;
      }
      COMP += 3;

      // media = (soma pixeis + npixeis/2) /n pixeis
      int media = (sum + nPixels/2)/nPixels;
      MED++;

      ImageSetPixel(img, j, i, (uint8)media);
    }
  }

  //Done ig
}


/*

Foi adicionado tanto no header como no ImageTool.c, o funcionamento das duas variações da função Blur 
(ImageBlur e ImageBlur2)

Podem ser testados por exemplo:

Valores menores:
$ make
$ ./imageTool pgm/large/ireland_03_1600x1200.pgm blur 5,5 save test.pgm
$ ./imageTool pgm/large/ireland_03_1600x1200.pgm blur2 5,5 save test.pgm

Valores maiores:
$ make
$ ./imageTool pgm/large/ireland_03_1600x1200.pgm blur 10000,10000 save test.pgm
$ ./imageTool pgm/large/ireland_03_1600x1200.pgm blur2 10000,10000 save test.pgm

obs: com a versão "bruteforce" (ImageBlur), blur 50,50, demora muito tempo para processar,
por outro lado a versão otimizada (ImageBlur2), blur2 10000,10000, é praticamente instantâneo

*/
