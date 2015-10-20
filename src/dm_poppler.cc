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

/* The function to get a wordarray ( horizontal and vertical ) out of the PopplerPage */
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

    word_i->text = myString;
    word_i->x1 = xMinA;
    word_i->y1 = yMinA;
    word_i->x2 = xMaxA;
    word_i->y2 = yMaxA;
    word_i->font_size = word->getFontSize( );

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
}

