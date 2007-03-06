
/*
 * Referencer is released under the GNU General Public License v2
 * See the COPYING file for licensing details.
 *
 * Copyright 2007 John Spray
 * (Exceptions listed in README)
 *
 */



#include <iostream>

#include <libgnomevfsmm.h>

#include "TagList.h"
#include "DocumentList.h"
#include "LibraryParser.h"
#include "ProgressDialog.h"
#include "Utility.h"

#include "Library.h"


Library::Library ()
{
	doclist_ = new DocumentList ();
	taglist_ = new TagList ();
}


Library::~Library ()
{
	delete doclist_;
	delete taglist_;
}


Glib::ustring Library::writeXML ()
{
	std::ostringstream out;
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	out << "<library>\n";
	taglist_->writeXML (out);
	doclist_->writeXML (out);
	out << "</library>\n";
	return out.str ();
}


bool Library::readXML (Glib::ustring XMLtext)
{
	TagList *newtags = new TagList ();
	DocumentList *newdocs = new DocumentList ();

	LibraryParser parser (*newtags, *newdocs);
	Glib::Markup::ParseContext context (parser);
	try {
		context.parse (XMLtext);
	} catch (Glib::MarkupError const ex) {
		std::cerr << "Exception on line " << context.get_line_number () << ", character " << context.get_char_number () << ": '" << ex.what () << "'\n";
		Utility::exceptionDialog (&ex, "parsing Library XML.");

		delete newtags;
		delete newdocs;
		return false;
	}

	context.end_parse ();

	delete taglist_;
	taglist_ = newtags;

	delete doclist_;
	doclist_ = newdocs;

	return true;
}


void Library::clear ()
{
	taglist_->clear ();
	doclist_->clear ();
}



// True on success
bool Library::load (Glib::ustring const &libfilename)
{
	Gnome::Vfs::Handle libfile;

	Glib::RefPtr<Gnome::Vfs::Uri> liburi = Gnome::Vfs::Uri::create (libfilename);

	ProgressDialog progress;

	progress.setLabel (
		"<b><big>Opening "
		+ liburi->extract_short_name ()
		+ "</big></b>\n\nThis process may take some time, particularly\n"
		+ "if the library has been moved since it was last opened.");

	// If we get an exception and return, progress::~Progress should
	// take care of calling finish() for us.
	progress.start ();

	try {
		libfile.open (libfilename, Gnome::Vfs::OPEN_READ);
	} catch (const Gnome::Vfs::exception ex) {
		Utility::exceptionDialog (&ex, "opening library '" + libfilename + "'");
		return false;
	}

	Glib::RefPtr<Gnome::Vfs::FileInfo> fileinfo;
	fileinfo = libfile.get_file_info ();

	char *buffer = (char *) malloc (sizeof(char) * (fileinfo->get_size() + 1));
	if (!buffer) {
		std::cerr << "Warning: TagWindow::loadLibrary: couldn't allocate buffer\n";
		return false;
	}

	try {
		libfile.read (buffer, fileinfo->get_size());
	} catch (const Gnome::Vfs::exception ex) {
		Utility::exceptionDialog (&ex, "reading library '" + libfilename + "'");
		free (buffer);
		return false;
	}
	buffer[fileinfo->get_size()] = 0;

	Glib::ustring rawtext = buffer;
	free (buffer);
	libfile.close ();

	std::cerr << "Reading XML...\n";
	if (!readXML (rawtext))
		return false;
	std::cerr << "Done, got " << doclist_->getDocs ().size() << " docs\n";

	int i = 0;
	DocumentList::Container &docs = doclist_->getDocs ();
	DocumentList::Container::iterator docit = docs.begin ();
	DocumentList::Container::iterator const docend = docs.end ();
	for (; docit != docend; ++docit) {
		progress.getLock ();
		progress.update ((double)(i++) / (double)docs.size ());
		if (Utility::fileExists (docit->getFileName())) {
			// Do nothing, all is well, the file is still there
			std::cerr << "Filename still good: " << docit->getFileName() << "\n";
		} else if (!docit->getRelFileName().empty()) {
			// Oh no!  We lost the file!  But we've got a relfilename!  Is relfilename still valid?
			Glib::ustring filename = Glib::build_filename (
				Glib::path_get_dirname (libfilename),
				docit->getRelFileName());
			std::cerr << "Derived from relative: " << filename << "\n";
			if (Utility::fileExists (filename)) {
				std::cerr << "\tValid\n";
				docit->setFileName (filename);
			}
		}
		progress.releaseLock ();
	}

	progress.finish ();

	return true;
}


// True on success
bool Library::save (Glib::ustring const &libfilename)
{
	Gnome::Vfs::Handle libfile;

	Glib::ustring tmplibfilename = libfilename + ".save-tmp";

	try {
		libfile.create (tmplibfilename, Gnome::Vfs::OPEN_WRITE,
			false, Gnome::Vfs::PERM_USER_READ | Gnome::Vfs::PERM_USER_WRITE);
	} catch (const Gnome::Vfs::exception ex) {
		Utility::exceptionDialog (&ex, "opening file '" + tmplibfilename + "'");
		return false;
	}

	DocumentList::Container &docs = doclist_->getDocs ();
	DocumentList::Container::iterator docit = docs.begin ();
	DocumentList::Container::iterator const docend = docs.end ();
	for (; docit != docend; ++docit) {
		if (Utility::fileExists (docit->getFileName())) {
			docit->updateRelFileName (libfilename);
		}
	}

	Glib::ustring rawtext = writeXML ();

	try {
		libfile.write (rawtext.c_str(), strlen(rawtext.c_str()));
	} catch (const Gnome::Vfs::exception ex) {
		Utility::exceptionDialog (&ex, "writing to file '" + tmplibfilename + "'");
		return false;
	}

	libfile.close ();

	// Forcefully move our tmp file into its real position
	try {
		Gnome::Vfs::Handle::move (tmplibfilename, libfilename, true);
	} catch (const Gnome::Vfs::exception ex) {
		Utility::exceptionDialog (&ex, "moving '"
			+ tmplibfilename + "' to '" + libfilename + "'");
		return false;
	}

	return true;
}


void Library::writeBibtex (
	Glib::ustring const &bibfilename,
	std::vector<Document*> const &docs,
	bool const usebraces)
{
	Glib::ustring tmpbibfilename = bibfilename + ".export-tmp";

	Gnome::Vfs::Handle bibfile;

	try {
		bibfile.create (tmpbibfilename, Gnome::Vfs::OPEN_WRITE,
			false, Gnome::Vfs::PERM_USER_READ | Gnome::Vfs::PERM_USER_WRITE);
	} catch (const Gnome::Vfs::exception ex) {
		std::cerr << "TagWindow::onExportBibtex: "
			"exception in create '" << ex.what() << "'\n";
		Utility::exceptionDialog (&ex, "opening BibTex file");
		return;
	}

	std::ostringstream bibtext;

	std::vector<Document*>::const_iterator it = docs.begin ();
	std::vector<Document*>::const_iterator const end = docs.end ();
	for (; it != end; ++it) {
		(*it)->writeBibtex (bibtext, usebraces);
	}

	try {
		bibfile.write (bibtext.str().c_str(), strlen(bibtext.str().c_str()));
	} catch (const Gnome::Vfs::exception ex) {
		Utility::exceptionDialog (&ex, "writing to BibTex file");
		bibfile.close ();
		return;
	}

	bibfile.close ();

	// Forcefully move our tmp file into its real position
	Gnome::Vfs::Handle::move (tmpbibfilename, bibfilename, true);
}
