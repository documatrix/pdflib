//========================================================================
//
// ForOutputDev.cc
//
//========================================================================

#include <GfxState.h>
#include <GfxFont.h>
#include <Page.h>
#include <ForOutputDev.h>
#include <dm_poppler.h>
#include <glib.h>

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// ForOutputDev
//------------------------------------------------------------------------

ForOutputDev::ForOutputDev()
{
  doc = NULL;
  char_pos = 0;
  object_pos = 0;

  current_color.r = current_color.g = current_color.b = 0;
  clip_x1 = clip_x2 = clip_y1 = clip_y2 = 0;

  path_number = 0;
  path_list = new Path;

  image_nr = 0;
  image_list = new Image;
  current_image = image_list;
}

ForOutputDev::~ForOutputDev()
{
}

void ForOutputDev::startPage( int pageNum, GfxState *state )
{
  printf( "startPage\n" );
  char_pos = 0;
  object_pos = 0;

  current_color.r = current_color.g = current_color.b = 0;
  clip_x1 = clip_x2 = clip_y1 = clip_y2 = 0;
}

void ForOutputDev::endPage( )
{
}

void ForOutputDev::restoreState(GfxState *state)
{
}

void ForOutputDev::updateFont(GfxState *state)
{
}

void ForOutputDev::beginString(GfxState *state, GooString *s)
{
}

void ForOutputDev::endString(GfxState *state)
{
}

void ForOutputDev::beginActualText(GfxState *state, GooString *text)
{
}

void ForOutputDev::endActualText(GfxState *state)
{
}


void ForOutputDev::drawChar( GfxState *state, double x, double y,
                             double dx, double dy,
                             double originX, double originY,
                             CharCode c, int nBytes, Unicode *u, int uLen )
{
  //printf( "Char\n");
  char_pos += nBytes;
  object_pos = 0;
}

void ForOutputDev::incCharCount( int nChars )
{
  printf( "Achtung incCharCount wurde aufgerufen wurde nicht getestet!\n" );
  char_pos += nChars;
  object_pos = 0;
}

void ForOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
                                 int width, int height, GBool invert,
                                 GBool interpolate, GBool inlineImg)
{
  state->getFillRGB( &current_color );
  doImage( state, str, width, height );
}

void ForOutputDev::setSoftMaskFromImageMask(GfxState *state, Object *ref, Stream *str,
                                            int width, int height, GBool invert,
                                            GBool inlineImg, double *baseMatrix)
{
  printf( "setSoftMaskFromImageMask\n" );
}

void ForOutputDev::unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix)
{
  printf( "unsetSoftMaskFromImageMask\n" );
}

void ForOutputDev::drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
                                        int width, int height, GBool invert,
                                        GBool interpolate, GBool inlineImg)
{
  printf( "drawImageMaskRegular\n" );
}


void ForOutputDev::drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
                                          int width, int height, GBool invert,
                                          GBool interpolate, GBool inlineImg)
{
  printf( "drawImageMaskPrescaled\n" );
}

void ForOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
                            int width, int height, GfxImageColorMap *colorMap,
                            GBool interpolate, int *maskColors, GBool inlineImg)
{
  doImage( state, str, width, height );
}

void ForOutputDev::drawSoftMaskedImage(GfxState *state, Object *ref, Stream *str,
                                       int width, int height,
                                       GfxImageColorMap *colorMap,
                                       GBool interpolate,
                                       Stream *maskStr,
                                       int maskWidth, int maskHeight,
                                       GfxImageColorMap *maskColorMap,
                                       GBool maskInterpolate)
{
  printf( "drawSoftMaskedImage\n" );
}

void ForOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
                                   int width, int height,
                                   GfxImageColorMap *colorMap,
                                   GBool interpolate,
                                   Stream *maskStr,
                                   int maskWidth, int maskHeight,
                                   GBool maskInvert, GBool maskInterpolate)
{
  printf( "drawMaskedImage\n" );
}

GBool ForOutputDev::tilingPatternFill(GfxState *state, Gfx *gfxA, Catalog *cat, Object *str,
          double *pmat, int paintType, int /*tilingType*/, Dict *resDict,
          double *mat, double *bbox,
          int x0, int y0, int x1, int y1,
          double xStep, double yStep)
{
  printf( "tilingPatternFill\n" );
}

GBool ForOutputDev::patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading)
{
  printf( "patchMeshShadedFill\n" );
}

