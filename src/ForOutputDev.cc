//========================================================================
//
// ForOutputDev.cc
//
//========================================================================

#include <config.h>
#include <GfxState.h>
#include <GfxFont.h>
#include <Page.h>
#include <ForOutputDev.h>
#include <dm_poppler.h>
#include <glib.h>
#include <math.h>

//------------------------------------------------------------------------

#ifdef LOG_DEBUG
#define LOG(x) (x)
#else
#define LOG(x)
#endif

#define PI 3.14159265

//------------------------------------------------------------------------
// ForOutputDev
//------------------------------------------------------------------------

ForOutputDev::ForOutputDev()
{
  doc = NULL;
  char_pos = 0;
  object_pos = 0;

  current_color_rgb.r = current_color_rgb.g = current_color_rgb.b = 0;
  current_color_cmyk.c = current_color_cmyk.m = current_color_cmyk.y = current_color_cmyk.k = 0;
  current_color_space = csDeviceRGB;
  clip_x1 = clip_x2 = clip_y1 = clip_y2 = 0;

  path_number = 0;
  path_list = new Path;
  current_path = path_list;

  image_nr = 0;
  image_list = new Image;
  current_image = image_list;
}

ForOutputDev::~ForOutputDev()
{
}

#if POPPLER_CHECK_VERSION(0, 23, 0)
void ForOutputDev::startPage( int pageNum, GfxState *state, XRef *xref )
#else
void ForOutputDev::startPage( int pageNum, GfxState *state )
#endif
{
  LOG( printf( "startPage\n" ) );
  char_pos = 0;
  object_pos = 0;

  current_color_rgb.r = current_color_rgb.g = current_color_rgb.b = 0;
  current_color_cmyk.c = current_color_cmyk.m = current_color_cmyk.y = current_color_cmyk.k = 0;
  current_color_space = csDeviceRGB;
  clip_x1 = clip_x2 = clip_y1 = clip_y2 = 0;
}

void ForOutputDev::endPage( )
{
}

void ForOutputDev::restoreState(GfxState *state)
{
  clip( state );
}

void ForOutputDev::updateFont(GfxState *state)
{
}

#if POPPLER_CHECK_VERSION(0, 64, 0)
void ForOutputDev::beginString(GfxState *state, const GooString *s)
#else
void ForOutputDev::beginString(GfxState *state, GooString *s)
#endif
{
}

void ForOutputDev::endString(GfxState *state)
{
}
#if POPPLER_CHECK_VERSION(0, 82, 0)
void ForOutputDev::beginActualText(GfxState *state, const GooString *text)
#else
void ForOutputDev::beginActualText(GfxState *state, GooString *text)
#endif
{
}

void ForOutputDev::endActualText(GfxState *state)
{
}

#if POPPLER_CHECK_VERSION(0, 82, 0)
void ForOutputDev::drawChar( GfxState *state, double x, double y,
                             double dx, double dy,
                             double originX, double originY,
                             CharCode c, int nBytes, const Unicode *u, int uLen )
#else
void ForOutputDev::drawChar( GfxState *state, double x, double y,
                             double dx, double dy,
                             double originX, double originY,
                             CharCode c, int nBytes, Unicode *u, int uLen )
#endif
{
  char_pos += nBytes;
  object_pos = 0;
}

void ForOutputDev::incCharCount( int nChars )
{
  LOG( printf( "incCharCount\n" ) );
  char_pos += nChars;
  object_pos = 0;
}

void ForOutputDev::drawImageMask(GfxState *state, Object *ref, Stream *str,
                                 int width, int height, GBool invert,
                                 GBool interpolate, GBool inlineImg)
{
  getFillColor( state );
  doImage( state, str, width, height );
}

void ForOutputDev::setSoftMaskFromImageMask(GfxState *state, Object *ref, Stream *str,
                                            int width, int height, GBool invert,
                                            GBool inlineImg, double *baseMatrix)
{
  LOG( printf( "setSoftMaskFromImageMask\n" ) );
}

void ForOutputDev::unsetSoftMaskFromImageMask(GfxState *state, double *baseMatrix)
{
  LOG( printf( "unsetSoftMaskFromImageMask\n" ) );
}

