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
}

void ForOutputDev::endPage( )
{
}


void ForOutputDev::stroke( GfxState *state )
{
  doPath( state, false );
}

void ForOutputDev::fill( GfxState *state )
{
  doPath( state, true );
}

void ForOutputDev::doPath( GfxState *state, bool fill )
{
  static Path *current_path = path_list;
  GfxPath *path = state->getPath( );
  GfxSubpath *subpath;
  double *x, *y;

  current_path->fill = fill;
  int points_count = 0;
  for ( int i = 0; i < path->getNumSubpaths( ); ++i )
  {
    subpath = path->getSubpath( i );
    points_count += subpath->getNumPoints( );
  }
  current_path->x = (double*)malloc( points_count * sizeof( double* ) );
  current_path->y = (double*)malloc( points_count * sizeof( double* ) );
  current_path->command = (int*)malloc( points_count * sizeof( int* ) );
  current_path->count = points_count;
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

