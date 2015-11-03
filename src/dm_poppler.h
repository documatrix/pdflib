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
#include <stdint.h>

  /* The definition for the RGBColor*/
  typedef struct
  {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    double alpha;
  }RGBAColor;

  /* The definition for the For Rectangle*/
  typedef struct
  {
    double x1;
    double x2;
    double y1;
    double y2;
  }Rectangle;

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
  typedef enum LineCaps DMLineCap;

  /* The definition for enum LineJoins */
  enum LineJoins{
    JOIN_MITER = 0,
    JOIN_ROUND = 1,
    JOIN_BEVEL = 2
  };
  typedef enum LineJoins DMLineJoin;

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
    DMLineCap line_cap;
    DMLineJoin line_join;
    LineDash line_dash;
    double miter_limit;
    Rectangle clip;
    int char_pos;
    int object_pos;
  }ForPath;

  /* The definition for struct word */
  typedef struct
  {
    double x1, y1, x2, y2, font_size, baseline;
    char *text;
    double *edges;
    int edge_count;
    int char_count;
  }Word;

  /* The definition for struct Image */
  typedef struct
  {
    unsigned int id;
    int64_t file_position;
    RGBAColor color;
    int width;
    int height;

    int char_pos;
    int object_pos;
  }ForImage;

  /* The function to get a wordarray ( horizontal and vertical ) out of the PopplerPage */
  gboolean get_words( PopplerPage *page, Word **words, guint *n_words );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_word_destroy( Word *word );

  /* The function to get a patharray out of the PopplerPage */
  gboolean get_for_elements( PopplerPage *page, ForPath **paths, guint *n_paths, ForImage **images, guint *n_images );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_for_path_destroy( ForPath *path );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_for_image_destroy( ForImage *image );

#ifdef __cplusplus
}
#endif

#endif