void ForOutputDev::drawImageMaskRegular(GfxState *state, Object *ref, Stream *str,
                                        int width, int height, GBool invert,
                                        GBool interpolate, GBool inlineImg)
{
  LOG( printf( "drawImageMaskRegular\n" ) );
}


void ForOutputDev::drawImageMaskPrescaled(GfxState *state, Object *ref, Stream *str,
                                          int width, int height, GBool invert,
                                          GBool interpolate, GBool inlineImg)
{
  LOG( printf( "drawImageMaskPrescaled\n" ) );
}

#if POPPLER_CHECK_VERSION(0, 82, 0)
void ForOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
                            int width, int height, GfxImageColorMap *colorMap,
                            GBool interpolate, const int *maskColors, GBool inlineImg)
#else
void ForOutputDev::drawImage(GfxState *state, Object *ref, Stream *str,
                            int width, int height, GfxImageColorMap *colorMap,
                            GBool interpolate, int *maskColors, GBool inlineImg)
#endif
{
  current_opacity = state->getFillOpacity( );
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
  doImage( state, str, width, height );
  LOG( printf( "drawSoftMaskedImage\n" ) );
}

void ForOutputDev::drawMaskedImage(GfxState *state, Object *ref, Stream *str,
                                   int width, int height,
                                   GfxImageColorMap *colorMap,
                                   GBool interpolate,
                                   Stream *maskStr,
                                   int maskWidth, int maskHeight,
                                   GBool maskInvert, GBool maskInterpolate)
{
  LOG( printf( "drawMaskedImage\n" ) );
}

// GBool ForOutputDev::tilingPatternFill(GfxState *state, Gfx *gfxA, Catalog *cat, Object *str,
//           double *pmat, int paintType, int /*tilingType*/, Dict *resDict,
//           double *mat, double *bbox,
//           int x0, int y0, int x1, int y1,
//           double xStep, double yStep)
// {
//   LOG( printf( "tilingPatternFill\n" ) );
// }

GBool ForOutputDev::patchMeshShadedFill(GfxState *state, GfxPatchMeshShading *shading)
{
  LOG( printf( "patchMeshShadedFill\n" ) );
  return false;
}

GBool ForOutputDev::gouraudTriangleShadedFill(GfxState *state, GfxGouraudTriangleShading *shading)
{
  LOG( printf( "gouraudTriangleShadedFill\n" ) );
  return false;
}

void ForOutputDev::clip( GfxState *state )
{
  state->getUserClipBBox( &clip_x1, &clip_y1, &clip_x2, &clip_y2 );
}

void ForOutputDev::eoClip( GfxState *state )
{
  state->getUserClipBBox( &clip_x1, &clip_y1, &clip_x2, &clip_y2 );
}

void ForOutputDev::fill( GfxState *state )
{
  getFillColor( state );
  doPath( state, FILL );
}

void ForOutputDev::eoFill( GfxState *state )
{
  getFillColor( state );
  doPath( state, EOFILL );
}

void ForOutputDev::stroke( GfxState *state )
{
  getStrokeColor( state );
  doPath( state, STROKE );
}

