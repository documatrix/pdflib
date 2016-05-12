using DMPoppler;
using Poppler;
using Testlib;

namespace PDFLibTests
{
  public static int main( string[] args )
  {
    GLib.Test.init( ref args );

    GLib.TestSuite ts_pdflib = new GLib.TestSuite( "pdflib" );
    GLib.TestSuite.get_root( ).add_suite( ts_pdflib );

    /* DMPoppler */
    GLib.TestSuite ts_dm_poppler = new GLib.TestSuite( "DMPoppler" );
    ts_dm_poppler.add( new GLib.TestCase( "test_f_get_words", default_setup, test_f_get_words, default_teardown ) );
    ts_dm_poppler.add( new GLib.TestCase( "test_f_get_paths", default_setup, test_f_get_paths, default_teardown ) );
    ts_dm_poppler.add( new GLib.TestCase( "test_s_color", default_setup, test_s_color, default_teardown ) );
    ts_pdflib.add_suite( ts_dm_poppler );

    GLib.Test.run( );
    return 0;
  }

  /**
   * This test-case tests the get_words method.
   */
  public static void test_f_get_words( )
  {
    Word[]? words = null;
    PDFLibTests.get_test_elements( "../tests/test_with_img.pdf", out words, null, null );

    GLib.assert( words != null );

    bool found_word_1 = false;
    bool found_word_2 = false;
    foreach ( unowned Word word in (!)words )
    {
      if ( word.text == "mitten" )
      {
        GLib.assert( (int32)word.x1 == 454 );
        GLib.assert( (int32)word.y1 == 505 );
        GLib.assert( (int32)word.x2 == 481 );
        GLib.assert( (int32)word.y2 == 514 );
        GLib.assert( (int32)word.edges[ 0 ] == 454 );
        GLib.assert( (int32)word.baseline == 512 );
        GLib.assert( (int32)word.font_size == 10 );
        GLib.assert( word.font_name == "Helvetica" );
        GLib.assert( (int32)word.rotation == 0 );
        found_word_1 = true;
      }
      else if ( word.text == "rotiert" )
      {
        GLib.assert( (int32)word.x1 == 95 );
        GLib.assert( (int32)word.y1 == 750 );
        GLib.assert( (int32)word.x2 == 104 );
        GLib.assert( (int32)word.y2 == 775 );
        GLib.assert( (int32)word.edges[ 0 ] == 775 );
        GLib.assert( (int32)word.baseline == 102 );
        GLib.assert( (int32)word.font_size == 10 );
        GLib.assert( word.font_name == "Helvetica" );
        GLib.assert( (int32)word.rotation == 90 );
        found_word_2 = true;
      }
    }

    GLib.assert( found_word_1 );
    GLib.assert( found_word_2 );
  }

