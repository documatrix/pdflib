//========================================================================
//
// ForOutputDev.h
//
//========================================================================

#ifndef FOROUTPUTDEV_H
#define FOROUTPUTDEV_H

#include <OutputDev.h>
#include <stdint.h>

class PDFDoc;
class GfxState;
class GfxPath;
class Gfx8BitFont;
struct GfxRGB;

//------------------------------------------------------------------------
// ForOutputDev
//------------------------------------------------------------------------

/* The definition for struct Line */
struct Path
{
  unsigned int count;
  double *x, *y;
  int *command;
  int path_painting_operator;
  uint8_t color_red;
  uint8_t color_blue;
  uint8_t color_green;
  double opacity;
  double line_width;
  int line_cap;
  int line_join;
  double miter_limit;
  int dash_length;
  double *dash_pattern;
  double dash_start;

  int char_pos;
  int object_pos;
  Path *next;
};

/* The definition for struct Image */
struct Image
{
  unsigned int id;
  int64_t file_pos;
  uint8_t color_red;
  uint8_t color_blue;
  uint8_t color_green;
  double opacity;
  int width;
  int height;

  int char_pos;
  int object_pos;
  Image *next;
};

class ForOutputDev: public OutputDev
{
public:

  // Constructor.
  ForOutputDev( );

  // Destructor.
  virtual ~ForOutputDev( );

  // Does this device use upside-down coordinates?
  // (Upside-down means (0,0) is the top left corner of the page.)
  virtual GBool upsideDown( ) { return gTrue; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar( ) { return gTrue; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars( ) { return gFalse; }

  virtual GBool needCharCount() { return gTrue; }

 // Does this device use tilingPatternFill()?  If this returns false,
  // tiling pattern fills will be reduced to a series of other drawing
  // operations.
  virtual GBool useTilingPatternFill() { return gFalse; }

  // Does this device need to clip pages to the crop box even when the
  // box is the crop box?
  virtual GBool needClipToCropBox() { return gTrue; }

  //----- initialization and control

  // Start a page.
  virtual void startPage( int pageNum, GfxState *state );

  // End a page.
  virtual void endPage( );

  //----- save/restore graphics state
  virtual void restoreState(GfxState *state);

  //----- update text state
  virtual void updateFont(GfxState *state);

  //----- text drawing
  virtual void beginString(GfxState *state, GooString *s);
  virtual void endString(GfxState *state);
  virtual void drawChar(GfxState *state, double x, double y,
                        double dx, double dy,
                        double originX, double originY,
                        CharCode c, int nBytes, Unicode *u, int uLen);

  virtual void incCharCount(int nChars);
  virtual void beginActualText(GfxState *state, GooString *text);
  virtual void endActualText(GfxState *state);

  //----- Image painting
  virtual void drawImageMask(GfxState *state, Object *ref, Stream *str,
                             int width, int height, GBool invert, GBool interpolate,
                             GBool inlineImg);

  virtual void setSoftMaskFromImageMask(GfxState *state,
                                        Object *ref, Stream *str,
                                        int width, int height, GBool invert,
                                        GBool inlineImg, double *baseMatrix);
  virtual void unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix);

  void drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
                              int width, int height, GBool invert, GBool interpolate,
                              GBool inlineImg);

  void drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
                            int width, int height, GBool invert, GBool interpolate,
                            GBool inlineImg);

  virtual void drawImage(GfxState *state, Object *ref, Stream *str,
                         int width, int height, GfxImageColorMap *colorMap,
                         GBool interpolate, int *maskColors, GBool inlineImg);

  virtual void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
                                   int width, int height,
                                   GfxImageColorMap *colorMap,
                                   GBool interpolate,
                                   Stream *maskStr,
                                   int maskWidth, int maskHeight,
                                   GfxImageColorMap *maskColorMap,
                                   GBool maskInterpolate);

  virtual void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
                               int width, int height,
                               GfxImageColorMap *colorMap,
                               GBool interpolate,
                               Stream *maskStr,
                               int maskWidth, int maskHeight,
                               GBool maskInvert, GBool maskInterpolate);

  virtual GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str,
                                  double *pmat, int paintType, int tilingType, Dict *resDict,
                                  double *mat, double *bbox,
                                  int x0, int y0, int x1, int y1,
                                  double xStep, double yStep);

  virtual GBool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading);

  virtual GBool gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading);

  //----- path painting
  virtual void stroke( GfxState *state );
  virtual void fill( GfxState *state );
  virtual void eoFill( GfxState *state );

  //----- path clipping
  virtual void clip( GfxState *state );
  virtual void eoClip(GfxState *state);

  //----- save path
  virtual void doPath( GfxState *state, int fill );

  //----- save image
  virtual void doImage( GfxState *state, Stream *str, int width, int height );

  // the current document
  PDFDoc *doc;

  // the current color
  GfxRGB current_color;
  // the current opacity
  double current_opacity;


  // the printed paths
  Path *path_list;
  Path *current_path;
  // the number of paths
  int path_number;

  // the printed images
  Image *image_list;
  Image *current_image;
  unsigned int image_nr;

  int char_pos;
  int object_pos;
  double clip_x1;
  double clip_y1;
  double clip_x2;
  double clip_y2;
};

#endif
