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
  Path *next;
  GfxRGB color;
  double opacity;
  double line_width;
  int line_cap;
  int line_join;
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
  virtual GBool upsideDown( ) { return gFalse; }

  // Does this device use drawChar() or drawString()?
  virtual GBool useDrawChar( ) { return gFalse; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  virtual GBool interpretType3Chars( ) { return gFalse; }

  //----- initialization and control

  // Start a page.
  virtual void startPage( int pageNum, GfxState *state );

  // End a page.
  virtual void endPage( );

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
};

#endif
