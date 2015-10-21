
[CCode (cheader_filename = "dm_poppler.h")]
namespace DMPoppler 
{
  [CCode (cname="PathCommand")]
  public enum PathCommand
  {
    [CCode (cname="MOVE_TO")]
    MOVE_TO,
    [CCode (cname="LINE_TO")]
    LINE_TO,
    [CCode (cname="CURVE_TO")]
    CURVE_TO
  }

  [CCode (cname = "Word")]
  public struct Word
  {
    public double x1;
    public double y1;
    public double x2;
    public double y2;
    public string text;
    public double font_size;
  }

  [CCode (cname = "ForPath")]
  public struct ForPath
  {
    public uint count;
    public double *x;
    public double *y;
    public int *command;
    public bool fill;
  }

  [CCode (cname = "get_words")]
  bool get_words( Poppler.Page *poppler_page, [CCode (array_length_cname = "n_words", array_length_pos = 2.1, array_length_type = "guint")] out Word[] words );

  [CCode (cname = "get_paths")]
  bool get_paths( Poppler.Page *poppler_page, [CCode (array_length_cname = "n_paths", array_length_pos = 2.1, array_length_type = "guint")] out ForPath[] paths );
}
