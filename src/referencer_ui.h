
#ifndef REFERENCER_UI_H
#define REFERENCER_UI_H

Glib::ustring referencer_ui =
		"<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='LibraryMenu'>"
		"      <menuitem action='NewLibrary'/>"
		"      <menuitem action='OpenLibrary'/>"
		"      <separator/>"
		"      <menuitem action='SaveLibrary'/>"
		"      <menuitem action='SaveAsLibrary'/>"
		"      <separator/>"
		"      <menuitem action='ExportBibtex'/>"
		"      <menuitem action='ManageBibtex'/>"
		"      <menuitem action='ExportNotes'/>"
		"      <menuitem action='Import'/>"
		"      <separator/>"
		"      <menuitem action='Properties'/>"
		"      <separator/>"
		"      <menuitem action='WorkOffline'/>"
		"      <separator/>"
		"      <placeholder name='PluginLibraryActions'/>"
		"      <separator/>"
		"      <menuitem action='Quit'/>"
		"    </menu>"
		"    <menu action='EditMenu'>"
		"      <menuitem action='PasteBibtex'/>"
		"      <menuitem action='CopyCite'/>"
		"      <separator/>"
		"      <placeholder name='PluginEditActions'/>"
		"      <separator/>"
		"      <menuitem action='Preferences'/>"
		"    </menu>"
		"    <menu action='ViewMenu'>"
		"      <menuitem action='UseIconView'/>"
		"      <menuitem action='UseListView'/>"
		"      <menu action='ViewMenuSort' name='ViewMenuSort'>"
		"        <placeholder name='ViewMenuSortItems'/>"
		"      </menu>"
		"      <separator/>"
		"      <menuitem action='ShowTagPane'/>"
		"	   <menuitem action='ShowNotesPane'/>"
		"      <separator/>"
		"      <placeholder name='PluginViewActions'/>"
		"    </menu>"
		"    <menu action='TagMenu'>"
		"      <menuitem action='CreateTag'/>"
		"      <separator/>"
		"      <menuitem action='DeleteTag'/>"
		"      <menuitem action='RenameTag'/>"
		"      <separator/>"
		"      <placeholder name='PluginTagActions'/>"
		"    </menu>"
		"    <menu action='DocMenu'>"
		"      <menuitem action='AddDocFile'/>"
		"      <menuitem action='AddDocFolder'/>"
		"      <menuitem action='AddDocDoi'/>"
		"      <menuitem action='AddDocUnnamed'/>"
//		"      <menuitem action='LibraryFolder'/>"
		"      <separator/>"
		"      <menuitem action='OpenDoc'/>"
		"      <menuitem action='DocProperties'/>"
		"      <separator/>"
		"      <menuitem action='RemoveDoc'/>"
		"      <menuitem action='GetMetadataDoc'/>"
		"      <menuitem action='RenameDoc'/>"
		"      <menuitem action='DeleteDoc'/>"
		"      <separator/>"
		"      <placeholder name='PluginDocumentActions'/>"
		"    </menu>"
		"    <menu action='ToolsMenu'>"
		"      <menuitem action='Search'/>"
		"      <placeholder name='PluginToolsActions'/>"
		"    </menu>"
		"    <placeholder name='PluginMenus'/>"
		"    <menu action='HelpMenu'>"
		"      <menuitem action='Introduction'/>"
		"      <menuitem action='About'/>"
		"      <separator/>"
		"      <placeholder name='PluginHelpActions'/>"
		"    </menu>"
		"  </menubar>"
		"  <toolbar name='ToolBar'>"
		"    <toolitem action='NewLibrary'/>"
		"    <toolitem action='OpenLibrary'/>"
		"    <toolitem action='SaveLibrary'/>"
		"    <separator/>"
		"    <toolitem action='CopyCite'/>"
		"    <toolitem action='PasteBibtex'/>"
		"    <separator/>"
		"    <placeholder name='PluginToolBarActions'/>"
		"  </toolbar>"
		"  <toolbar name='TagBar'>"
		"    <toolitem action='CreateTag'/>"
		"    <toolitem action='DeleteTag'/>"
		"    <placeholder name='PluginTagBarActions'/>"
		"  </toolbar>"
		"  <toolbar name='DocBar'>"
		"    <toolitem action='AddDocFile'/>"
		"    <toolitem action='RemoveDoc'/>"
		"    <placeholder name='PluginDocBarActions'/>"
		"  </toolbar>"
		"  <popup name='DocPopup'>"
		"    <menuitem action='RemoveDoc'/>"
		"    <menuitem action='GetMetadataDoc'/>"
		"    <menuitem action='OpenDoc'/>"
		"    <separator/>"
		"    <placeholder name='PluginDocPopupActions'/>"
		"    <separator/>"
		"    <placeholder name='WebLinkDocPopupActions'/>"
		"    <separator/>"
		"    <menu name='TaggerMenu' action='TaggerMenuAction'>"
		"      <placeholder name='TaggerMenuTags'/>"
		"      <separator/>"
		"      <menuitem action='CreateAndAttachTag'/>"
		"    </menu>"
		"    <separator/>"
		"    <menuitem action='DocProperties'/>"
		"  </popup>"
		"  <popup name='TagPopup'>"
		"    <menuitem action='CreateTag'/>"
		"    <separator/>"
		"    <menuitem action='DeleteTag'/>"
		"    <menuitem action='RenameTag'/>"
		"    <separator/>"
		"    <placeholder name='PluginTagPopupActions'/>"
		"  </popup>"
		"  <accelerator action='Find'/>"
		"</ui>";

#endif
