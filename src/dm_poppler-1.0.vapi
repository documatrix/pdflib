
[CCode (cheader_filename = "dm_poppler.h")]
namespace DMPoppler
{
  [CCode (cname="PathCmd")]
  public enum PathCmd
  {
    [CCode (cname="MOVE_TO")]
    MOVE_TO,
    [CCode (cname="LINE_TO")]
    LINE_TO,
    [CCode (cname="CURVE_TO")]
    CURVE_TO;

    public string to_string( )
    {
      switch ( this )
      {
        case MOVE_TO:
          return "moveto";
        case LINE_TO:
          return "lineto";
        case CURVE_TO:
          return "curveto";
         default:
          return "Undefined";
      }
    }
  }

  [CCode (cname="PathPaintOp")]
  public enum PathPaintingOperator
  {
    [CCode (cname="STROKE")]
    STROKE,
    [CCode (cname="FILL")]
    FILL,
    [CCode (cname="EOFILL")]
    EOFILL;

    public string to_string( )
    {
      switch ( this )
      {
        case STROKE:
          return "stroke";
        case FILL:
          return "fill";
        case EOFILL:
          return "eofill";
         default:
          return "Undefined";
      }
    }
  }

  [CCode (cname="DMLineCap")]
  public enum LineCap
  {
    [CCode (cname="CAP_BUTT")]
    BUTT,
    [CCode (cname="CAP_ROUND")]
    ROUND,
    [CCode (cname="CAP_SQUARE")]
    SQUARE;

    public string to_string( )
    {
      switch ( this )
      {
        case BUTT:
          return "butt";
        case ROUND:
          return "round";
        case SQUARE:
          return "square";
         default:
          return "Undefined";
      }
    }
  }

  [CCode (cname="DMLineJoin")]
  public enum LineJoin
  {
    [CCode (cname="JOIN_MITER")]
    MITER,
    [CCode (cname="JOIN_ROUND")]
    ROUND,
    [CCode (cname="JOIN_BEVEL")]
    BEVEL;

    public string to_string( )
    {
      switch ( this )
      {
        case MITER:
          return "miter";
        case ROUND:
          return "round";
        case BEVEL:
          return "bevel";
         default:
          return "Undefined";
      }
    }
  }

  [CCode (cname="LineDash")]
  public struct LineDash
  {
    [CCode (array_length_cname = "length")]
    public double[] pattern;
    public double start;
  }

  [CCode (cname="ForColorSpace")]
  public enum ForColorSpace
  {
    [CCode (cname="forDeviceRGB")]
    RGB,
    [CCode (cname="forDeviceCMYK")]
    CMYK;

    public string to_string( )
    {
      switch ( this )
      {
        case RGB:
          return "RGB";
        case CMYK:
          return "CMYK";
         default:
          return "Undefined";
      }
    }
  }

  [CCode (cname="ForColor")]
  public struct ForColor
  {
    public uint8 r;
    public uint8 g;
    public uint8 b;

    public uint8 c;
    public uint8 m;
    public uint8 y;
    public uint8 k;

    public double alpha;

    public ForColorSpace color_space;

    /**
     * This method will return CSS code for use in HTML files which represents the color.
     * @return The CSS code which represents the color.
     */
    public string get_html_color( )
    {
      if ( this.color_space == ForColorSpace.RGB )
      {
        return "rgb(%u,%u,%u)".printf( this.r, this.g, this.b );
      }
      if ( this.color_space == ForColorSpace.CMYK )
      {
        return "cmyk(%u,%u,%u,%u)".printf( this.c, this.m, this.y, this.k );
      }
      return "Undefined";
    }
  }

  [CCode (cname="ForRectangle")]
  public struct ForRectangle
  {
    public double x1;
    public double x2;
    public double y1;
    public double y2;
  }

  [CCode (cname = "Word")]
  public struct Word
  {
    public double x1;
    public double y1;
    public double x2;
    public double y2;
    public string text;
    public string font_name;
    public double font_size;
    public double baseline;
    public int32 char_count;
    [CCode (array_length_cname = "edge_count")]
    public double[] edges;
    public int32 char_pos;
    public ForColor color;
    public int32 rotation;
  }

  [CCode (cname = "ForPath")]
  public struct ForPath
  {
    [CCode (array_length_cname = "count", array_length_type = "guint")]
    public double[] x;
    [CCode (array_length_cname = "count", array_length_type = "guint")]
    public double[] y;
    [CCode (array_length_cname = "count", array_length_type = "guint")]
    public PathCmd[] cmd;
    public PathPaintingOperator path_painting_operator;
    public ForColor color;
    public double line_weight;
    public LineCap line_cap;
    public LineJoin line_join;
    public LineDash line_dash;
    public double miter_limit;
    public int32 char_pos;
    public int32 object_pos;
    public double ctm[ 6 ];
  }

  [CCode (cname = "ForImage")]
  public struct ForImage
  {
    public int32 id;
    public int64 file_position;
    public ForColor color;
    public int32 height;
    public int32 width;
    public ForRectangle area;

    public int32 char_pos;
    public int32 object_pos;
  }

  [CCode (cname = "get_words")]
  bool get_words( Poppler.Page *poppler_page, [CCode (array_length_cname = "n_words", array_length_pos = 2.1, array_length_type = "guint")] out Word[] words );

  [CCode (cname = "get_elements")]
  bool get_elements( Poppler.Page *poppler_page, [CCode (array_length_cname = "n_paths", array_length_pos = 2.1, array_length_type = "guint")] out ForPath[] paths, [CCode (array_length_cname = "n_images", array_length_pos = 4.1, array_length_type = "guint")] out ForImage[] images );
}
