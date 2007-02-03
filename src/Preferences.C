
#include <iostream>

#include "Utility.h"

#include "Preferences.h"


Preferences *_global_prefs;

#define CONF_PATH "/apps/referencer"

Preferences::Preferences ()
{
	confclient_ = Gnome::Conf::Client::get_default_client ();

	libraryfilename_ = confclient_->get_entry (CONF_PATH "/libraryfilename");
	workoffline_ = confclient_->get_entry (CONF_PATH "/workoffline");
	uselistview_ = confclient_->get_entry (CONF_PATH "/uselistview");
	showtagpane_ = confclient_->get_entry (CONF_PATH "/showtagpane");
	doilaunch_ = confclient_->get_entry (CONF_PATH "/doilaunch");
	metadatalookup_ = confclient_->get_entry (CONF_PATH "/metadatalookup");

	doilaunchdefault_ = "http://dx.doi.org/<!DOI!>";
	metadatalookupdefault_ =
		"http://www.crossref.org/openurl/?id=doi:<!DOI!>&noredirect=true";

	if (!confclient_->dir_exists (CONF_PATH)) {
		std::cerr << "Preferences::Preferences: CONF_PATH "
			"doesn't exist, setting it up\n";

		setLibraryFilename ("");
		setShowTagPane (true);
		setUseListView (false);
		setWorkOffline (false);
		setDoiLaunch (doilaunchdefault_);
		setMetadataLookup (metadatalookupdefault_);
		firsttime_ = true;
	} else {
		firsttime_ = false;
	}

	confclient_->add_dir (
		CONF_PATH,
		Gnome::Conf::CLIENT_PRELOAD_ONELEVEL);

	confclient_->notify_add (
		CONF_PATH,
		sigc::mem_fun (*this, &Preferences::onConfChange));

	xml_ = Gnome::Glade::Xml::create (
		Utility::findDataFile ("preferences.glade"));

	dialog_ = (Gtk::Dialog *) xml_->get_widget ("Preferences");
	workofflinecheck_ = (Gtk::CheckButton *) xml_->get_widget ("WorkOffline");
	workofflinecheck_->signal_toggled().connect (
		sigc::mem_fun (*this, &Preferences::onWorkOfflineToggled));

	doilaunchentry_ = (Gtk::Entry *) xml_->get_widget ("DoiLaunch");
	doilaunchentry_->signal_changed().connect (
		sigc::mem_fun (*this, &Preferences::onURLChanged));
	metadatalookupentry_ = (Gtk::Entry *) xml_->get_widget ("MetadataLookup");
	metadatalookupentry_->signal_changed().connect (
		sigc::mem_fun (*this, &Preferences::onURLChanged));

	Gtk::Button *button = (Gtk::Button *) xml_->get_widget ("ResetToDefaults");
	button->signal_clicked().connect (
		sigc::mem_fun (*this, &Preferences::onResetToDefaults));

	ignorechanges_ = false;
}


Preferences::~Preferences ()
{

}

void Preferences::onConfChange (int number, Gnome::Conf::Entry entry)
{
	std::cerr << "onConfChange: '" << entry.get_key () << "'\n";

	ignorechanges_ = true;

	Glib::ustring key = entry.get_key ();
	if (key == CONF_PATH "/workoffline") {
		workofflinecheck_->set_active (
			entry.get_value ().get_bool ());
		workofflinesignal_.emit ();
	} else if (key == CONF_PATH "/uselistview") {
		uselistviewsignal_.emit ();
	} else if (key == CONF_PATH "/showtagpane") {
		showtagpanesignal_.emit ();
	} else if (key == CONF_PATH "/doilaunch") {
		doilaunchentry_->set_text (
			entry.get_value ().get_string ());
	} else if (key == CONF_PATH "/metadatalookup") {
		metadatalookupentry_->set_text (
			entry.get_value ().get_string ());
	}

	ignorechanges_ = false;
}

void Preferences::showDialog ()
{
	ignorechanges_ = true;

	workofflinecheck_->set_active (
		confclient_->get_bool (workoffline_.get_key()));
	doilaunchentry_->set_text (
		confclient_->get_string (doilaunch_.get_key()));
	metadatalookupentry_->set_text (
		confclient_->get_string (metadatalookup_.get_key()));

	ignorechanges_ = false;

	dialog_->run ();
	dialog_->hide ();
}


void Preferences::onWorkOfflineToggled ()
{
	if (ignorechanges_) return;

	confclient_->set (
		workoffline_.get_key(), workofflinecheck_->get_active());
}


using Utility::DOIURLValid;

void Preferences::onURLChanged ()
{
	if (ignorechanges_) return;

	// Should we be telling gconf to ignore us for a minute, since
	// these operations cause a useless callback to onConfChange?

	// Silently don't remember bad settings for now -- should prompt the
	// user when the dialog closes if they still haven't entered something valid
	if (DOIURLValid (doilaunchentry_->get_text ())) {
		confclient_->set (
			doilaunch_.get_key(), doilaunchentry_->get_text ());
	}

	if (DOIURLValid (metadatalookupentry_->get_text ())) {
		confclient_->set (
			metadatalookup_.get_key(), metadatalookupentry_->get_text ());
	}
}

// This is just for the button in the dialog!
void Preferences::onResetToDefaults ()
{
	doilaunchentry_->set_text (doilaunchdefault_);
	metadatalookupentry_->set_text (metadatalookupdefault_);
}


Glib::ustring Preferences::getLibraryFilename ()
{
	return confclient_->get_string (libraryfilename_.get_key());
}


void Preferences::setLibraryFilename (Glib::ustring const &filename)
{
	return confclient_->set (libraryfilename_.get_key(), filename);
}


bool Preferences::getWorkOffline ()
{
	return confclient_->get_bool (workoffline_.get_key());
}


void Preferences::setWorkOffline (bool const &offline)
{
	confclient_->set (workoffline_.get_key(), offline);
}


sigc::signal<void>& Preferences::getWorkOfflineSignal ()
{
	return workofflinesignal_;
}


bool Preferences::getUseListView ()
{
	return confclient_->get_bool (uselistview_.get_key());
}


void Preferences::setUseListView (bool const &uselistview)
{
	confclient_->set (uselistview_.get_key(), uselistview);
}


sigc::signal<void>& Preferences::getUseListViewSignal ()
{
	return uselistviewsignal_;
}


bool Preferences::getShowTagPane ()
{
	return confclient_->get_bool (showtagpane_.get_key());
}


void Preferences::setShowTagPane (bool const &showtagpane)
{
	confclient_->set (showtagpane_.get_key(), showtagpane);
}


sigc::signal<void>& Preferences::getShowTagPaneSignal ()
{
	return showtagpanesignal_;
}


using Utility::StringPair;
using Utility::twoWaySplit;

StringPair Preferences::getDoiLaunch ()
{
	return twoWaySplit (
		confclient_->get_string (doilaunch_.get_key()),
		"<!DOI!>");
}


void Preferences::setDoiLaunch (Glib::ustring const &doilaunch)
{
	confclient_->set (doilaunch_.get_key(), doilaunch);
}


StringPair Preferences::getMetadataLookup ()
{
	return twoWaySplit (
		confclient_->get_string (metadatalookup_.get_key()),
		"<!DOI!>");
}


void Preferences::setMetadataLookup (Glib::ustring const &metadatalookup)
{
	confclient_->set (metadatalookup_.get_key(), metadatalookup);
}
