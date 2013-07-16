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

#ifndef __SET_OTHER_DIALOG__
#define __SET_OTHER_DIALOG__

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "wxRovio.h"

enum{
	ID_SET_OTHER_SPEED = 700,
	ID_SET_OTHER_LIGHT_A,
	ID_SET_OTHER_LIGHT_B,
	ID_SET_OTHER_LIGHT_C,
	ID_SET_OTHER_LIGHT_D,
	ID_SET_OTHER_LIGHT_E,
	ID_SET_OTHER_LIGHT_F,

	ID_SET_OTHER_RESET
};

class SetOtherDialog:public wxDialog
{
public:
	SetOtherDialog();

	SetOtherDialog(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxString &caption = wxT("Set Other"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	bool Create(wxWindow *parent,
		wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Set Other"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	void Init();
	void CreateControls();

	void SetAllValue(RovioState);

	int GetSpeed(){return speed;}
	unsigned char GetLight(){return light;}

	void OnReset(wxCommandEvent &event);

	bool TransferDataToWindow();
	bool TransferDataFromWindow();
private:
	int speed;
	unsigned char light;

	DECLARE_CLASS(SetOtherDialog)
	DECLARE_EVENT_TABLE()
};

#endif