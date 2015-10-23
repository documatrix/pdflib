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
  path_number = 0;
  char_pos = 0;
  object_pos = 0;
  current_color.r = current_color.g = current_color.b = 0;
  path_list = new Path;
}

ForOutputDev::~ForOutputDev()
{
}

void ForOutputDev::startPage( int pageNum, GfxState *state )
{
  printf( "startPage\n" );
  current_color.r = current_color.g = current_color.b = 0;
  char_pos = 0;
  object_pos = 0;
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
  printf( "drawImageMask\n" );
  printf( "fill color: %d %d %d\n", current_color.r, current_color.g, current_color.b );
  printf( "Height %d, Width %d Position %d\n", width, height, str->getPos( ) );
  object_pos ++;
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
  printf( "drawImage\n" );
  printf( "Height %d, Width %d Position %d\n", width, height, str->getPos( ) );
  object_pos ++;
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

void ForOutputDev::fill( GfxState *state )
{
  current_opacity = state->getFillOpacity();
  state->getFillRGB( &current_color );
  printf( "fill color: %d %d %d\n", current_color.r, current_color.g, current_color.b );
  doPath( state, true );
}

void ForOutputDev::stroke( GfxState *state )
{
  current_opacity = state->getStrokeOpacity();
  state->getStrokeRGB( &current_color );
  printf( "stroke color: %d %d %d\n", current_color.r, current_color.g, current_color.b );
  doPath( state, false );
}

void ForOutputDev::doPath( GfxState *state, bool fill )
{
  printf( "Do PATH\n" );
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
  current_path->fill = fill;

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
  current_path->color.r = current_color.r;
  current_path->color.g = current_color.g;
  current_path->color.b = current_color.b;
  current_path->opacity = current_opacity;

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

      /* Set Points*/
      current_path->x[ point_nr ] = subpath->getX( sub_nr );
      current_path->y[ point_nr ] = subpath->getY( sub_nr );
    }
  }
  current_path->object_pos = object_pos;
  current_path->char_pos = char_pos;
  path_number ++;
  object_pos ++;
  current_path->next = new Path;
  current_path = current_path->next;
}

