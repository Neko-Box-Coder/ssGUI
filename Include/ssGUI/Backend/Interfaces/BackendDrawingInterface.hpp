#ifndef SSGUI_BACKEND_DRAWING_INTERFACE
#define SSGUI_BACKEND_DRAWING_INTERFACE

#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/DataClasses/DrawingProperties.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <string>
#include <vector>

#include <iostream>

//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: ssGUI::Backend::BackendDrawingInterface
    class BackendDrawingInterface
    {
        private:
            // BackendDrawingInterface(const BackendDrawingInterface&);
            // BackendDrawingInterface& operator=(const BackendDrawingInterface&);
        
        public:
            BackendDrawingInterface(){}
            virtual ~BackendDrawingInterface() = 0;

            //function: DrawEntities
            //Draws the entity based on what is set in the _properties_. Returns true if drawn successfully. *Note that if you are not using <ssGUIManager>, you need to call <Render> at the end in order to render it*.
            virtual bool DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                        const std::vector<glm::vec2>& texCoords,
                                        const std::vector<glm::u8vec4>& colors,
                                        const std::vector<int>& counts,
                                        const std::vector<ssGUI::DrawingProperty>& properties) = 0;

            /*function: Render
            Renders every entity that are drawn to the <MainWindow>. This will automatically clear the back buffer. 
            If you are using <ssGUIManager>, this will be automatically called.*/
            virtual void Render(glm::u8vec3 clearColor) = 0;

            /*function: ClearBackBuffer
            Clears the back buffer manually. If you are using <ssGUIManager>, this will be automatically called for caching.*/
            virtual void ClearBackBuffer(glm::u8vec3 clearColor) = 0;
            
        protected:
            virtual bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::vec2>& texCoords,
                                    const std::vector<glm::u8vec4>& colors,
                                    const ssGUI::Backend::BackendFontInterface& font,
                                    int CharacterSize) = 0;

            virtual bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::vec2>& texCoords,
                                    const std::vector<glm::u8vec4>& colors,
                                    const ssGUI::Backend::BackendImageInterface& image) = 0;


            virtual bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::u8vec4>& colors) = 0;

            //NOTE: End index is exclusive
            virtual bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::vec2>& texCoords,
                                    const std::vector<glm::u8vec4>& colors,
                                    int startIndex, int endIndex,
                                    const ssGUI::Backend::BackendFontInterface& font,
                                    int CharacterSize) = 0;

            virtual bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::vec2>& texCoords,
                                    const std::vector<glm::u8vec4>& colors,
                                    int startIndex, int endIndex,
                                    const ssGUI::Backend::BackendImageInterface& image) = 0;


            virtual bool DrawShape( const std::vector<glm::vec2>& vertices, 
                                    const std::vector<glm::u8vec4>& colors,
                                    int startIndex, int endIndex) = 0;

    };
    inline BackendDrawingInterface::~BackendDrawingInterface(){}   //Pure virtual destructor needs to be defined
}

#endif