void ForOutputDev::doPath( GfxState *state, int path_painting_operator )
{
#if POPPLER_CHECK_VERSION(0, 83, 0)
  const GfxPath *path = state->getPath( );
  const GfxSubpath *subpath;
#else
  GfxPath *path = state->getPath( );
  GfxSubpath *subpath;
#endif
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
  int dashLength;
  double dashStart;
#if POPPLER_CHECK_VERSION(22, 9, 0)
  const double *dashPattern;
  const std::vector<double> &dash = state->getLineDash(&dashStart);
  dashPattern = dash.data();
  dashLength = dash.size();
#else
  double *dashPattern;
  state->getLineDash(&dashPattern, &dashLength, &dashStart);
#endif

  current_path->dash_length  = dashLength;
  current_path->dash_pattern = (double*)malloc( dashLength * sizeof( double ) );
  for ( int i = 0; i < dashLength; ++i )
  {
    current_path->dash_pattern[ i ] = dashPattern[ i ];
  }
  current_path->dash_start   = dashStart;

  /* Color */
  if ( current_color_space != csDeviceCMYK )
  {
    current_path->color.r = colToByte( current_color_rgb.r );
    current_path->color.g = colToByte( current_color_rgb.g );
    current_path->color.b = colToByte( current_color_rgb.b );
    current_path->color.color_space = deviceRGB;
  }
  else
  {
    current_path->color.c = colToByte( current_color_cmyk.c );
    current_path->color.m = colToByte( current_color_cmyk.m );
    current_path->color.y = colToByte( current_color_cmyk.y );
    current_path->color.k = colToByte( current_color_cmyk.k );
    current_path->color.color_space = deviceCMYK;
  }
  current_path->color.opacity = current_opacity;

  /* Points */
  current_path->x = (double*)malloc( points_count * sizeof( double ) );
  current_path->y = (double*)malloc( points_count * sizeof( double ) );
  current_path->command = (int*)malloc( points_count * sizeof( int ) );
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
      current_path->x[ point_nr ] = subpath->getX( sub_nr );
      current_path->y[ point_nr ] = state->getPageHeight( ) - subpath->getY( sub_nr );
    }
  }
  current_path->object_pos = object_pos;
  current_path->char_pos = char_pos;

  Matrix *m = new Matrix;
  state->getCTM( m );
  m->m[ 3 ] *= -1.0;
  m->m[ 5 ] -= state->getPageHeight( );
  m->m[ 5 ] *= -1.0;

  for ( int i = 0; i < 6 ; i++ )
  {
    current_path->ctm[ i ] = m->m[ i ];
  }

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
  if ( current_color_space != csDeviceCMYK )
  {
    current_image->color.r = colToByte( current_color_rgb.r );
    current_image->color.g = colToByte( current_color_rgb.g );
    current_image->color.b = colToByte( current_color_rgb.b );
    current_image->color.color_space = deviceRGB;
  }
  else
  {
    current_image->color.c = colToByte( current_color_cmyk.c );
    current_image->color.m = colToByte( current_color_cmyk.m );
    current_image->color.y = colToByte( current_color_cmyk.y );
    current_image->color.k = colToByte( current_color_cmyk.k );
    current_image->color.color_space = deviceCMYK;
  }
  current_image->color.opacity = current_opacity;

  current_image->object_pos = object_pos;
  current_image->char_pos = char_pos;

  Matrix *m = new Matrix;
  state->getCTM( m );
  current_image->rotation = atan2( m->m[ 2 ], m->m[ 0 ] ) * 180 / PI;     // Rotation ausrechen
  current_image->rotation = -current_image->rotation + 360;               // Vorzeichen umdrehen um damit es gegen den Uhrzeigersinn rotiert
  current_image->rotation = (int)( current_image->rotation + 0.5 ) % 360; // Positiv machen

  current_image->x = m->m[ 4 ];
  current_image->y = m->m[ 5 ];

  /* Next */
  image_nr ++;
  object_pos ++;
  current_image->next = new Image;
  current_image = current_image->next;
}

void ForOutputDev::getStrokeColor( GfxState *state )
{
  current_opacity = state->getStrokeOpacity( );
  current_color_space = state->getStrokeColorSpace( )->getMode( );

  if ( current_color_space == csSeparation )
  {
    current_color_space = ( (GfxSeparationColorSpace*)state->getStrokeColorSpace( ) )->getAlt( )->getMode( );
  }

  if ( current_color_space == csDeviceCMYK )
  {
    state->getStrokeCMYK( &current_color_cmyk );
  }
  else
  {
    state->getStrokeRGB( &current_color_rgb );
  }
}

void ForOutputDev::getFillColor( GfxState *state )
{
  current_opacity = state->getFillOpacity( );
  current_color_space = state->getFillColorSpace( )->getMode( );

  if ( current_color_space == csSeparation )
  {
    current_color_space = ( (GfxSeparationColorSpace*)state->getFillColorSpace( ) )->getAlt( )->getMode( );
  }

  if ( current_color_space == csDeviceCMYK )
  {
    state->getFillCMYK( &current_color_cmyk );
  }
  else
  {
    state->getFillRGB( &current_color_rgb );
  }
}

Path* ForOutputDev::getPathList( )
{
  return path_list;
}

int ForOutputDev::getPathCount( )
{
  return path_number;
}

Image* ForOutputDev::getImageList( )
{
  return image_list;
}

unsigned int ForOutputDev::getImageCount( )
{
  return image_nr;
}
