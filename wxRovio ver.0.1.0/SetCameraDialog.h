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

#ifndef __SET_CAMERA_DIALOG__
#define __SET_CAMERA_DIALOG__

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "wxRovio.h"

enum{
	ID_SET_CAMERA_FRAME_RATE = 500,
	ID_SET_CAMERA_RESOLUTION,
	ID_SET_CAMERA_BRIGHTNESS,
	ID_SET_CAMERA_QUALITY,
	ID_SET_CAMERA_FREQUENCY,
	ID_SET_CAMERA_NIGHT_MODE_STATE,
	ID_SET_CAMERA_NIGHT_MODE_FRAME_RATE,
	ID_SET_CAMERA_AUTO_GAIN_CONTRL,
	ID_SET_CAMERA_RESET
};

class SetCameraDialog:public wxDialog
{
public:
	SetCameraDialog();

	SetCameraDialog(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxString &caption = wxT("Set Camera"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	bool Create(wxWindow *parent,
		wxWindowID id = wxID_ANY,
	    const wxString &caption = wxT("Set Camera"),
	    const wxPoint &pos = wxDefaultPosition,
	    const wxSize &size = wxDefaultSize,
	    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU);

	void Init();
	void CreateControls();

	void SetAllValue(RovioState);

	int GetFrameRate(){return frame_rate;}
	int GetQuality(){return quality;}
	int GetResolution(){return resolution;}
	int GetBrightness(){return brightness;}
	bool GetNightModeState(){return night_mode_state;}
	bool GetAutoGainContrl(){return auto_gain_contrl_state;}
	int GetNightModeFrameRate()
	{
		if(night_mode_frame_rate == 0){
			return 2;
		}
		else if(night_mode_frame_rate == 1){
			return 4;
		}
		else if(night_mode_frame_rate == 2){
			return 8;
		}
	}
	int GetFrequency()
	{
		if(frequency == 0){
			return 0;
		}
		else if(frequency == 1){
			return 50;
		}
		else if(frequency == 2){
			return 60;
		}
	}

	void OnReset(wxCommandEvent &event);

	bool TransferDataToWindow();
	bool TransferDataFromWindow();
private:
	int frame_rate;
	int quality;
	int resolution;
	int brightness;
	int frequency;
	bool night_mode_state;
	int night_mode_frame_rate;
	bool auto_gain_contrl_state;

	DECLARE_CLASS(SetCameraDialog)
	DECLARE_EVENT_TABLE()
};

#endif