  /**
   * This test-case tests the colors of the different elements.
   */
  public static void test_s_color( )
  {
    Word[]? words = null;
    ForPath[]? paths = null;
    ForImage[]? images = null;
    PDFLibTests.get_test_elements( "../tests/test_with_img.pdf", out words, out paths, out images );

    GLib.assert( words != null );
    GLib.assert( paths != null );
    GLib.assert( images != null );

    bool found_word_1 = false;
    bool found_word_2 = false;
    foreach ( unowned Word word in (!)words )
    {
      if ( word.text == "mitten" )
      {
        found_word_1 = true;
        GLib.assert( word.color.color_space == ForColorSpace.RGB );
        GLib.assert( word.color.r == 255 );
        GLib.assert( word.color.g == 0 );
        GLib.assert( word.color.b == 0 );
        GLib.assert( word.color.alpha == 1.0 );
        GLib.assert( word.color.get_html_color( ) == "rgb(255,0,0)" );
      }
      else if ( word.text == "links1" )
      {
        found_word_2 = true;
        GLib.assert( word.color.color_space == ForColorSpace.RGB );
        GLib.assert( word.color.r == 127 );
        GLib.assert( word.color.g == 191 );
        GLib.assert( word.color.b == 255 );
        GLib.assert( word.color.alpha == 1.0 );
        GLib.assert( word.color.get_html_color( ) == "rgb(127,191,255)" );
      }
    }
    GLib.assert( found_word_1 );
    GLib.assert( found_word_2 );

    bool found_img_1 = false;
    bool found_img_2 = false;
    foreach ( unowned ForImage img in (!)images )
    {
      if ( img.width == 118 && img.height == 118 && img.area.x1 >= 10 && img.area.y1 >= 169 && img.area.x2 <= 24 && img.area.y2 <= 184 )
      {
        found_img_1 = true;
        GLib.assert( img.color.color_space == ForColorSpace.RGB );
        GLib.assert( img.color.r == 0 );
        GLib.assert( img.color.g == 255 );
        GLib.assert( img.color.b == 0 );
        GLib.assert( img.color.alpha == 1.0 );
        GLib.assert( img.color.get_html_color( ) == "rgb(0,255,0)" );
      }
      else if ( img.width == 118 && img.height == 118 && img.area.x1 >= 10 && img.area.y1 >= 69 && img.area.x2 <= 24 && img.area.y2 <= 84 )
      {
        found_img_2 = true;
        GLib.assert( img.color.color_space == ForColorSpace.CMYK );
        GLib.assert( img.color.c == 255 );
        GLib.assert( img.color.m == 0 );
        GLib.assert( img.color.y == 0 );
        GLib.assert( img.color.k == 0 );
        GLib.assert( img.color.alpha == 1.0 );
        GLib.assert( img.color.get_html_color( ) == "cmyk(255,0,0,0)" );
      }
    }
    GLib.assert( found_img_1 );
    GLib.assert( found_img_2 );

    bool found_path_1 = false;
    bool found_path_2 = false;
    foreach ( unowned ForPath path in (!)paths )
    {
      if ( path.cmd.length == 2 && path.cmd[ 0 ] == PathCmd.MOVE_TO && path.x[ 0 ] == 0 && (int32)path.y[ 0 ] == 661 && path.cmd[ 1 ] == PathCmd.LINE_TO && path.x[ 1 ] == 200 && (int32)path.y[ 1 ] == 661 )
      {
        found_path_1 = true;
        GLib.assert( path.color.color_space == ForColorSpace.RGB );
        GLib.assert( path.color.r == 255 );
        GLib.assert( path.color.g == 0 );
        GLib.assert( path.color.b == 0 );
        GLib.assert( path.color.alpha == 1.0 );
        GLib.assert( path.color.get_html_color( ) == "rgb(255,0,0)" );
      }
      else if ( path.cmd.length == 2 && path.cmd[ 0 ] == PathCmd.MOVE_TO && path.x[ 0 ] == 0 && (int32)path.y[ 0 ] == 656 && path.cmd[ 1 ] == PathCmd.LINE_TO && path.x[ 1 ] == 200 && (int32)path.y[ 1 ] == 656 )
      {
        found_path_2 = true;
        GLib.assert( path.color.color_space == ForColorSpace.CMYK );
        GLib.assert( path.color.c == 255 );
        GLib.assert( path.color.m == 0 );
        GLib.assert( path.color.y == 0 );
        GLib.assert( path.color.k == 0 );
        GLib.assert( path.color.alpha == 1.0 );
        GLib.assert( path.color.get_html_color( ) == "cmyk(255,0,0,0)" );
      }
    }
    GLib.assert( found_path_1 );
    GLib.assert( found_path_2 );
  }

