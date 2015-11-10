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
    public Word[]? words = null;

    /**
     * An array containing all images of the page.
     */
    public ForImage[]? images = null;

    /**
     * An array containing all paths of the page.
     */
    public ForPath[]? paths = null;

    /**
     * Creates a new PDFPage from a Poppler.Page.
     * @param page The Poppler.Page.
     */
    public PDFPage.from_poppler_page( Page page )
    {
      this.page = page;

      this.page.get_size( out this.width, out this.height );

      DMPoppler.get_words( this.page, out this.words );

      DMPoppler.get_elements( this.page, out this.paths, out this.images );
    }

    /**
     * Sorts all words, images and paths of this page and stores them into a PDFObject array.
     * @return A sorted PDFObject[] containing all words, images and paths of this page.
     */
    public PDFObject[] get_sorted_elements( )
    {
      uint32 pdf_objects_length = 0;

      unowned Word word = { };
      uint32 word_index = 0;
      bool word_empty = true;

      unowned ForImage image = { };
      uint32 image_index = 0;
      bool image_empty = true;

      unowned ForPath path = { };
      uint32 path_index = 0;
      bool path_empty = true;

      if ( this.words != null && this.words.length > 0 )
      {
        word = this.words[ 0 ];
        pdf_objects_length += this.words.length;
        word_empty = false;
      }

      if ( this.images != null && this.images.length > 0 )
      {
        image = this.images[ 0 ];
        pdf_objects_length += this.images.length;
        image_empty = false;
      }

      if ( this.paths != null && this.paths.length > 0 )
      {
        path = this.paths[ 0 ];
        pdf_objects_length += this.paths.length;
        path_empty = false;
      }

      PDFObject[] pdf_objects = new PDFObject[ pdf_objects_length ];
      for ( uint32 i = 0; i < pdf_objects_length; i ++ )
      {
        if ( !word_empty &&
             ( image_empty || word.char_pos <= image.char_pos ) &&
             ( path_empty || word.char_pos <= path.char_pos )
           )
        {
          pdf_objects[ i ] = new PDFObject.from_word( word );
          word_index ++;
          if ( word_index >= this.words.length )
          {
            word_empty = true;
          }
          else
          {
            word = this.words[ word_index ];
          }
        }
        else if ( !image_empty &&
                  ( path_empty || ( image.char_pos < path.char_pos || ( image.char_pos == path.char_pos && image.object_pos <= path.object_pos ) ) )
                )
        {
          pdf_objects[ i ] = new PDFObject.from_image( image );
          image_index ++;
          if ( image_index >= this.images.length )
          {
            image_empty = true;
          }
          else
          {
            image = this.images[ image_index ];
          }
        }
        else if ( !path_empty )
        {
          PDFObject po = new PDFObject.from_path( path );
          pdf_objects[ i ] = po;
          path_index ++;
          if ( path_index >= this.paths.length )
          {
            path_empty = true;
          }
          else
          {
            path = this.paths[ path_index ];
          }
        }
        else
        {
          /* Darf nicht passieren */
          break;
        }
      }

      return pdf_objects;
    }

    /**
     * Loads the image data for the given PDFImage.
     * @param image The image whose data will be loaded.
     * @return The loaded image data.
     */
    public uint8[] load_image_data( ForImage image )
    {
      Cairo.Surface surface = this.page.get_image( image.id );
      void* data = null;
      uint len = 0;

      surface.write_to_png_stream( ( imgdata ) =>
      {
        data = GLib.realloc( data, len + imgdata.length );
        GLib.Memory.copy( &data[ len ], imgdata, imgdata.length );
        len += imgdata.length;
        return Cairo.Status.SUCCESS;
      } );

      uint8[] img_data = new uint8[ len ];
      GLib.Memory.copy( &img_data[ 0 ], data, len );

      GLib.free( data );
      return img_data;
    }
  }

  /**
   * This enum contains the possible PDFObject types.
   */
  public enum PDFObjectType
  {
    WORD,
    IMAGE,
    PATH;
  }

  /**
   * This class represents a Object in the PDF file.
   * It is used to store and sort different types of PDF objects in the same array.
   */
  public class PDFObject : GLib.Object
  {
    /**
     * The type of this PDFObject.
     */
    public PDFObjectType type;

    /**
     * If PDFObject.type is PDFObjectType.WORD, this Word will contain the actual word object.
     */
    public unowned Word word;

    /**
     * If PDFObject.type is PDFObjectType.IMAGE, this ForImage will contain the actual image object.
     */
    public unowned ForImage image;

    /**
     * If PDFObject.type is PDFObjectType.PATH, this ForPath will contain the actual path object.
     */
    public unowned ForPath path;

    /**
     * Creates a new PDFObject with the type PDFObjectType.WORD and the given word as content.
     * @param word The word object.
     */
    public PDFObject.from_word( Word word )
    {
      this.word = word;
      this.type = PDFObjectType.WORD;
    }

    /**
     * Creates a new PDFObject with the type PDFObjectType.IMAGE and the given image as content.
     * @param image The image object.
     */
    public PDFObject.from_image( ForImage image )
    {
      this.image = image;
      this.type = PDFObjectType.IMAGE;
    }

    /**
     * Creates a new PDFObject with the type PDFObjectType.PATH and the given path as content.
     * @param path The path object.
     */
    public PDFObject.from_path( ForPath path )
    {
      this.path = path;
      this.type = PDFObjectType.PATH;
    }
  }
}
