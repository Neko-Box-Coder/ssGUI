﻿NDContentPage.OnToolTipsLoaded({27:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype27\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;BaseEventCallback</div></div></div><div class=\"TTSummary\">Implementation class for EventCallback</div></div>",52:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype52\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual bool</span> IsEventListenerExist(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">key,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">adder</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Check if the listener exists with key and adder specified. Note that same key but with different adder is diffentiated.</div></div>",54:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype54\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">key,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">adder</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Removes a listener with the key and adder specified</div></div>",56:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype56\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual void</span> ClearEventListeners() = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Clears all event listeners</div></div>",57:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype57\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual int</span> GetEventListenerCount() <span class=\"SHKeyword\">const</span> = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the number of event listeners</div></div>",58:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype58\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> Notify(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">source</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Call this function to trigger this event callback</div></div>",59:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype59\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> BindToObject(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">bindObj</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Binds this event callback to a GUI object</div></div>",60:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype60\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ssGUIObjectIndex AddObjectReference(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">obj</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Proxy function to ObjectsReferences::AddObjectReference</div></div>",61:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype61\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> ssGUI::GUIObject* GetObjectReference(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ssGUIObjectIndex&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) <span class=\"SHKeyword\">const</span> = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Proxy function to ObjectsReferences::GetObjectReference</div></div>",62:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype62\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> RemoveObjectReference(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PType first\">ssGUIObjectIndex&nbsp;</td><td class=\"PName last\">index</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Proxy function to ObjectsReferences::RemoveObjectReference</div></div>",64:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype64\" class=\"NDPrototype\"><div class=\"PSection PPlainSection\"><span class=\"SHKeyword\">virtual</span> std::<span class=\"SHKeyword\">string</span> GetEventCallbackName() <span class=\"SHKeyword\">const</span> = <span class=\"SHNumber\">0</span></div></div><div class=\"TTSummary\">Returns the name of this event callback</div></div>",65:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype65\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual</span> EventCallback* Clone(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">newContainer,</td></tr><tr><td class=\"first\"></td><td class=\"PType\"><span class=\"SHKeyword\">bool</span>&nbsp;</td><td class=\"PName last\">copyListeners</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Clone this event callback with the option of copying the listeners</div></div>",567:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype567\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;ObjectsReferences</div></div></div><div class=\"TTSummary\">The purpose of this class is to track references between each GUI Objects.&nbsp; So when cloning happens, the cloned version of the GUI Objects will be referenced (if present) instead of the original GUI Objects.</div></div>",1122:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1122\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::</span>&#8203;GUIObject</div></div></div><div class=\"TTSummary\">This is the implementation class for ssGUI::GUIObject. See ssGUI::GUIObject for more details about the functions</div></div>",1695:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype1695\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;EventCallback</div></div></div><div class=\"TTSummary\">EventCallback enables the ablity to act against a certain event.</div></div>",1701:"<div class=\"NDToolTip TFunction LC\"><div id=\"NDPrototype1701\" class=\"NDPrototype WideForm\"><div class=\"PSection PParameterSection CStyle\"><table><tr><td class=\"PBeforeParameters\"><span class=\"SHKeyword\">virtual void</span> AddEventListener(</td><td class=\"PParametersParentCell\"><table class=\"PParameters\"><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">string</span>&nbsp;</td><td class=\"PName last\">key,</td></tr><tr><td class=\"PModifierQualifier first\">ssGUI::</td><td class=\"PType\">GUIObject*&nbsp;</td><td class=\"PName last\">adder,</td></tr><tr><td class=\"PModifierQualifier first\">std::</td><td class=\"PType\"><span class=\"SHKeyword\">function</span>&lt;<span class=\"SHKeyword\">void</span>(ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)&gt;&nbsp;</td><td class=\"PName last\">callback</td></tr></table></td><td class=\"PAfterParameters\">) = <span class=\"SHNumber\">0</span></td></tr></table></div></div><div class=\"TTSummary\">Adds a listener to this EventCallback by passing a key string for the ability to remove it later.</div></div>",11:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype11\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;OnRecursiveChildAddEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>before</b> a child is paranted recursively on this GUI object.&nbsp; The child object being added will be the source for triggering this event callback.</div></div>",81:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype81\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;SliderValueChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>shortly after</b> the value of the slider is changed.&nbsp; This is triggered under the &lt;ssGUI::Slider::MainLogic&gt; method therefore the event is not immediate.</div></div>",18:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype18\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;OnWindowCloseEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>before</b> this GUI Object (Window) is closed. Window(Container) will be the source for triggering this event callback.</div></div>",74:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype74\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;SliderValueFinishedChangingEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>shortly after</b> the value of the slider has <b>finished</b> changing.&nbsp; Meaning this will be triggered if the slider value stayed the same for current update <b>and</b> the previous update it has been changed.</div></div>",4:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype4\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;OnFontChangeEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>before</b> the font of a text is changed. Text(Container) will be the source for triggering this event callback.</div></div>",146:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype146\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;BackgroundColorChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> the background color is changed. (Container) will be the source for triggering this event callback.</div></div>",88:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype88\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;ChildRemovedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> a child is removed on this GUI object The child object being removed will be the source for triggering this event callback.</div></div>",67:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype67\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;MinMaxSizeChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> min or max size of a GUI object has changed. Container will be the source for triggering this event callback.</div></div>",104:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype104\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;SliderValueChangedViaGuiEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>shortly after</b> the value of the slider is changed.&nbsp; This is triggered under the &lt;ssGUI::Slider::MainLogic&gt; method therefore the event is not immediate.</div></div>",125:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype125\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;ItemSelectedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> an item is selected. Dropdown(Container) will be the source for triggering this event callback.</div></div>",111:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype111\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;RecursiveChildRemovedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> a child is removed recursively on this GUI object The child object being removed will be the source for triggering this event callback.</div></div>",118:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype118\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;FocusLostEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered after the GUI Object lost focus</div></div>",132:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype132\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;RecursiveChildAddedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> a child is paranted recursively on this GUI object The child object being added will be the source for triggering this event callback.</div></div>",294:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype294\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;FocusedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered after the GUI Object gained focus</div></div>",167:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype167\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;ChildPositionChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> &lt;GUIObject::ChangeChildOrderToBeforePosition&gt; or &lt;GUIObject::ChangeChildOrderToAfterPosition&gt; is called.&nbsp; The child that is passed to &lt;GUIObject::ChangeChildOrderToBeforePosition&gt; or &lt;GUIObject::ChangeChildOrderToAfterPosition&gt; will be the source for triggering this event callback.</div></div>",153:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype153\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;ButtonStateChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> the button state has changed. Button(Container) will be the source for triggering this event callback.</div></div>",174:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype174\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;OnObjectDestroyEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>before</b> the GUI object is destroyed. Container will be the source for triggering this event callback.</div></div>",181:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype181\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;WindowDragStateChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> this GUI object (window) drag state is changed. Window(Container) will be the source for triggering this event callback.</div></div>",160:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype160\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;SizeChangedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> this GUI object\'s size has changed. Container will be the source for triggering this event callback.</div></div>",139:"<div class=\"NDToolTip TClass LC\"><div class=\"NDClassPrototype\" id=\"NDClassPrototype139\"><div class=\"CPEntry TClass Current\"><div class=\"CPName\"><span class=\"Qualifier\">ssGUI::&#8203;EventCallbacks::</span>&#8203;ChildAddedEventCallback</div></div></div><div class=\"TTSummary\">This event callback is triggered <b>after</b> a child is paranted on this GUI object The child object being added will be the source for triggering this event callback.</div></div>"});