using DMPoppler;
using Poppler;
using Testlib;

namespace PDFLibTests
{
  public static int main( string[] args )
  {
    GLib.Test.init( ref args );

    GLib.TestSuite ts_dm_poppler = new GLib.TestSuite( "pdflib" );
    GLib.TestSuite.get_root( ).add_suite( ts_dm_poppler );

    /* DMPoppler */
    GLib.TestSuite ts_get_words = new GLib.TestSuite( "pdflib" );
    ts_get_words.add( new GLib.TestCase( "test_f_get_words", default_setup, test_f_get_words, default_teardown ) );
    ts_dm_poppler.add_suite( ts_get_words );

    GLib.TestSuite ts_get_paths = new GLib.TestSuite( "pdflib" );
    ts_get_paths.add( new GLib.TestCase( "test_f_get_paths", default_setup, test_f_get_paths, default_teardown ) );
    ts_dm_poppler.add_suite( ts_get_paths );

    GLib.Test.run( );
    return 0;
  }

  /**
   * This test-case tests the get_words method.
   */
  public static void test_f_get_words( )
  {
    File test_pdf = File.new_for_path( "../tests/test.pdf" );
    try
    {
      string uri = test_pdf.get_uri( );
      stdout.printf( "%s\n", uri );
      Document document = new Poppler.Document.from_file( uri, "" );

      Page page1 = document.get_page( 0 );

      Word[] words = null;
      DMPoppler.get_words( page1, out words );
      GLib.assert( words.length == 10 );
      for ( int i = 0; i < words.length; i ++ )
      {
        if ( words[ i ].text == "unten" )
        {
          GLib.assert( (int)words[ i ].y1 == 767 );
          GLib.assert( (int)words[ i ].y2 == 776 );
        }
        if ( words[ i ].text == "links" )
        {
          GLib.assert( (int)words[ i ].x1 == 100 );
          GLib.assert( (int)words[ i ].x2 == 120 );
        }
      }
    }
    catch ( Poppler.Error e )
    {
      GLib.assert( false );
    }
  }

  /**
   * This test-case tests the get_paths method.
   */
  public static void test_f_get_paths( )
  {
    File test_pdf = File.new_for_path( "../tests/test.pdf" );
    try
    {
      string uri = test_pdf.get_uri( );
      stdout.printf( "%s\n", uri );
      Document document = new Poppler.Document.from_file( uri, "" );

      Page page1 = document.get_page( 0 );
      ForPath[] paths = null;
      GLib.assert( DMPoppler.get_paths( page1, out paths ) );
      GLib.assert( (int)paths[ 1 ].x[ 1 ] == 450 );
      GLib.assert( (int)paths[ 1 ].y[ 1 ] == 328 );
      GLib.assert( paths[ 1 ].command[ 1 ] == PathCommand.LINE_TO );
      GLib.assert( !paths[ 1 ].fill );
    }
    catch ( Poppler.Error e )
    {
      GLib.assert( false );
    }
  }
}
