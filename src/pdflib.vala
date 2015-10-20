using Poppler;
using DMPoppler;

namespace PDFLib
{
  /**
   * This class can be used to read PDF file.
   */
  public class PDFReader : GLib.Object
  {
    /**
     * The PDF document.
     */
    private Document document;

    /**
     * The current page number.
     */
    private int32 current_page_nr = 0;

    /**
     * The page conut of the PDF file.
     */
    private int32 page_count = 0;

    /**
     * Creates a new PDFReader object.
     * @param pdf_file The input pdf file which will be read.
     * @param password The optional password of the pdf_file.
     * @throws GLib.Error if the pdf_file could not be opened for reading.
     */
    public PDFReader( string pdf_file, string password = "" ) throws GLib.Error
    {
      File input = File.new_for_path( pdf_file );
      string uri = Filename.to_uri( input.get_path( ) );

      this.document = new Poppler.Document.from_file( uri, password );

      this.page_count = this.document.get_n_pages( );
    }

    /**
     * Reads a page from the PDF file and returns it.
     * @return The read PDFPage or null if all pages were read.
     */
    public PDFPage? read_page( )
    {
      if ( this.current_page_nr >= this.page_count )
      {
        return null;
      }

      Page page = this.document.get_page( this.current_page_nr );
      PDFPage pdf_page = new PDFPage( );

      page.get_size( out pdf_page.width, out pdf_page.height );

      DMPoppler.get_words( page, out pdf_page.words );

      this.current_page_nr ++;
      return pdf_page;
    }
  }

  /**
   * This class represents a PDF page.
   */
  public class PDFPage : GLib.Object
  {
    /**
     * The height of the page.
     */
    public double height;

    /**
     * The width of the page.
     */
    public double width;

    /**
     * An array containing all words of the page.
     */
    public DMPoppler.Word[]? words = null;
  }
}
