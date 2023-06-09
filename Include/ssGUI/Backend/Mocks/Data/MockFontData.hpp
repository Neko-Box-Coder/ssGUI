#ifndef SSGUI_MOCK_FONT_DATA_H
#define SSGUI_MOCK_FONT_DATA_H

#include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include <unordered_map>
#include <unordered_set>
#include <utility>

struct hash_pair final 
{
    template<class TFirst, class TSecond>
    size_t operator()(const std::pair<TFirst, TSecond>& p) const noexcept 
    {
        uintmax_t hash = std::hash<TFirst>{}(p.first);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<TSecond>{}(p.second);
        return std::hash<uintmax_t>{}(hash);
    }
};

namespace ssGUI
{
    inline void SetMockBackendFontData(ssGUI::Backend::BackendFontMock& mockFont)
    {
        mockFont.OverrideReturns(IsValid())
                .Returns(true);
        
        auto setRepeatedRenderInfoAttributes = [](ssGUI::CharacterRenderInfo& renderInfo)
        {
            renderInfo.BaselinePosition = glm::vec2(0, 0);\
            renderInfo.CharacterAtNewline = 0;\
            renderInfo.Rendered = 1;\
            renderInfo.Valid = 1;;\
            renderInfo.LineMinY = 0;\
            renderInfo.LineMaxY = 0;\
            renderInfo.RenderFontSize = 15;\
            renderInfo.TargetSizeMultiplier = 1;
        };
        
        std::unordered_map<std::pair<wchar_t, float>, ssGUI::CharacterRenderInfo, hash_pair> renderInfos;
        renderInfos[{static_cast<wchar_t>(84), 15.f}].Advance = 10;
        renderInfos[{static_cast<wchar_t>(84), 15.f}].DrawOffset = glm::ivec2(1, -10);
        renderInfos[{static_cast<wchar_t>(84), 15.f}].Size = glm::vec2(8, 10);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(84), 15.f}]);
        
        renderInfos[{static_cast<wchar_t>(104), 15.f}].Advance = 9;
        renderInfos[{static_cast<wchar_t>(104), 15.f}].DrawOffset = glm::ivec2(1, -11);
        renderInfos[{static_cast<wchar_t>(104), 15.f}].Size = glm::vec2(8, 11);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(104), 15.f}]);
        
        renderInfos[{static_cast<wchar_t>(105), 15.f}].Advance = 3;
        renderInfos[{static_cast<wchar_t>(105), 15.f}].DrawOffset = glm::ivec2(0, -11);
        renderInfos[{static_cast<wchar_t>(105), 15.f}].Size = glm::vec2(3, 11);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(105), 15.f}]);

        renderInfos[{static_cast<wchar_t>(115), 15.f}].Advance = 7;
        renderInfos[{static_cast<wchar_t>(115), 15.f}].DrawOffset = glm::ivec2(0, -8);
        renderInfos[{static_cast<wchar_t>(115), 15.f}].Size = glm::vec2(7, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(115), 15.f}]);

        renderInfos[{static_cast<wchar_t>(32), 15.f}].Advance = 4;
        renderInfos[{static_cast<wchar_t>(32), 15.f}].DrawOffset = glm::ivec2(0, 0);
        renderInfos[{static_cast<wchar_t>(32), 15.f}].Size = glm::vec2(0, 0);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(32), 15.f}]);

        renderInfos[{static_cast<wchar_t>(105), 15.f}].Advance = 3;
        renderInfos[{static_cast<wchar_t>(105), 15.f}].DrawOffset = glm::ivec2(0, -11);
        renderInfos[{static_cast<wchar_t>(105), 15.f}].Size = glm::vec2(3, 11);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(105), 15.f}]);

        renderInfos[{static_cast<wchar_t>(111), 15.f}].Advance = 9;
        renderInfos[{static_cast<wchar_t>(111), 15.f}].DrawOffset = glm::ivec2(0, -8);
        renderInfos[{static_cast<wchar_t>(111), 15.f}].Size = glm::vec2(9, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(111), 15.f}]);

        renderInfos[{static_cast<wchar_t>(109), 15.f}].Advance = 14;
        renderInfos[{static_cast<wchar_t>(109), 15.f}].DrawOffset = glm::ivec2(1, -8);
        renderInfos[{static_cast<wchar_t>(109), 15.f}].Size = glm::vec2(13, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(109), 15.f}]);

        renderInfos[{static_cast<wchar_t>(101), 15.f}].Advance = 8;
        renderInfos[{static_cast<wchar_t>(101), 15.f}].DrawOffset = glm::ivec2(0, -8);
        renderInfos[{static_cast<wchar_t>(101), 15.f}].Size = glm::vec2(8, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(101), 15.f}]);

        renderInfos[{static_cast<wchar_t>(114), 15.f}].Advance = 7;
        renderInfos[{static_cast<wchar_t>(114), 15.f}].DrawOffset = glm::ivec2(1, -8);
        renderInfos[{static_cast<wchar_t>(114), 15.f}].Size = glm::vec2(6, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(114), 15.f}]);

        renderInfos[{static_cast<wchar_t>(97), 15.f}].Advance = 8;
        renderInfos[{static_cast<wchar_t>(97), 15.f}].DrawOffset = glm::ivec2(0, -8);
        renderInfos[{static_cast<wchar_t>(97), 15.f}].Size = glm::vec2(8, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(97), 15.f}]);

        renderInfos[{static_cast<wchar_t>(110), 15.f}].Advance = 9;
        renderInfos[{static_cast<wchar_t>(110), 15.f}].DrawOffset = glm::ivec2(1, -8);
        renderInfos[{static_cast<wchar_t>(110), 15.f}].Size = glm::vec2(8, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(110), 15.f}]);

        renderInfos[{static_cast<wchar_t>(100), 15.f}].Advance = 9;
        renderInfos[{static_cast<wchar_t>(100), 15.f}].DrawOffset = glm::ivec2(0, -11);
        renderInfos[{static_cast<wchar_t>(100), 15.f}].Size = glm::vec2(9, 11);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(100), 15.f}]);

        renderInfos[{static_cast<wchar_t>(116), 15.f}].Advance = 6;
        renderInfos[{static_cast<wchar_t>(116), 15.f}].DrawOffset = glm::ivec2(0, -10);
        renderInfos[{static_cast<wchar_t>(116), 15.f}].Size = glm::vec2(6, 10);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(116), 15.f}]);

        renderInfos[{static_cast<wchar_t>(120), 15.f}].Advance = 8;
        renderInfos[{static_cast<wchar_t>(120), 15.f}].DrawOffset = glm::ivec2(0, -8);
        renderInfos[{static_cast<wchar_t>(120), 15.f}].Size = glm::vec2(8, 8);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(120), 15.f}]);

        renderInfos[{static_cast<wchar_t>(102), 15.f}].Advance = 6;
        renderInfos[{static_cast<wchar_t>(102), 15.f}].DrawOffset = glm::ivec2(0, -11);
        renderInfos[{static_cast<wchar_t>(102), 15.f}].Size = glm::vec2(7, 11);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(102), 15.f}]);

        renderInfos[{static_cast<wchar_t>(103), 15.f}].Advance = 9;
        renderInfos[{static_cast<wchar_t>(103), 15.f}].DrawOffset = glm::ivec2(0, -8);
        renderInfos[{static_cast<wchar_t>(103), 15.f}].Size = glm::vec2(9, 12);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(103), 15.f}]);

        renderInfos[{static_cast<wchar_t>(10), 15.f}].Advance = 9;
        renderInfos[{static_cast<wchar_t>(10), 15.f}].DrawOffset = glm::ivec2(1, -11);
        renderInfos[{static_cast<wchar_t>(10), 15.f}].Size = glm::vec2(7, 11);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(10), 15.f}]);

        renderInfos[{static_cast<wchar_t>(32), 15.f}].Advance = 4;
        renderInfos[{static_cast<wchar_t>(32), 15.f}].DrawOffset = glm::ivec2(0, -0);
        renderInfos[{static_cast<wchar_t>(32), 15.f}].Size = glm::vec2(0, 0);
        setRepeatedRenderInfoAttributes(renderInfos[{static_cast<wchar_t>(32), 15.f}]);

        mockFont.OverrideReturns(GetCharacterRenderInfo(wchar_t, float))
                .ReturnsByAction<ssGUI::CharacterRenderInfo>
                (
                    [renderInfos](std::vector<void*>& args, void* out)
                    {
                        (*static_cast<ssGUI::CharacterRenderInfo*>(out)) = renderInfos.at({*static_cast<wchar_t*>(args[0]), *static_cast<float*>(args[1])});
                    }
                )
                .If
                (
                    [renderInfos](std::vector<void*>& args) -> bool
                    {
                        return  args.size() == 2 && 
                                renderInfos.find({*static_cast<wchar_t*>(args[0]), *static_cast<float*>(args[1])}) != renderInfos.end();
                    }
                )
                .Otherwise_Do
                (
                    [](...)
                    {
                        assert(false);
                    }
                );
        
        std::unordered_set<wchar_t> supportedChars;
        supportedChars.insert(static_cast<wchar_t>(84));
        supportedChars.insert(static_cast<wchar_t>(104));
        supportedChars.insert(static_cast<wchar_t>(105));
        supportedChars.insert(static_cast<wchar_t>(115));
        supportedChars.insert(static_cast<wchar_t>(111));
        supportedChars.insert(static_cast<wchar_t>(109));
        supportedChars.insert(static_cast<wchar_t>(101));
        supportedChars.insert(static_cast<wchar_t>(114));
        supportedChars.insert(static_cast<wchar_t>(97));
        supportedChars.insert(static_cast<wchar_t>(110));
        supportedChars.insert(static_cast<wchar_t>(100));
        supportedChars.insert(static_cast<wchar_t>(116));
        supportedChars.insert(static_cast<wchar_t>(120));
        supportedChars.insert(static_cast<wchar_t>(102));
        supportedChars.insert(static_cast<wchar_t>(103));
        
        mockFont.OverrideReturns(IsCharacterSupported(wchar_t))
                .ReturnsByAction<bool>
                (
                    [supportedChars](std::vector<void *>& args, void* out)
                    {
                        if(args.size() != 1)
                        {
                            assert(false);
                            return false;
                        }
                        
                        return supportedChars.find(*static_cast<wchar_t*>(args[0])) != supportedChars.end();
                    }
                );

        //NOTE: For now, GetKerning always returns 0 on native freetype until we can enable harfbuzz and fix it maybe
        mockFont.OverrideReturns(GetKerning(wchar_t, wchar_t, float))
                .Returns(0.f);
        
        auto assertFalse = [](...) { assert(false); };
        
        mockFont.OverrideReturns(GetLineSpacing(float))
                .Returns(20.4375f)
                .WhenCalledWith(15.f)
                .Otherwise_Do(assertFalse);
        
        mockFont.OverrideReturns(GetUnderlineOffset(float))
                .Returns(0.f)
                .WhenCalledExpectedly_Do(assertFalse);
        
        mockFont.OverrideReturns(GetUnderlineThickness(float))
                .Returns(0.f)
                .WhenCalledExpectedly_Do(assertFalse);
        
        mockFont.OverrideReturns(LoadFromPath(std::string))
                .Returns(false)
                .WhenCalledExpectedly_Do(assertFalse);
                
        mockFont.OverrideReturns(LoadFromMemory(void*, int))
                .Returns(true);
        
        mockFont.OverrideReturns(GetFixedAvailableFontSizes(std::vector<float>&))
                .Returns(false)
                .WhenCalledExpectedly_Do(assertFalse);
        
        mockFont.OverrideReturns(GetCharacterImage(wchar_t, float, ssGUI::ImageData&))
                .Returns(false)
                .WhenCalledExpectedly_Do(assertFalse);
        
        mockFont.OverrideReturns(GetRawHandle())
                .Returns(nullptr)
                .WhenCalledExpectedly_Do(assertFalse);
    }
}

#endif