GBool ForOutputDev::gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading)
{
  printf( "gouraudTriangleShadedFill\n" );
}

void ForOutputDev::fill( GfxState *state )
{
  current_opacity = state->getFillOpacity();
  state->getFillRGB( &current_color );
  doPath( state, FILL );
}

void ForOutputDev::eoFill( GfxState *state )
{
  current_opacity = state->getFillOpacity();
  state->getFillRGB( &current_color );
  doPath( state, EOFILL );
}

void ForOutputDev::stroke( GfxState *state )
{
  current_opacity = state->getStrokeOpacity();
  state->getStrokeRGB( &current_color );
  doPath( state, STROKE );
}

void ForOutputDev::doPath( GfxState *state, int path_painting_operator )
{
  double xMin, yMin, xMax, yMax;
  state->getUserClipBBox(&xMin, &yMin, &xMax, &yMax);

  static Path *current_path = path_list;
  GfxPath *path = state->getPath( );
  GfxSubpath *subpath;
  double *x, *y;

  int points_count = 0;
  for ( int i = 0; i < path->getNumSubpaths( ); ++i )
  {
    subpath = path->getSubpath( i );
    points_count += subpath->getNumPoints( );
  }
  current_path->count = points_count;
  current_path->path_painting_operator = path_painting_operator;

  /* Path Settings */
  current_path->line_width  = state->getLineWidth( );
  current_path->line_cap    = state->getLineCap( );
  current_path->line_join   = state->getLineJoin( );
  current_path->miter_limit = state->getMiterLimit( );

  /* LineDash */
  double *dashPattern;
  int dashLength;
  double dashStart;
  state->getLineDash(&dashPattern, &dashLength, &dashStart);
  current_path->dash_length  = dashLength;
  current_path->dash_pattern = (double*)malloc( dashLength * sizeof( double* ) );
  for ( int i = 0; i < dashLength; ++i )
  {
    current_path->dash_pattern[ i ] = dashPattern[ i ];
  }
  current_path->dash_start   = dashStart;

  /* Color */
  current_path->color_red   = colToByte( current_color.r );
  current_path->color_green = colToByte( current_color.g );
  current_path->color_blue  = colToByte( current_color.b );
  current_path->opacity     = current_opacity;

  /* Points */
  current_path->x = (double*)malloc( points_count * sizeof( double* ) );
  current_path->y = (double*)malloc( points_count * sizeof( double* ) );
  current_path->command = (int*)malloc( points_count * sizeof( int* ) );
  int point_nr = 0;
  for ( int i = 0; i < path->getNumSubpaths( ); ++i )
  {
    subpath = path->getSubpath( i );

    for ( int sub_nr = 0; sub_nr < subpath->getNumPoints( ); ++sub_nr, ++point_nr )
    {
      /* Set Commands */
      if ( sub_nr == 0 )
      {
        current_path->command[ point_nr ] = MOVE_TO;
      }
      else
      {
        if ( subpath->getCurve( sub_nr ) )
        {
          current_path->command[ point_nr ] = CURVE_TO;
        }
        else
        {
          current_path->command[ point_nr ] = LINE_TO;
        }
      }

      /* Set Points (upsite down!)*/
      current_path->x[ point_nr ] = subpath->getX( sub_nr ) - xMin + clip_x1;
      current_path->y[ point_nr ] = state->getPageHeight( ) - ( subpath->getY( sub_nr ) - yMin + clip_y1 );
      // printf( "oldy %f newy %f pageheight %f yMin %f clip_y1 %f\n", subpath->getY( sub_nr ), current_path->y[ point_nr ], yMin, clip_y1 );
    }
  }
  current_path->object_pos = object_pos;
  current_path->char_pos = char_pos;

  /* Next */
  path_number ++;
  object_pos ++;
  current_path->next = new Path;
  current_path = current_path->next;
}

void ForOutputDev::doImage( GfxState *state, Stream *str, int width, int height )
{
  current_image->file_pos = str->getPos( );
  current_image->id = image_nr;

  current_image->height = height;
  current_image->width  = width;

  /* Color */
  current_image->color_red   = colToByte( current_color.r );
  current_image->color_green = colToByte( current_color.g );
  current_image->color_blue  = colToByte( current_color.b );
  current_image->opacity     = colToByte( current_opacity );

  current_image->object_pos = object_pos;
  current_image->char_pos = char_pos;

  /* Next */
  image_nr ++;
  object_pos ++;
  current_image->next = new Image;
  current_image = current_image->next;
}
