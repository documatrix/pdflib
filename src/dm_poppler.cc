//========================================================================
//
// dm_poppler.cc
//
// Copyright 2014 Documatrix GmbH
//
//========================================================================

#include <dm_poppler.h>

#include <poppler.h>
#include <poppler-private.h>
#include <config.h>

#include <poppler-config.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <goo/GooString.h>
#include <goo/gmem.h>
#include <GlobalParams.h>
#include <Object.h>
#include <Stream.h>
#include <Array.h>
#include <Dict.h>
#include <XRef.h>
#include <Catalog.h>
#include <Page.h>
#include <PDFDoc.h>
#include <PDFDocFactory.h>
#include <TextOutputDev.h>
#include <CharTypes.h>
#include <UnicodeMap.h>
#include <PDFDocEncoding.h>
#include <Error.h>
#include <ForOutputDev.h>

/**
 * This function can be used to read a word array ( horizontal and vertical ) out of the PopplerPage.
 * @param page The current PopplerPage of the PDF Document.
 * @param words The word array containing the Rectangle and the word text.
 * @param n_words The length of the words array.
 * @return True if the words were found.
 */
gboolean get_words( PopplerPage *page, Word** words, guint *n_words )
{
  TextOutputDev *text_dev;
  Gfx *gfx;

  text_dev = new TextOutputDev( NULL, gTrue, 0, gFalse, gFalse );
  gfx = page->page->createGfx( text_dev,
                               72.0, 72.0, 0,
                               gFalse, /* useMediaBox */
                               gTrue, /* Crop */
                               -1, -1, -1, -1,
                               gFalse, /* printing */
                               NULL, NULL );

  page->page->display( gfx );
  text_dev->endPage( );

  TextWordList *wordlist = text_dev->makeWordList( );
  const int word_length = wordlist != NULL ? wordlist->getLength( ) : 0;

  /* no word found */
  if ( word_length == 0 )
  {
    return gFalse;
  }

  TextWord *word;
  *words = g_new( Word, word_length );
  Word* word_i;
  GooString* myText;
  double xMinA, yMinA, xMaxA, yMaxA;

  *n_words = word_length;

  /* Save the wordlist into an wordarray */
  for ( int i = 0; i < word_length; i ++ )
  {
    word = wordlist->get( i );
    word->getBBox( &xMinA, &yMinA, &xMaxA, &yMaxA );

    myText = word->getText( );

    char* myString = strdup( myText->getCString( ) );

    word_i = *words + i;
    printf( "Text, %s, Pos %d \n", myString, word->getCharPos( ) );
    word_i->text = myString;
    word_i->x1 = xMinA;
    word_i->y1 = yMinA;
    word_i->x2 = xMaxA;
    word_i->y2 = yMaxA;
    word_i->font_size = word->getFontSize( );
    word_i->baseline = word->getBaseline( );
    word_i->char_count = word->getLength( );
    word_i->edge_count = word_i->char_count + 1;
    word_i->edges = (double*)malloc( word_i->edge_count * sizeof( double* ) );
    for ( int i = 0; i < word_i->edge_count; i ++ )
    {
      word_i->edges[ i ] = word->getEdge( i );
    }

    delete myText;
  }
  delete wordlist;
  delete gfx;
  delete text_dev;

  return gTrue;
}

/* The destroy funtion of the struct word -> no memoryleak */
void dm_poppler_word_destroy( Word *word )
{
  free( (void*)word->text );
  free( (void*)word->edges );
}

/**
 * This function can be used to read a path array ( horizontal and vertical ) out of the PopplerPage.
 * @param page The current PopplerPage of the PDF Document.
 * @param paths The path array containing the the line or curve points.
 * @param n_paths The length of the paths array.
 * @return True if the paths were found.
 */
