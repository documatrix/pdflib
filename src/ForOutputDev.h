//========================================================================
//
// ForOutputDev.h
//
//========================================================================

#ifndef FOROUTPUTDEV_H
#define FOROUTPUTDEV_H

#include <OutputDev.h>
#include <poppler-features.h>
#include <stdint.h>

#ifndef gTrue
#define gTrue true
#define gFalse false
typedef bool GBool;
#endif

class PDFDoc;
class GfxState;
class GfxPath;
class Gfx8BitFont;
struct GfxRGB;

//------------------------------------------------------------------------
// ForOutputDev
//------------------------------------------------------------------------

/* The different color spaces */
enum ForOutputDevColorSpace
{
  deviceRGB,
  deviceCMYK
};

/* The definition for struct Color */
struct Color
{
  uint8_t r;
  uint8_t b;
  uint8_t g;

  uint8_t c;
  uint8_t m;
  uint8_t y;
  uint8_t k;

  double opacity;

  ForOutputDevColorSpace color_space;
};

/* The definition for struct Line */
struct Path
{
  unsigned int count;
  double *x, *y;
  int *command;
  int path_painting_operator;
  Color color;
  double line_width;
  int line_cap;
  int line_join;
  double miter_limit;
  int dash_length;
  double *dash_pattern;
  double dash_start;

  double ctm[ 6 ];

  int char_pos;
  int object_pos;
  Path *next;
};

/* The definition for struct Image */
struct Image
{
  unsigned int id;
  int64_t file_pos;
  Color color;
  int width;
  int height;
  double x;
  double y;

  int char_pos;
  int object_pos;
  double rotation;
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
  GBool upsideDown( ) override { return gTrue; }

  // Does this device use drawChar() or drawString()?
  GBool useDrawChar( ) override { return gTrue; }

  // Does this device use beginType3Char/endType3Char?  Otherwise,
  // text in Type 3 fonts will be drawn with drawChar/drawString.
  GBool interpretType3Chars( ) override { return gFalse; }

  bool needCharCount() override { return true; }

 // Does this device use tilingPatternFill()?  If this returns false,
  // tiling pattern fills will be reduced to a series of other drawing
  // operations.
  GBool useTilingPatternFill() override { return gFalse; }

  // Does this device need to clip pages to the crop box even when the
  // box is the crop box?
  GBool needClipToCropBox() override { return gTrue; }

  //----- initialization and control

  // Start a page.
#if POPPLER_CHECK_VERSION(0, 23, 0)
  void startPage(int pageNum, GfxState *state, XRef *xref) override;
#else
  void startPage( int pageNum, GfxState *state ) override;
#endif

  // End a page.
  void endPage( ) override;

  //----- save/restore graphics state
  void restoreState(GfxState *state)  override;

  //----- update text state
  void updateFont(GfxState *state) override;

  //----- text drawing
#if POPPLER_CHECK_VERSION(0, 64, 0)
  void beginString(GfxState *state, const GooString *s) override;
#else
  void beginString(GfxState *state, GooString *s) override;
#endif

  void endString(GfxState *state) override;

#if POPPLER_CHECK_VERSION(0, 82, 0)
  void drawChar(GfxState *state, double x, double y,
                        double dx, double dy,
                        double originX, double originY,
                        CharCode c, int nBytes, const Unicode *u, int uLen) override;
#else
  void drawChar(GfxState *state, double x, double y,
                        double dx, double dy,
                        double originX, double originY,
                        CharCode c, int nBytes, Unicode *u, int uLen) override;
#endif

  void incCharCount(int nChars) override;

  void beginActualText(GfxState *state, const GooString *text) override;
  void endActualText(GfxState *state) override;

  //----- Image painting
  void drawImageMask(GfxState *state, Object *ref, Stream *str,
                             int width, int height, GBool invert, GBool interpolate,
                             GBool inlineImg) override;

  void setSoftMaskFromImageMask(GfxState *state,
                                        Object *ref, Stream *str,
                                        int width, int height, GBool invert,
                                        GBool inlineImg, double *baseMatrix) override;
  void unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix) override;

  void drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
                              int width, int height, GBool invert, GBool interpolate,
                              GBool inlineImg);

  void drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
                            int width, int height, GBool invert, GBool interpolate,
                            GBool inlineImg);

#if POPPLER_CHECK_VERSION(0, 82, 0)
  void drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, bool interpolate, const int *maskColors, bool inlineImg) override;
#else
  void drawImage(GfxState *state, Object *ref, Stream *str, int width, int height, GfxImageColorMap *colorMap, GBool interpolate, int *maskColors, GBool inlineImg) override;
#endif

  void drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
                                   int width, int height,
                                   GfxImageColorMap *colorMap,
                                   GBool interpolate,
                                   Stream *maskStr,
                                   int maskWidth, int maskHeight,
                                   GfxImageColorMap *maskColorMap,
                                   GBool maskInterpolate) override;

  void drawMaskedImage(GfxState *state, Object *ref, Stream *str,
                               int width, int height,
                               GfxImageColorMap *colorMap,
                               GBool interpolate,
                               Stream *maskStr,
                               int maskWidth, int maskHeight,
                               GBool maskInvert, GBool maskInterpolate) override;

  // GBool tilingPatternFill(GfxState *state, Gfx *gfx, Catalog *cat, Object *str,
  //                                 double *pmat, int paintType, int tilingType, Dict *resDict,
  //                                 double *mat, double *bbox,
  //                                 int x0, int y0, int x1, int y1,
  //                                 double xStep, double yStep) override;

  GBool patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading) override;

  GBool gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading) override;

  //----- path painting
  void stroke( GfxState *state ) override;
  void fill( GfxState *state ) override;
  void eoFill( GfxState *state ) override;

  //----- path clipping
  void clip( GfxState *state ) override;
  void eoClip(GfxState *state) override;

  //----- save path
  void doPath( GfxState *state, int fill );

  //----- save image
  void doImage( GfxState *state, Stream *str, int width, int height );

  //----- get colors
  void getStrokeColor( GfxState *state );
  void getFillColor( GfxState *state );

  Path* getPathList( );
  int getPathCount( );

  Image* getImageList( );
  unsigned int getImageCount( );

  // the current document
  PDFDoc *doc;

  // the current color
  GfxRGB current_color_rgb;
  GfxCMYK current_color_cmyk;
  GfxColorSpaceMode current_color_space;
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
