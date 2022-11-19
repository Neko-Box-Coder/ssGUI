#include "ft2build.h" 
#include FT_FREETYPE_H

#include <vector>
#include <iostream>
#include <fstream>

int main()
{
    FT_Library library;

    FT_Error error = FT_Init_FreeType( &library );
    if(error)
    {
        std::cout << "an error occurred during library initialization ...\n";
        return -1;
    }

    //To see number of faces from a given file do:
    FT_Face face;
    // std::string fontPath = "/usr/share/fonts/noto/NotoColorEmoji.ttf";
    // std::string fontPath = "/usr/share/fonts/noto/NotoSans-Thin.ttf";
    std::string fontPath = "./NotoSans-Regular.ttf";

    std::cout << "Loading :" << fontPath << "\n";

    //Get number of faces
    error = FT_New_Face
            ( 
                library,
                fontPath.c_str(),
                -1,
                &face 
            );

    if(error)
    {
        //Do error process...
        std::cout << "Failed to read font file ...\n";
        return -1;
    }

    std::cout<<"face->num_faces: "<<face->num_faces<<"\n";

    //Load the face
    error = FT_New_Face
            ( 
                library,
                fontPath.c_str(),
                0,
                &face 
            );

    if(error)
    {
        //Do error process...
        std::cout << "Failed to read font file ...\n";
        return -1;
    }

    /*//Load font from memory
    FT_Byte* buffer = nullptr;
    FT_Long bufferSize = 0;

    error = FT_New_Memory_Face
            (
                library,
                buffer,             //first byte in memory
                bufferSize,         //size in bytes
                0,                  //face_index
                &face
            );

    if(error)
    {
        //Do error process...
    }*/

    /*
    //Set character size
    //1 Point = 1/72 inch
    error = FT_Set_Char_Size
            (
                face,           // handle to face object
                0,              // char_width in 1/64th of points
                                // 0 means same as char_height
                64 * 14,        // char_height in 1/64th of points, this case is 14 pixel high
                72,             // horizontal device resolution
                0               // vertical device resolution, same as horizontal
            );
    */

    std::vector<int> fixWidths;
    std::vector<int> fixHeights;
    // FT_IS_SCALABLE(face);
    if(FT_HAS_FIXED_SIZES(face))
    {
        for(int i = 0; i < face->num_fixed_sizes; i++)
        {
            fixWidths.push_back(face->available_sizes[i].width);
            fixHeights.push_back(face->available_sizes[i].height);
            std::cout << "width[" << i << "]: " << fixWidths.back() << "\n";
            std::cout << "height[" << i << "]: " << fixHeights.back() << "\n";
        }
        error = FT_Select_Size(face, 0);
    } 
    else
    {
        //Or.. you can just do this
        error = FT_Set_Pixel_Sizes
                (
                    face,   // handle to face object
                    0,      // pixel_width
                    // 109      // pixel_height
                    40      // pixel_height
                );
    }

    if(error)
    {
        std::cout << "Failed to Set pixel size: " << error << "\n";
        return -1;
    }

    // FT_ULong charcode = 9996;
    FT_ULong charcode = 65;
    //0 = missing glyph, which is a box/space
    FT_UInt glyph_index = FT_Get_Char_Index( face, charcode );

    FT_Int32 flags =    FT_LOAD_FORCE_AUTOHINT | 
                        FT_LOAD_TARGET_NORMAL;
    if(FT_HAS_COLOR(face))
        flags |= FT_LOAD_COLOR;
    else
        flags |= FT_LOAD_RENDER;

    //In order to get the glyph image, it needs to be in a glyph slot
    error = FT_Load_Glyph
            (
                face,           /* handle to face object */
                glyph_index,    /* glyph index           */
                flags           /* load flags */
            );

    if(error)
    {
        std::cout << "Failed to load glyph\n";
        return -1;
    }

    //Convert to bitmap if there isn't one
    if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
    {
        //This uses RGB instead of grayscale
        error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    }

    if(error)
    {
        std::cout << "Failed to render glyph\n";
        return -1;
    }

    if(FT_HAS_COLOR(face) && face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_BGRA)
    {
        // face->glyph->bitmap.palette_mode
        std::cout << "color font loaded\n";
    }
    else if(!FT_HAS_COLOR(face) && face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
    {
        int numOfGrays = face->glyph->bitmap.num_grays;
        std::cout << "mono font loaded\n";
        std::cout << "numOfGrays: " << numOfGrays << "\n";
    }
    else
    {
        std::cout << "Invalid pixel mode\n";
        return -1;
    }

    std::ofstream outputStream("./output.fontImage", std::ios::out | std::ios::binary);

    if(!outputStream)
    {
        std::cout << "Failed to write file\n";
        return -1;
    }

    int width = face->glyph->bitmap.width;
    int height = face->glyph->bitmap.rows;
    int pitch = face->glyph->bitmap.pitch;
    unsigned char* buf = face->glyph->bitmap.buffer;

    outputStream.write(reinterpret_cast<char*>(buf), face->glyph->bitmap.pitch * face->glyph->bitmap.rows);
    
    std::cout<<"face->glyph->bitmap.width: "<<face->glyph->bitmap.width<<"\n";
    std::cout<<"face->glyph->bitmap.rows: "<<face->glyph->bitmap.rows<<"\n";


    if(!outputStream.good())
    {
        std::cout << "Failed to output\n";
        return -1;
    }

    //For other encoding that is not unicode, this function can be used
    // error = FT_Select_Charmap(
    //       face,               /* target face object */
    //       FT_ENCODING_BIG5 ); /* encoding           */


    //To find the encoding needed, this code can be used
    // FT_CharMap  found = 0;
    // FT_CharMap  charmap;
    // int         n;


    // for ( n = 0; n < face->num_charmaps; n++ )
    // {
    // charmap = face->charmaps[n];
    // if ( charmap->platform_id == my_platform_id &&
    //     charmap->encoding_id == my_encoding_id )
    // {
    //     found = charmap;
    //     break;
    // }
    // }

    // if ( !found ) { ... }

    // /* now, select the charmap for the face object */
    // error = FT_Set_Charmap( face, found );
    // if ( error ) { ... }



    /*
    //Rendering characters..

    FT_GlyphSlot  slot = face->glyph;  // a small shortcut
    int           pen_x, pen_y, n;


    // ... initialize library ...
    // ... create face object ...
    // ... set character size ...
    size_t num_chars = 20;
    std::string text = "this is just some text";

    pen_x = 300;
    pen_y = 200;


    for (size_t n = 0; n < num_chars; n++ )
    {
        FT_UInt  glyph_index;


        //retrieve glyph index from character code
        glyph_index = FT_Get_Char_Index( face, text[n] );

        //load glyph image into the slot (erase previous one)
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        if ( error )
            continue;  //ignore errors

        //convert to an anti-aliased bitmap
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        if ( error )
            continue;

        //now, draw to our target surface
        my_draw_bitmap( &slot->bitmap,
                        pen_x + slot->bitmap_left,
                        pen_y - slot->bitmap_top );

        //increment pen position
        pen_x += slot->advance.x >> 6;
        pen_y += slot->advance.y >> 6; //not useful for now
    }
    */

    return 0;
}