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

      PDFPage pdf_page = new PDFPage.from_poppler_page( this.document.get_page( this.current_page_nr ) );

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
     * The poppler page.
     */
    private Page page;

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

    /**
     * An array containing all images of the page.
     */
    public PDFImage[]? images = null;

    /**
     * Creates a new PDFPage from a Poppler.Page.
     * @param page The Poppler.Page.
     */
    public PDFPage.from_poppler_page( Page page )
    {
      this.page = page;

      this.page.get_size( out this.width, out this.height );

      DMPoppler.get_words( this.page, out this.words );

      this.read_images( );
    }

    /**
     * Reads the images from the Poppler.Page.
     */
    private void read_images( )
    {
      List<ImageMapping> img_mapping = this.page.get_image_mapping( );
      uint32 length = img_mapping.length( );

      this.images = new PDFImage[ length ];

      for ( int32 i = 0; i < length; i ++ )
      {
        this.images[ i ] = new PDFImage.from_image_mapping( img_mapping.nth_data( i ) );
      }
    }

    /**
     * Loads the image data for the given PDFImage.
     */
    public void load_image_data( PDFImage pdf_image )
    {
      pdf_image.set_data_from_surface( this.page.get_image( pdf_image.id ) );
    }
  }

  /**
   * This class represents a PDF image.
   */
  public class PDFImage : GLib.Object
  {
    /**
     * The X coordinate of the upper left corner.
     */
    public double x1;

    /**
     * The Y coordinate of the upper left corner.
     */
    public double y1;

    /**
     * The X coordinate of the lower right corner.
     */
    public double x2;

    /**
     * The Y coordinate of the lower right corner.
     */
    public double y2;

    /**
     * The image id on the page.
     */
    public int32 id;

    /**
     * The unique image id.
     */
    public string key;

    /**
     * The image data.
     */
    public uint8[] data;

    /**
     * Creates a new PDFImages from the given Poppler.ImageMapping.
     * @param img_map The Poppler.ImageMapping.
     */
    public PDFImage.from_image_mapping( ImageMapping img_map )
    {
      Rectangle area = img_map.area;
      this.x1 = area.x1;
      this.y1 = area.y1;
      this.x2 = area.x2;
      this.y2 = area.y2;
      this.id = img_map.image_id;

      //TODO unique img id
      this.key = "img%d".printf( this.id );
    }

    /**
     * Loads the data from the given Cairo.Surface and stores in into PDFImage.data.
     * @param surface The Cairo.Surface from which the image data will be loaded.
     */
    public void set_data_from_surface( Cairo.Surface surface )
    {
      void* data = null;
      uint len = 0;

      surface.write_to_png_stream( ( imgdata ) =>
      {
        data = GLib.realloc( data, len + imgdata.length );
        GLib.Memory.copy( &data[ len ], imgdata, imgdata.length );
        len += imgdata.length;
        return Cairo.Status.SUCCESS;
      } );

      this.data = new uint8[ len ];
      GLib.Memory.copy( &this.data[ 0 ], data, len );

      GLib.free( data );
    }
  }
}
