﻿NDSummary.OnToolTipsLoaded("File:◀️_ssGUI_API_▶️/GUIObjectClasses/MainWindow.hpp",{1235:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1235\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">ssGUI</div></div></div></div>",1236:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1236\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;MainWindow</div></div></div><div class=\"TTSummary\">Main Window is the <b>actual</b> window. Not to be confused with ssGUI::Window which lives inside MainWindow.&nbsp; You should call &lt;GUIObject::RedrawObject&gt; after changing the properties of the MainWindow.&nbsp; This class should not be overwritten unless you know what you are doing</div></div>",1238:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1238\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Render()</div></div><div class=\"TTSummary\">Renders all the entities drawn on this MainWindow. Called by ssGUI::ssGUIManager normally.</div></div>",1239:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1239\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ClearBackBuffer()</div></div><div class=\"TTSummary\">Clears all the entities drawn on this MainWindow</div></div>",1240:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1240\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Backend::BackendMainWindowInterface* GetBackendWindowInterface()</div></div></div>",1241:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1241\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Backend::BackendDrawingInterface* GetBackendDrawingInterface()</div></div></div>",1242:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1242\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::ivec2 GetDisplayPosition() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::GetDisplayPosition&gt;</div></div>",1243:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1243\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetDisplayPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">ivec2&nbsp;</td><td class=\"PName last\">pos</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetDisplayPosition&gt;</div></div>",1244:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1244\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::ivec2 GetPositionOffset() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::GetPositionOffset&gt;</div></div>",1245:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1245\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetTitle(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\">wstring&nbsp;</td><td class=\"PName last\">title</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::IsVSync&gt;</div></div>",1246:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1246\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::wstring GetTitle() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::IsVSync&gt;</div></div>",1247:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1247\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetIcon(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">ImageData&amp;&nbsp;</td><td class=\"PName last\">iconImage</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetIcon&gt;</div></div>",1248:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1248\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVSync(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">vSync</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetVSync&gt;</div></div>",1249:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1249\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsVSync() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::IsVSync&gt;</div></div>",1250:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1250\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetMSAA(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">level</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetMSAA&gt;</div></div>",1251:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1251\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetMSAA() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::GetMSAA&gt;</div></div>",1252:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1252\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetWindowMode(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Enums::</td><td class=\"PType\">WindowMode&nbsp;</td><td class=\"PName last\">WindowMode</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetWindowMode&gt;</div></div>",1253:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1253\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::WindowMode GetWindowMode() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::GetWindowMode&gt;</div></div>",1254:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1254\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Internal_FocusChanged(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">focused</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">(Internal ssGUI function) You should not be calling this function normally.</div></div>",1255:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1255\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetVisible(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">visible</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetVisible&gt;</div></div>",1256:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1256\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsVisible() <span class=\"SHKeyword\">const</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::IsVisible&gt;</div></div>",1257:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1257\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetEnabled(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">enabled</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">Proxy function for SetVisible</div></div>",1258:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1258\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsEnabled() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">Proxy function for IsVisible</div></div>",1259:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1259\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsSelfEnabled() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">Proxy function for IsVisible</div></div>",1260:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1260\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual bool</span> IsFocused() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::IsFocused&gt;</div></div>",1261:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1261\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetFocus(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">focus</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetFocus&gt;</div></div>",1262:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1262\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetPosition() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">This will always return (0, 0). To get the main window\'s position on the monitor, see GetDisplayPosition See &lt;ssGUI::Hierarchy::GetPosition&gt;</div></div>",1263:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1263\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">position</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">This will not set the position of the main window, to do that, see SetDisplayPosition See &lt;ssGUI::Hierarchy::SetPosition&gt;</div></div>",1264:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1264\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetGlobalPosition() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">This will always return (0, 0). To get the main window\'s position on the monitor, see GetDisplayPosition See &lt;ssGUI::Hierarchy::GetGlobalPosition&gt;</div></div>",1265:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1265\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetGlobalPosition(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">position</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">This will not set the position of the main window, to do that, see SetDisplayPosition See &lt;ssGUI::Hierarchy::SetGlobalPosition&gt;</div></div>",1266:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1266\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> glm::vec2 GetSize() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::GetSize&gt;</div></div>",1267:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1267\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> SetSize(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">size</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See &lt;BackendMainWindowInterface::SetSize&gt;</div></div>",1268:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1268\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> ssGUI::Enums::GUIObjectType GetType() <span class=\"SHKeyword\">const override</span></div></div><div class=\"TTSummary\">See Window::GetType</div></div>",1269:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1269\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Close() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">See Window::Close</div></div>",1270:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1270\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> AbortClosing() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">See Window::AbortClosing</div></div>",1271:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1271\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Internal_OnClose() <span class=\"SHKeyword\">override</span></div></div><div class=\"TTSummary\">See Window::Internal_OnClose</div></div>",1272:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1272\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> Internal_Draw()</div></div><div class=\"TTSummary\">This checks for window state before rendering</div></div>",1273:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1273\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Internal_Draw(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Backend::</td><td class=\"PType\">BackendDrawingInterface*&nbsp;</td><td class=\"PName last\">drawingInterface,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">mainWindow,</td></tr><tr><td class=\"PModifierQualifier first\">glm::</td><td class=\"PType\">vec2&nbsp;</td><td class=\"PName last\">mainWindowPositionOffset</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">This checks for window state before rendering</div></div>",1274:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1274\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Internal_Update(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::Backend::</td><td class=\"PType\">BackendSystemInputInterface*&nbsp;</td><td class=\"PName last\">inputInterface,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">InputStatus&amp;&nbsp;</td><td class=\"PName last\">inputStatus,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">mainWindow</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See GUIObject::Internal_Update</div></div>",1275:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1275\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> MainWindow* Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">cloneChildren</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">override</span></td></tr></table></div></div><div class=\"TTSummary\">See Window::Clone</div></div>"});