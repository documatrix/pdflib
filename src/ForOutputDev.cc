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
  path_list = new Path;
}

ForOutputDev::~ForOutputDev()
{
}

void ForOutputDev::startPage( int pageNum, GfxState *state )
{
  printf( "startPage\n" );
  current_color.r = current_color.g = current_color.b = 0;
}

void ForOutputDev::endPage( )
{
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
  current_path->line_width = state->getLineWidth( );
  current_path->line_cap   = state->getLineCap( );
  current_path->line_join  = state->getLineJoin( );

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
  path_number ++;
  current_path->next = new Path;
  current_path = current_path->next;
}

