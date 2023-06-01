#ifndef SSGUI_BACKEND_DRAWING_MOCK_H
#define SSGUI_BACKEND_DRAWING_MOCK_H

//#define SSGUI_MOCK_ENABLE_LOG
#include "ssGUI/Backend/Mocks/MockMacro.hpp"

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include <unordered_set>

namespace ssGUI
{

//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendDrawingMock
    class BackendDrawingMock : public BackendDrawingInterface
    {
        private:
            ssGUI::Backend::BackendDrawingInterface* UnderlyingInterface;
            int SavedStateCount;
            int CurrentDrawingBuffer;
            
            std::vector<glm::vec2> Vertices[2];
            std::vector<glm::vec2> TexCoords[2];
            std::vector<glm::u8vec4> Colors[2];
            std::vector<int> Counts[2];
            std::vector<ssGUI::DrawingProperty> Properties[2];
            glm::u8vec3 ClearedColor[2];
            std::unordered_set<ssGUI::Backend::BackendImageInterface*> CachedImage;

            BackendDrawingMock& operator=(BackendDrawingMock const& other);

        protected:
            BackendDrawingMock(BackendDrawingMock const& other);
        
        public:
            BackendDrawingMock(ssGUI::Backend::BackendDrawingInterface* drawingInterface);
            ~BackendDrawingMock() override;
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(ssGUI::Backend::BackendDrawingInterface*, UnderlyingInterface)
            
            SSGUI_MOCK_DECLARE_VARIABLE_GETTER(std::unordered_set<ssGUI::Backend::BackendImageInterface*>, CachedImage);
            
            //function: GetStateCounter
            int GetStateCounter() const;
            
            //function: GetDrawnVertices
            const std::vector<glm::vec2>& GetDrawnVertices() const;
            
            //function: GetRenderedVertices
            const std::vector<glm::vec2>& GetRenderedVertices() const;

            //function: GetDrawnTexCoords
            const std::vector<glm::vec2>& GetDrawnTexCoords() const;
            
            //function: GetRenderedTexCoords
            const std::vector<glm::vec2>& GetRenderedTexCoords() const;

            //function: GetDrawnColors
            const std::vector<glm::u8vec4>& GetDrawnColors() const;
            
            //function: GetRenderedColors
            const std::vector<glm::u8vec4>& GetRenderedColors() const;

            //function: GetDrawnCounts
            const std::vector<int>& GetDrawnCounts() const;
            
            //function: GetRenderedCounts
            const std::vector<int>& GetRenderedCounts() const;

            //function: GetDrawnProperties
            const std::vector<ssGUI::DrawingProperty>& GetDrawnProperties() const;
            
            //function: GetRenderedProperties
            const std::vector<ssGUI::DrawingProperty>& GetRenderedProperties() const;

            //function: GetDrawnClearedColor
            const glm::u8vec3& GetDrawnClearedColor() const;
            
            //function: GetRenderedClearedColor
            const glm::u8vec3& GetRenderedClearedColor() const;
            
            //function: SaveState
            //See <BackendDrawingInterface::SaveState>
            void SaveState() override;

            //function: RestoreState
            //See <BackendDrawingInterface::RestoreState>
            void RestoreState() override;

            //function: DrawEntities
            //See <BackendDrawingInterface::DrawEntities>
            bool DrawEntities(  const std::vector<glm::vec2>& vertices, 
                                const std::vector<glm::vec2>& texCoords,
                                const std::vector<glm::u8vec4>& colors,
                                const std::vector<int>& counts,
                                const std::vector<ssGUI::DrawingProperty>& properties) override;

            //function: Render
            //See <BackendDrawingInterface::Render>
            void Render(glm::u8vec3 clearColor) override;

            //function: ClearBackBuffer
            //See <BackendDrawingInterface::ClearBackBuffer>
            void ClearBackBuffer(glm::u8vec3 clearColor) override;

            //function: AddImageCache
            //See <BackendDrawingInterface::AddImageCache>
            void AddImageCache(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
            //function: RemoveImageCache
            //See <BackendDrawingInterface::RemoveImageCache>
            void RemoveImageCache(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
            //function: GetRawImageCacheHandle
            //See <BackendDrawingInterface::GetRawImageCacheHandle>
            void* GetRawImageCacheHandle(ssGUI::Backend::BackendImageInterface* backendImage) override;
            
        //NOTE: These functions are not used in mock
        protected:
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const ssGUI::Backend::BackendImageInterface& image) override;


            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors) override;

            //NOTE: End index is exclusive
            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            const uint32_t character,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendFontInterface& font,
                            int characterSize) override;

            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::vec2>& texCoords,
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex,
                            const ssGUI::Backend::BackendImageInterface& image) override;


            bool DrawShape( const std::vector<glm::vec2>& vertices, 
                            const std::vector<glm::u8vec4>& colors,
                            int startIndex, int endIndex) override;

    };
}

}

#endif