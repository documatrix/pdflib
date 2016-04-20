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

  /* The different color spaces */
  enum _ForColorSpace
  {
    forDeviceRGB,
    forDeviceCMYK
  };
  typedef enum _ForColorSpace ForColorSpace;

  /* The definition for the RGBColor*/
  typedef struct
  {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    uint8_t c;
    uint8_t m;
    uint8_t y;
    uint8_t k;

    double alpha;

    ForColorSpace color_space;
  }ForColor;

  /* The definition for the For Rectangle*/
  typedef struct
  {
    double x1;
    double x2;
    double y1;
    double y2;
  }ForRectangle;

  /* The definition for enum PathCommand */
  enum PathCommand{
    MOVE_TO = 0,
    LINE_TO = 1,
    CURVE_TO = 2
  };
  typedef enum PathCommand PathCmd;

  /* The definition for enum PathCommand */
  enum PathPaintingOperator{
    STROKE = 0,
    FILL = 1,
    EOFILL = 2
  };
  typedef enum PathPaintingOperator PathPaintOp;

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
    PathPaintOp path_painting_operator;
    ForColor color;
    double opacity;
    double line_weight;
    DMLineCap line_cap;
    DMLineJoin line_join;
    LineDash line_dash;
    double miter_limit;
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
    int char_pos;
    ForColor color;
    int rotation;
    const char *font_name;
  }Word;

  /* The definition for struct Image */
  typedef struct
  {
    unsigned int id;
    int64_t file_position;
    ForColor color;
    int width;
    int height;
    ForRectangle area;

    int char_pos;
    int object_pos;
  }ForImage;

  /* The function to get a wordarray ( horizontal and vertical ) out of the PopplerPage */
  gboolean get_words( PopplerPage *page, Word **words, guint *n_words );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_word_destroy( Word *word );

  /* The function to get a patharray out of the PopplerPage */
  gboolean get_elements( PopplerPage *page, ForPath **paths, guint *n_paths, ForImage **images, guint *n_images );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_for_path_destroy( ForPath *path );

  /* The destroy funtion of the struct word -> no memoryleak */
  void dm_poppler_for_image_destroy( ForImage *image );

#ifdef __cplusplus
}
#endif

#endif
