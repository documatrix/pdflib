
[CCode (cheader_filename = "dm_poppler.h")]
namespace DMPoppler 
{
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

  [CCode (cname = "get_words")]
  bool get_words( Poppler.Page *poppler_page, [CCode (array_length_cname = "n_words", array_length_pos = 2.1, array_length_type = "guint")] out Word[] word );
}