gboolean get_elements( PopplerPage *page, ForPath **paths, guint *n_paths, ForImage **images, guint *n_images )
{
  ForOutputDev *for_dev;
  TextOutputDev *text_dev;
  Gfx *gfx;
  GError *error;

  GList *mapping, *l;
  mapping = poppler_page_get_image_mapping( page );

  for_dev = new ForOutputDev( );
  gfx = page->page->createGfx(
                               for_dev,
                               72.0, 72.0, 0,
                               gFalse, /* useMediaBox */
                               gTrue,  /* Crop */
                               -1, -1, -1, -1,
                               gFalse, /* printing */
                               NULL, NULL
                             );

  page->page->display( gfx );
  for_dev->endPage( );

  Path *current_path = for_dev->path_list;
  int path_number = for_dev->path_number;

  Image *current_image = for_dev->image_list;
  int image_number = for_dev->image_nr;

  /* no paths and images found */
  if ( path_number == 0 && image_number == 0 )
  {
    return gFalse;
  }

  *paths = g_new( ForPath, path_number );
  *n_paths = path_number;

  ForPath *path_i;

  /* Save the path_linklist into an path_array */
  for ( int i = 0; i < path_number; i ++ )
  {
    path_i = *paths + i;

    /* Path Points */
    path_i->count = current_path->count;
    path_i->x = current_path->x;
    path_i->y = current_path->y;
    path_i->cmd = (PathCmd*)current_path->command;

    /* Path Settings */
    path_i->color.red   = current_path->color_red;
    path_i->color.green = current_path->color_green;
    path_i->color.blue  = current_path->color_blue;
    path_i->color.alpha = current_path->opacity;

    path_i->fill        = (PathPaintOp)current_path->fill;
    path_i->line_weight = current_path->line_width;
    path_i->line_cap    = (DMLineCap)current_path->line_cap;
    path_i->line_join   = (DMLineJoin)current_path->line_join;
    path_i->miter_limit = current_path->miter_limit;

    /* LineDash */
    path_i->line_dash.length  = current_path->dash_length;
    path_i->line_dash.pattern = current_path->dash_pattern;
    path_i->line_dash.start   = current_path->dash_start;

    /* Object Sortierung */
    path_i->char_pos   = current_path->char_pos;
    path_i->object_pos = current_path->object_pos;

    current_path = current_path->next;
  }

  *images = g_new( ForImage, image_number );
  *n_images = image_number;

  ForImage *image_i;

  /* Save the path_linklist into an path_array */
  for ( int i = 0; i < image_number; i ++ )
  {
    image_i = *images + i;

    /* Image Settings */
    image_i->color.red     = current_image->color_red;
    image_i->color.green   = current_image->color_green;
    image_i->color.blue    = current_image->color_blue;
    image_i->color.alpha   = current_image->opacity;
    image_i->height        = current_image->height;
    image_i->width         = current_image->width;
    image_i->id            = current_image->id;
    image_i->file_position = current_image->file_pos;

    PopplerImageMapping *imapping;
    for ( l = mapping; l; l = g_list_next( l ) )
    {
      imapping = (PopplerImageMapping *)l->data;
      if ( imapping->image_id == image_i->id )
      {
        break;
      }
    }
    if ( imapping != NULL )
    {
      image_i->area.x1 = imapping->area.x1;
      image_i->area.y1 = imapping->area.y1;
      image_i->area.x2 = imapping->area.x2;
      image_i->area.y2 = imapping->area.y2;
    }

    /* Object Sortierung */
    image_i->char_pos   = current_image->char_pos;
    image_i->object_pos = current_image->object_pos;

    current_image = current_image->next;
  }

  return gTrue;
}

/* The destroy funtion of the struct for path -> no memoryleak */
void dm_poppler_for_path_destroy( ForPath *path )
{
  free( (void*)path->line_dash.pattern );
  free( (void*)path->cmd );
  free( (void*)path->x );
  free( (void*)path->y );
}

/* The destroy funtion of the struct image -> no memoryleak */
void dm_poppler_for_image_destroy( Image *img )
{
}