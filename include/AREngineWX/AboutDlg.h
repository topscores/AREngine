///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ABOUT_DLG_H__
#define __ABOUT_DLG_H__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AboutDlg
///////////////////////////////////////////////////////////////////////////////
class AboutDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxButton* m_button1;
	
	public:
		AboutDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 312,83 ), long style = wxDEFAULT_DIALOG_STYLE );
		~AboutDlg();
	
};

#endif //__ABOUT_DLG_H__
