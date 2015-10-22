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

  /* The definition for the RGBColor*/
  typedef struct
  {
    int red;
    int green;
    int blue;
    double alpha;
  }RGBAColor;

  /* The definition for enum PathCommand */
  enum PathCommand{
    MOVE_TO = 0,
    LINE_TO = 1,
    CURVE_TO = 2
  };
  typedef enum PathCommand PathCmd;

  /* The definition for the RGBColor*/
  typedef struct
  {
    int length;
    double *pattern;
    double start;
  }LineDash;

  /* The definition for enum LineCaps */
  enum LineCaps{
    CAP_BUTT = 0,
    CAP_ROUND = 1,
    CAP_SQUARE = 2
  };
  typedef enum LineCaps LineCap;

  /* The definition for enum LineJoins */
  enum LineJoins{
    JOIN_MITER = 0,
    JOIN_ROUND = 1,
    JOIN_BEVEL = 2
  };
  typedef enum LineJoins LineJoin;

  /* The definition for struct ForPath */
  typedef struct
  {
    unsigned int count;
    double *x;
    double *y;
    PathCmd *cmd;
    gboolean fill;
    RGBAColor color;
    double opacity;
    double line_weight;
    LineCap line_cap;
    LineJoin line_join;
    LineDash line_dash;
  }ForPath;

  /* The definition for struct word */
  typedef struct
  {
    double x1, y1, x2, y2, font_size;
    char *text;
  }Word;


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
