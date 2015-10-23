//========================================================================
//
// ForOutputDev.h
//
//========================================================================

#ifndef FOROUTPUTDEV_H
#define FOROUTPUTDEV_H

#include <OutputDev.h>

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
  bool fill;
  GfxRGB color;
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

  //----- path painting
  virtual void stroke( GfxState *state );
  virtual void fill( GfxState *state );

  virtual void doPath( GfxState *state, bool fill );

  // the current document
  PDFDoc *doc;

  // the current color
  GfxRGB current_color;
  // the current opacity
  double current_opacity;


  // the printed path
  Path *path_list;
  // the number of paths
  int path_number;

  int char_pos;
  int object_pos;
};

#endif
