﻿NDSummary.OnToolTipsLoaded("File:◀️_ssGUI_API_▶️/ssGUIManager.hpp",{1612:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1612\"><div class=\"CPEntry TClass Current\"><div class=\"CPModifiers\"><span class=\"SHKeyword\">namespace</span> </div><div class=\"CPName\">ssGUI</div></div></div></div>",1613:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1613\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;ssGUIManager</div></div></div><div class=\"TTSummary\">This manages all the GUI Objects, system input, update and render order, etc...&nbsp; There\'s should only be 1 instance of ssGUIManager.</div></div>",1615:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1615\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> AddGUIObject(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">obj</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Only main window should be added, the reason why it is accepting ssGUI::GUIObject is for future compatibility.</div></div>",1616:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1616\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> RemoveGUIObject(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">obj</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes the main window object added by AddGUIObject. ssGUI::Hierarchy::Delete should be used instead to remove any ssGUI::MainWindow.</div></div>",1617:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1617\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">int</span> GetGUIObjectCount()</div></div><div class=\"TTSummary\">Returns the number of main window added by AddGUIObject</div></div>",1618:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1618\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> StartRunning()</div></div><div class=\"TTSummary\">Starts the ssGUIManager.&nbsp; This will block the thread until all MainWindows are closed</div></div>",1619:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1619\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\">ssGUI::Backend::BackendSystemInputInterface* GetBackendInputInterface()</div></div><div class=\"TTSummary\">Gets the ssGUI::Backend::BackendSystemInputInterface used by ssGUIManager</div></div>",1620:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1620\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">static</span> ssGUI::ssGUIManager* GetInstance()</div></div><div class=\"TTSummary\">Static method that allow accessing ssGUIManager from anywhere. If there\'s no instance found, nullptr is returned.</div></div>",1621:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1621\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">int</span> AddPreGUIUpdateEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>()&gt;&nbsp;</td><td class=\"PName last\">event</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds event callback that gets triggered before the update function is called for GUI objects.&nbsp; Returns index that can be used to remove the event callback later.</div></div>",1622:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1622\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> RemovePreGUIUpdateEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes event callback that gets triggered before the update function is called for GUI objects</div></div>",1623:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1623\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">int</span> AddPostGUIUpdateEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>()&gt;&nbsp;</td><td class=\"PName last\">event</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds event callback that gets triggered after the update function (before render function) is called for GUI objects.&nbsp; Returns index that can be used to remove the event callback later.</div></div>",1624:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1624\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> RemovePostGUIUpdateEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes event callback that gets triggered after the update function (before render function) is called for GUI objects</div></div>",1625:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1625\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">int</span> AddPostGUIRenderingUpdateEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>()&gt;&nbsp;</td><td class=\"PName last\">event</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds event callback that gets triggered after the render function is called for GUI objects.&nbsp; Returns index that can be used to remove the event callback later.</div></div>",1630:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1630\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> RemovePostGUIRenderingUpdateEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes event callback that gets triggered after the render function is called for GUI objects</div></div>",1631:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1631\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">int</span> AddOnCustomRenderEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>( std::list&lt;ssGUI::GUIObject*&gt;&amp; ) &gt;&nbsp;</td><td class=\"PName last\">event</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Adds event callback that is responsible of rendering all the GUI Objects instead of using the default rendering by ssGUIManager Returns index that can be used to remove the event callback later.</div></div>",1632:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1632\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> RemoveOnCustomRenderEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">int</span>&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Removes the event callback that is responsible of rendering all the GUI Objects instead of using the default rendering by ssGUIManager</div></div>",1633:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1633\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> SetForceRendering(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">force</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets if we force all GUI objects to redraw</div></div>",1634:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1634\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">bool</span> IsForceRendering()</div></div><div class=\"TTSummary\">Returns if we force all GUI objects to redraw</div></div>",1635:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1635\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">void</span> SetTargetFramerate(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\"><span class=\"SHKeyword\">float</span>&nbsp;</td><td class=\"PName last\">framerate</td></tr></table></td><td class=\"PAfterParameters\">)</td></tr></table></div></div><div class=\"TTSummary\">Sets the target framerate (FPS), negative to disable it</div></div>",1636:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1636\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">float</span> GetTargetFramerate()</div></div><div class=\"TTSummary\">Returns the target framerate (FPS), negative if disabled</div></div>",1637:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1637\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">void</span> CleanUpDefaultSystemGUIObjects()</div></div><div class=\"TTSummary\">Clean up any default GUI Objects created by ssGUI. This is automatically called.</div></div>",1638:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1638\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">inline void</span> Clear()</div></div><div class=\"TTSummary\">Clears the console</div></div>"});