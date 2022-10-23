﻿NDSummary.OnToolTipsLoaded("File:◀️_ssGUI_API_▶️/DataClasses/Renderer.hpp",{529:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype529\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">ssGUI</div></div></div></div>",530:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype530\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;Renderer</div></div></div><div class=\"TTSummary\">This manages all the rendering information for a GUI Object</div></div>",532:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype532\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetEnabled(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">enabled</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if this GUI Object is enabled. Note that the logic of this GUI Object will not be called if not enabled.</div></div>",533:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype533\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsEnabled() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if this GUI Object is enabled. Note that the logic of this GUI Object will not be called if not enabled.&nbsp; Also Note that this can be affected by parent\'s enable status.</div></div>",534:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype534\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsSelfEnabled() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if itself is enabled excluding the enable status of its parent.&nbsp; Meaning even if the parent with visibility is false, it will return true if this GUI Object\'s enable is not set to false.</div></div>",535:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype535\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetBackgroundColor(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">u8vec4&nbsp;</td><td class=\"PName last\">color</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the background color of this GUI Object</div></div>",536:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype536\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::u8vec4 GetBackgroundColor() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the background color of this GUI Object</div></div>",537:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype537\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;glm::vec2&gt;&amp; Extension_GetDrawingVertices()</div></div><div class=\"TTSummary\">This returns a list of vertices for all the shapes that will be drawn by this GUI object.&nbsp; A shape is formed by having the vertices drawn in clockwise direction. Undefined behaviour if they are listed in anti-clockwise direction.&nbsp; This function is mainly be called by &lt;Extension::Internal_Draw&gt;.</div></div>",538:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype538\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;glm::vec2&gt;&amp; Extension_GetDrawingUVs()</div></div><div class=\"TTSummary\">This returns the UVs that are mapped to each vertex at the same index location.&nbsp; If no texture is used, you should still maintain the number of UVs same as the number of vertices.&nbsp; This function is mainly be called by &lt;Extension::Internal_Draw&gt;.</div></div>",539:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype539\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;glm::u8vec4&gt;&amp; Extension_GetDrawingColours()</div></div><div class=\"TTSummary\">This returns the colors that are mapped to each vertex at the same index location.&nbsp; If texture is used, this essentially affects the tint of the image, with white as no tinting at all.&nbsp; This function is mainly be called by &lt;Extension::Internal_Draw&gt;.</div></div>",540:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype540\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;<span class=\"SHKeyword\">int</span>&gt;&amp; Extension_GetDrawingCounts()</div></div><div class=\"TTSummary\">This returns the number of vertices each shape has, with the order same as vertices and others.&nbsp; So for example if the first value is 4, then the first 4 vertices form a shape. So on and so forth...&nbsp; This function is mainly be called by &lt;Extension::Internal_Draw&gt;.</div></div>",541:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype541\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::vector&lt;ssGUI::DrawingProperty&gt;&amp; Extension_GetDrawingProperties()</div></div><div class=\"TTSummary\">This returns the property of each shape, mapped to the same index location as Extension_GetDrawingCounts.&nbsp; This indicates if the shape is just a colored shape, an image or font.&nbsp; This function is mainly be called by &lt;Extension::Internal_Draw&gt;.</div></div>",542:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype542\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> Extension_GetGUIObjectFirstShapeIndex() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">This returns the index of first shape created by the GUI Object.&nbsp; So for example,</div></div>",543:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype543\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> Extension_GetGUIObjectFirstVertexIndex() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">This returns the index of first vertex created by the GUI Object Again similar to Extension_GetGUIObjectFirstShapeIndex but with indices instead</div></div>",544:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype544\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> RedrawObject()</div></div><div class=\"TTSummary\">Forces the GUI Object to be redrawn. <b>Do not</b> call this in Internal_Draw.</div></div>",545:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype545\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsRedrawNeeded() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if the GUI Object needs to be redrawn</div></div>"});