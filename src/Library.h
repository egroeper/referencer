
/*
 * Referencer is released under the GNU General Public License v2
 * See the COPYING file for licensing details.
 *
 * Copyright 2007 John Spray
 * (Exceptions listed in README)
 *
 */



#ifndef LIBRARY_H
#define LIBRARY_H

#include <glibmm/ustring.h>

class Document;
class DocumentList;
class TagList;
class RefWindow;

class Library {
	public:
	DocumentList *doclist_;
	TagList *taglist_;

	Library (RefWindow &tagwindow);
	~Library ();

	void clear ();
	bool load (Glib::ustring const &libfilename);
	bool save (Glib::ustring const &libfilename);

	Glib::ustring writeXML ();
	bool readXML (Glib::ustring XML);

	void writeBibtex (
		Glib::ustring const &bibfilename,
		std::vector<Document*> const &docs,
		bool const usebraces,
		bool const utf8);

	// The naming is BibtexFoo everywhere else, but in Library
	// we use the manage_ prefix to be consistent with the file format
	// which uses manage_ to be future-proof with more general manage
	// functionality
	void manageBibtex (
		Glib::ustring const &target,
		bool const brackets,
		bool const utf8);
	Glib::ustring getBibtexTarget () {return manage_target_;}
	bool getBibtexBraces () {return manage_braces_;}
	bool getBibtexUTF8 () {return manage_utf8_;}

	Glib::ustring getLibraryFolder () {return library_folder_uri_;}
	void setLibraryFolder (Glib::ustring LibraryFolderUri) {
	        library_folder_uri_ = LibraryFolderUri;
	}

	bool libraryFolderDialog ();

	private:
	Glib::ustring manage_target_;
	bool manage_braces_;
	bool manage_utf8_;

	Glib::ustring library_folder_uri_;
	bool library_folder_monitor_;

	RefWindow &tagwindow_;
};

#endif
