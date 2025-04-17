//
//	TemplateEditor.h
//	wolverine_engine
//
//	Created by Jacob Robinson on 4/14/2025.
//	Handles the creation and functions of the "Template Editor" window for the game editor
//

#ifndef TemplateEditor_h
#define TemplateEditor_h

#include "EditorManager.h"
#include "ShallowActorClass.h"

class TemplateEditorWindow
{
private:
    static inline bool show_template_editor = true;
    
    // Template the window is displaying
    static inline std::string selected_template = "";
    
    // Pointer to the shallow actor we're using to represent the template
    static inline std::shared_ptr<ShallowActor> template_rep = nullptr;
    
    /*
     * Saves the changes made to the current template, then update all current instances in the scene
     * NOTE: ONLY Update values if they're the same as the old value
     */
    static void SaveTemplateChanges();
public:
    /*
     * Toggle the window being displayed
     */
    static void ToggleWindow();
    
    /*
     * Save the changes to the current template, then change the template being edited
     * @param   to_edit the name of the new template to be edited
     */
    static void ChangeTemplate(std::string to_edit);
    
    /*
     * Display the template editor window
     */
    static void TemplateEditor();
};


#endif /* TemplateEditor.h */
