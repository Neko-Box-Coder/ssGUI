﻿NDSummary.OnToolTipsLoaded("File:◀️_ssGUI_API_▶️/GUIObjectClasses/CompositeClasses/ImageCanvas.hpp",{1023:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1023\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">ssGUI</div></div></div></div>",1024:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1024\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;ImageCanvas</div></div></div><div class=\"TTSummary\">This class provides panning, rotating and zooming functionality to view the image.&nbsp; It also provides position conversion between global and UV spaces.</div></div>",1026:"<div class=\"NDToolTip TVariable LC\"><div id=\"NDPrototype1026\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">static const</span> std::<span class=\"SHKeyword\">string</span> ListenerKey</div></div></div>",1028:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1028\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetUVFromGlobalPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">globalPos</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Gets the UV position (0 to image size) from global position</div></div>",1029:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1029\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetGlobalPositionFromUV(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">uv</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Gets the global position from the UV position (0 to image size)</div></div>",1030:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1030\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetImageRotation(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName\">rotation,</td><td></td><td class=\"last\"></td></tr><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">radians</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the rotation of the image, by default it uses degrees</div></div>",1031:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1031\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual float</span> GetImageRotation(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName\">radians</td><td class=\"PDefaultValueSeparator\">&nbsp;=&nbsp;</td><td class=\"PDefaultValue last\"><span class=\"SHKeyword\">false</span></td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span></td></tr></table></div></div><div class=\"TTSummary\">Gets the rotation fo the image, by default it uses degrees</div></div>",1032:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1032\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetImageScale(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">scale</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the scale of the image in percentage (0 to 1), 1 meaning the height/width (determined by IsMeasureScaleByHeight) of the image is the same size of ImageCanvas GUI Object.</div></div>",1033:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1033\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetImageScale() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the scale of the image in percentage (0 to 1), 1 meaning the height/width (determined by IsMeasureScaleByHeight) of the image is the same size of ImageCanvas.</div></div>",1034:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1034\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetOriginalImageScale() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the scale to set in order to have the original size of the image</div></div>",1035:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1035\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsMeasureScaleByHeight() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if the scaling is measured using height of ImageCanvas, false if it is measured using width of ImageCanvas.</div></div>",1036:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1036\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMeasureScaleByHeight(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">scaleByHeight</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if the scaling is measured using height of ImageCanvas, false if it is measured using width of ImageCanvas.</div></div>",1037:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1037\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetImagePosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">position</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the position of center of the image, represented by percentage (0 to 1).&nbsp; Setting it to (0, 0) will set the center of the image to be top-left, (0.5, 0.5) will be center of ImageCanvas, etc.</div></div>",1038:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1038\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetImagePosition() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the position of center of the image, represented by percentage (0 to 1).&nbsp; (0, 0) will indicates the center of the image to be top-left, (0.5, 0.5) will be center of ImageCanvas, etc.</div></div>",1039:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1039\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual const</span> std::vector&lt;glm::vec2&gt;&amp; GetImageVertices() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets all the vertices (from last frame or this frame depending on when you call this) of the image itself in global position</div></div>",1040:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1040\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetImageMinX() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the minimum global X position of the image (from last frame or this frame depending on when you call this)</div></div>",1041:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1041\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetImageMaxX() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the maximum global X position of the image (from last frame or this frame depending on when you call this)</div></div>",1042:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1042\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetImageMinY() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the minimum global Y position of the image (from last frame or this frame depending on when you call this)</div></div>",1043:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1043\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetImageMaxY() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the maximum global Y position of the image (from last frame or this frame depending on when you call this)</div></div>",1044:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1044\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> UseDefaultPanning(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">pan</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if user can pan by default when holding space button</div></div>",1045:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1045\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsUsingDefaultPanning() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if user can pan by default when holding space button</div></div>",1046:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1046\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> UseDefaultZooming(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">zoom</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if user can zoom by default when using the scroll wheel</div></div>",1047:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1047\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsUsingDefaultZooming() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if user can zoom by default when using the scroll wheel</div></div>",1048:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1048\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDefaultMinZoom(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">min</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the minimum scale value when using default zooming (inclusive)</div></div>",1049:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1049\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetDefaultMinZoom() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the minimum scale value when using default zooming (inclusive)</div></div>",1050:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1050\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDefaultMaxZoom(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">max</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the maximum scale value when using default zooming (inclusive)</div></div>",1051:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1051\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual float</span> GetDefaultMaxZoom() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the maximum scale value when using default zooming (inclusive)</div></div>",1052:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1052\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> UseDefaultRotating(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">rotate</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if the user can rotate by default when holding the R key</div></div>",1053:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1053\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsUsingDefaultRotating() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if the user can rotate by default when holding the R key</div></div>",1054:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1054\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShowHorizontalScrollbar(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">scrollbar</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if the horizontal scrollbar is available to use when the image is zoomed larger than the ImageCanvas</div></div>",1055:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1055\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsShowHorizontalScrollbar() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if the horizontal scrollbar is available to use when the image is zoomed larger than the ImageCanvas</div></div>",1056:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1056\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetShowVerticalScrollbar(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">scrollbar</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if the vertical scrollbar is available to use when the image is zoomed larger than the ImageCanvas</div></div>",1057:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1057\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsShowVerticalScrollbar() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Returns if the vertical scrollbar is available to use when the image is zoomed larger than the ImageCanvas</div></div>",1058:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1058\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetHorizontalScrollbar(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Scrollbar*&nbsp;</td><td class=\"PName last\">scrollbar</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the horizontal scrollbar object used by ImageCanvas</div></div>",1059:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1059\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Scrollbar* GetHorizontalScrollbar() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the horizontal scrollbar object used by ImageCanvas</div></div>",1060:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1060\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVerticalScrollbar(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">Scrollbar*&nbsp;</td><td class=\"PName last\">scrollbar</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the vertical scrollbar object used by ImageCanvas</div></div>",1061:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1061\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Scrollbar* GetVerticalScrollbar() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">Gets the vertical scrollbar object used by ImageCanvas</div></div>",1062:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1062\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::ImageFitting GetFitting() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\"><b>Fitting has no effect on ImageCanvas</b></div></div>",1063:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1063\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFitting(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Enums::</td><td class=\"PType\">ImageFitting&nbsp;</td><td class=\"PName last\">fitting</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\"><b>Fitting has no effect on ImageCanvas</b></div></div>",1064:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1064\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::GUIObjectType GetType() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">See Widget::GetType</div></div>",1065:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1065\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ImageCanvas* Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">cloneChildren</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See Widget::Clone</div></div>"});