 /*
  * wxRovio - WOWWEE ROVIO PC Control Application
  * Copyright (C) 2013  GCY <http://gcyrobot.blogspot.tw/>.
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef __LOGGING_DIALOG__
#define __LOGGING_DIALOG__

#include <wx/wx.h>

enum{
	ID_USERNAME = 400,
	ID_PASSWORD,
	ID_IP,
	ID_RESET
};

class LoggingDialog:public wxDialog
{
public:
	LoggingDialog();

	LoggingDialog(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxString &caption = wxT("Logging"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	bool Create(wxWindow *parent,
		wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Logging"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	void Init();
	void CreateControls();

	void OnReset(wxCommandEvent &event);

	bool TransferDataToWindow();
	bool TransferDataFromWindow();

	wxString GetUsername(){return username;}
	wxString GetPassword(){return password;}
	wxString GetIP(){return ip;}
   private:
	   wxString username;
	   wxString password;
	   wxString ip;

      DECLARE_CLASS(LoggingDialog)
      DECLARE_EVENT_TABLE()
};

#endif