  /**
   * This test-case tests the get_paths method.
   */
  public static void test_f_get_paths( )
  {
    //repair_template oder test_with_img
    File test_pdf = File.new_for_path( "../tests/test_with_img.pdf" );
    try
    {
      string uri = test_pdf.get_uri( );
      stdout.printf( "%s\n", uri );
      Document document = new Poppler.Document.from_file( uri, "" );

      ForPath[] paths = null;
      ForImage[] images = null;
      for ( int i = 0; i < document.get_n_pages( ); i ++ )
      {
        Poppler.Page page = document.get_page( i );
        GLib.assert( DMPoppler.get_elements( page, out paths, out images ) );
        // GLib.assert( paths.length == 3 );
        // GLib.assert( paths[ 0 ].x.length == 8 );
        // GLib.assert( paths[ 0 ].line_weight < 11 );
        // GLib.assert( paths[ 0 ].line_weight > 9 );
        // GLib.assert( (int)paths[ 2 ].x[ 1 ] == 450 );
        // GLib.assert( (int)paths[ 2 ].y[ 1 ] == 328 );
        // GLib.assert( paths[ 2 ].cmd[ 1 ] == PathCmd.LINE_TO );
        // GLib.assert( !paths[ 2 ].fill );

        stdout.printf( "Path count %d\n", paths.length );
        foreach ( unowned ForPath path in paths )
        {
          stdout.printf( "Path: CharPosition:%d, ObjectPosition:%d\n", path.char_pos, path.object_pos );
          stdout.printf( "  Dash Start:%3.2f Pattern:", path.line_dash.start );
          for ( int dash_nr = 0; dash_nr < path.line_dash.pattern.length; dash_nr ++ )
          {
            stdout.printf( " |%3.2f", path.line_dash.pattern[ dash_nr ] );
          }
          stdout.printf( "\n" );
          stdout.printf( "  R%03u,G:%03u,B:%03u,A:%lf\n  LineWeight:%lf Fill:%s LineCap:%s LineJoin:%s MiterLimit:%f\n",
            path.color.r,
            path.color.g,
            path.color.b,
            path.color.alpha,
            path.line_weight,
            path.path_painting_operator.to_string( ),
            path.line_cap.to_string( ),
            path.line_join.to_string( ),
            path.miter_limit
          );
          for ( int point_nr = 0; point_nr < path.x.length; point_nr ++ )
          {
            stdout.printf( "  X%f Y%f %s\n", path.x[ point_nr ], path.y[ point_nr ], path.cmd[ point_nr ].to_string( ) );
          }
          stdout.printf( "\n" );
        }

        stdout.printf( "Image count %d\n", images.length );
        foreach ( unowned ForImage img in images )
        {
          stdout.printf( "Image: CharPosition:%d, ObjectPosition:%d\n", img.char_pos, img.object_pos );
          stdout.printf( "  ID:%d, FilePosition:%lld, Width:%d, Height:%d\n",
            img.id,
            img.file_position,
            img.width,
            img.height
          );
          stdout.printf( "  R:%03u,G:%03u,B:%03u,A:%lf\n",
            img.color.r,
            img.color.g,
            img.color.b,
            img.color.alpha
          );
          stdout.printf( "  X1: %3.2f, Y1: %3.2f, X2: %3.2f, Y2: %3.2f\n",
            img.area.x1,
            img.area.y1,
            img.area.x2,
            img.area.y2
          );
          stdout.printf( "\n" );
        }
      }
    }
    catch ( GLib.Error e )
    {
      GLib.assert( false );
    }
  }

  /**
   * Returns all words, paths and images from the first page of the given PDF.
   * @param filename The pdf filename.
   * @param words The words.
   * @param paths The paths.
   * @param images The images.
   */
  public static void get_test_elements( string filename, out Word[]? words, out ForPath[]? paths, out ForImage[]? images )
  {
    File test_pdf = File.new_for_path( filename );
    try
    {
      Document document = new Poppler.Document.from_file( test_pdf.get_uri( ), "" );

      Page page = document.get_page( 0 );

      GLib.assert( DMPoppler.get_words( page, out words ) );
      GLib.assert( DMPoppler.get_elements( page, out paths, out images ) );
    }
    catch ( GLib.Error e )
    {
      GLib.assert( false );
    }
  }
}
