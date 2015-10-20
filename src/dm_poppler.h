//========================================================================
//
// dm_poppler.h
//
// Copyright 2014 Documatrix GmbH
//
//========================================================================

//========================================================================
//
//========================================================================


#ifndef DM_POPPLER_H
#define DM_POPPLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <poppler.h>
#include <poppler-page.h>

  /* The definition for struct word */
  typedef struct
  {
    double x1, y1, x2, y2, font_size;
    char *text;
  }Word;

  /* The function to get a word array ( horizontal and vertical ) out of the PopplerPage */
  gboolean get_words( PopplerPage *page, Word **words, guint *n_words );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_word_destroy( Word *word );

#ifdef __cplusplus
}
#endif

#endif
