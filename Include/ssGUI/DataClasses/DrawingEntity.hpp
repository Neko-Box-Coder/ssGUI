#ifndef SSGUI_DRAWING_ENTITY_HPP
#define SSGUI_DRAWING_ENTITY_HPP

#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include "ssGUI/HelperClasses/OutputStreamUtil.hpp"

#include "glm/vec2.hpp"

#include <vector>


//namespace: ssGUI
namespace ssGUI
{
    //struct: ssGUI::DrawingEntity
    //Structure used to pass to BackendDrawing to draw entities
    struct DrawingEntity
    {
        public:
            //var: EntityName
            std::string EntityName;
        
            //var: Vertices
            std::vector<glm::vec2> Vertices;
            
            //var: TexCoords
            std::vector<glm::vec2> TexCoords;
            
            //var: Colors
            std::vector<glm::u8vec4> Colors;

            //var: Character
            uint32_t Character = 0;

            //var: fontP
            Backend::FontInterface* BackendFont = nullptr;
            
            //var: characterSize
            int CharacterSize = -1;

            //var: imageP
            Backend::ImageInterface* BackendImage = nullptr;
            
            inline bool operator== (const DrawingEntity& other)
            {
                return  EntityName == other.EntityName &&
                        Vertices == other.Vertices && 
                        TexCoords == other.TexCoords && 
                        Colors == other.Colors && 
                        Character == other.Character &&
                        BackendFont == other.BackendFont &&
                        CharacterSize == other.CharacterSize &&
                        BackendImage == other.BackendImage;
            }
            
            inline bool operator!= (const DrawingEntity& other)
            {
                return !operator==(other);
            }
            
            inline friend std::ostream& operator<< (std::ostream& stream, const DrawingEntity& other)
            {
                stream  << SSGUI_OUTPUT_CLASS_NAME(DrawingEntity)
                        << SSGUI_OUTPUT_VAR(EntityName)
                        << SSGUI_OUTPUT_VAR(Vertices)
                        << SSGUI_OUTPUT_VAR(TexCoords)
                        << SSGUI_OUTPUT_VAR(Colors)
                        << SSGUI_OUTPUT_VAR(Character)
                        << SSGUI_OUTPUT_VAR(BackendFont)
                        << SSGUI_OUTPUT_VAR(CharacterSize)
                        << SSGUI_OUTPUT_LAST_VAR(BackendImage);
                        
                return stream;
            }
    };
}

#endif