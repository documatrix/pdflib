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

  /* The definition for struct ForPath */
  typedef struct
  {
    unsigned int count;
    double *x;
    double *y;
    int *command;
    gboolean fill;
  }ForPath;

  /* The definition for enum PathCommand */
  enum PathCommand{
    MOVE_TO = 0,
    LINE_TO = 1,
    CURVE_TO = 2
  };

  /* The function to get a wordarray ( horizontal and vertical ) out of the PopplerPage */
  gboolean get_words( PopplerPage *page, Word **words, guint *n_words );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_word_destroy( Word *word );

  /* The function to get a patharray out of the PopplerPage */
  gboolean get_paths( PopplerPage *page, ForPath **paths, guint *n_paths );

#ifdef __cplusplus
}
#endif

#endif
