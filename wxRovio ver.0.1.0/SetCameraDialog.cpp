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

#include "SetCameraDialog.h"

IMPLEMENT_CLASS(SetCameraDialog,wxDialog)

BEGIN_EVENT_TABLE(SetCameraDialog,wxDialog)
   EVT_BUTTON(ID_SET_CAMERA_RESET,SetCameraDialog::OnReset)
END_EVENT_TABLE()

SetCameraDialog::SetCameraDialog()
{
	Init();
}

SetCameraDialog::SetCameraDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
	Init();
	Create(parent,id,caption,pos,size,style);
}

inline void SetCameraDialog::Init()
{
}

void SetCameraDialog::SetAllValue(RovioState rovio_state)
{
	frame_rate = rovio_state.frame_rate;
	quality = rovio_state.quality;
	resolution = rovio_state.resolution;
	brightness = rovio_state.brightness;
	night_mode_state = rovio_state.night_mode_state;
	auto_gain_contrl_state = rovio_state.auto_gain_contrl_state;

	if(rovio_state.frequency == 0){
		frequency = 0;
	}
	else if(rovio_state.frequency == 50){
		frequency = 1;
	}
	else if(rovio_state.frequency == 60){
		frequency = 2;
	}

	if(rovio_state.night_mode_frame_rate == 2){
		night_mode_frame_rate = 0;
	}
	else if(rovio_state.night_mode_frame_rate == 4){
		night_mode_frame_rate = 1;
	}
	else if(rovio_state.night_mode_frame_rate == 8){
		night_mode_frame_rate = 2;
	}
}

bool SetCameraDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS | wxDIALOG_EX_CONTEXTHELP);

	if(!wxDialog::Create(parent,id,caption,pos,size,style)){
	  return false;
	}

	CreateControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	Center();

	return true;
}

void SetCameraDialog::CreateControls()
{
	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(top);

	wxBoxSizer *Box = new wxBoxSizer(wxVERTICAL);
	top->Add(Box,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	wxStaticText *frame_rate_lable = new wxStaticText(this,wxID_STATIC,wxT("&Frame Rate¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(frame_rate_lable,0,wxALIGN_LEFT | wxALL,5);

	wxSpinCtrl *frame_rate_spin = new wxSpinCtrl(this,ID_SET_CAMERA_FRAME_RATE,wxEmptyString,wxDefaultPosition,wxSize(60,-1),wxSP_ARROW_KEYS,2,32,20);
	Box->Add(frame_rate_spin,0,wxGROW | wxALL,5);

	wxStaticText *resolution_lable = new wxStaticText(this,wxID_STATIC,wxT("&Resolution¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(resolution_lable,0,wxALIGN_LEFT | wxALL,5);

	wxArrayString items;
	items.Add(wxT("176 x 144"));
	items.Add(wxT("320 x 240"));
	items.Add(wxT("352 x 288"));
	items.Add(wxT("640 x 480"));
	wxChoice *resolution_choice = new wxChoice(this,ID_SET_CAMERA_RESOLUTION,wxDefaultPosition,wxDefaultSize,items);
	Box->Add(resolution_choice,0,wxGROW | wxALL,5);

	wxStaticText *quality_lable = new wxStaticText(this,wxID_STATIC,wxT("&Quality¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(quality_lable,0,wxALIGN_LEFT | wxALL,5);

	items.Clear();
	items.Add(wxT("Low"));
	items.Add(wxT("Mid"));
	items.Add(wxT("High"));
	wxChoice *quality_choice = new wxChoice(this,ID_SET_CAMERA_QUALITY,wxDefaultPosition,wxDefaultSize,items);
	Box->Add(quality_choice,0,wxGROW | wxALL,5);

	wxStaticText *brightness_lable = new wxStaticText(this,wxID_STATIC,wxT("&Brightness¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(brightness_lable,0,wxALIGN_LEFT | wxALL,5);

	wxSlider *brightness_slider = new wxSlider(this,ID_SET_CAMERA_BRIGHTNESS,4,0,6,wxDefaultPosition,wxSize(200,-1),wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
	Box->Add(brightness_slider,0,wxGROW | wxALL,5);

	wxStaticText *frequency_lable = new wxStaticText(this,wxID_STATIC,wxT("&Frequency¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(frequency_lable,0,wxALIGN_LEFT | wxALL,5);

	items.Clear();
	items.Add(wxT("Auto"));
	items.Add(wxT("50"));
	items.Add(wxT("60"));
	wxChoice *frequency_choice = new wxChoice(this,ID_SET_CAMERA_FREQUENCY,wxDefaultPosition,wxDefaultSize,items);
	Box->Add(frequency_choice,0,wxGROW | wxALL,5);

	wxStaticText *night_mode_state_lable = new wxStaticText(this,wxID_STATIC,wxT("&Night Mode State¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(night_mode_state_lable,0,wxALIGN_LEFT | wxALL,5);

	items.Clear();
	items.Add(wxT("OFF"));
	items.Add(wxT("ON"));
	wxChoice *night_mode_state_choice = new wxChoice(this,ID_SET_CAMERA_NIGHT_MODE_STATE,wxDefaultPosition,wxDefaultSize,items);
	Box->Add(night_mode_state_choice,0,wxGROW | wxALL,5);

	wxStaticText *night_mode_frame_rate_lable = new wxStaticText(this,wxID_STATIC,wxT("&Night Mode Frame Rate¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(night_mode_frame_rate_lable,0,wxALIGN_LEFT | wxALL,5);

	items.Clear();
	items.Add(wxT("1/2"));
	items.Add(wxT("1/4"));
	items.Add(wxT("1/8"));
	wxChoice *night_mode_frame_rate_choice = new wxChoice(this,ID_SET_CAMERA_NIGHT_MODE_FRAME_RATE,wxDefaultPosition,wxDefaultSize,items);
	Box->Add(night_mode_frame_rate_choice,0,wxGROW | wxALL,5);

	wxStaticText *auto_gain_control_lable = new wxStaticText(this,wxID_STATIC,wxT("&Auto Gain Control¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(auto_gain_control_lable,0,wxALIGN_LEFT | wxALL,5);

	items.Clear();
	items.Add(wxT("OFF"));
	items.Add(wxT("ON"));
	wxChoice *auto_gain_control_choice = new wxChoice(this,ID_SET_CAMERA_AUTO_GAIN_CONTRL,wxDefaultPosition,wxDefaultSize,items);
	Box->Add(auto_gain_control_choice,0,wxGROW | wxALL,5);

	wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
	Box->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	wxButton *reset = new wxButton(this,ID_SET_CAMERA_RESET,wxT("&Reset"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(reset,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool SetCameraDialog::TransferDataToWindow()
{
	wxSpinCtrl *frame_rate_spin = (wxSpinCtrl*)FindWindow(ID_SET_CAMERA_FRAME_RATE);
	wxChoice *resolution_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_RESOLUTION);
	wxChoice *quality_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_QUALITY);
	wxSlider *brightness_slider = (wxSlider*)FindWindow(ID_SET_CAMERA_BRIGHTNESS);
	wxChoice *frequency_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_FREQUENCY);
	wxChoice *night_mode_state_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_NIGHT_MODE_STATE);
	wxChoice *night_mode_frame_rate_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_NIGHT_MODE_FRAME_RATE); 
	wxChoice *auto_gain_control_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_AUTO_GAIN_CONTRL);

	frame_rate_spin->SetValue(frame_rate);
	quality_choice->SetSelection(quality);
	resolution_choice->SetSelection(resolution);
	brightness_slider->SetValue(brightness);
	frequency_choice->SetSelection(frequency);
	night_mode_state_choice->SetSelection(night_mode_state);
	night_mode_frame_rate_choice->SetSelection(night_mode_frame_rate);
	auto_gain_control_choice->SetSelection(auto_gain_contrl_state);

	return true;
}

bool SetCameraDialog::TransferDataFromWindow()
{
	wxSpinCtrl *frame_rate_spin = (wxSpinCtrl*)FindWindow(ID_SET_CAMERA_FRAME_RATE);
	wxChoice *resolution_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_RESOLUTION);
	wxChoice *quality_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_QUALITY);
	wxSlider *brightness_slider = (wxSlider*)FindWindow(ID_SET_CAMERA_BRIGHTNESS);
	wxChoice *frequency_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_FREQUENCY);
	wxChoice *night_mode_state_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_NIGHT_MODE_STATE);
	wxChoice *night_mode_frame_rate_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_NIGHT_MODE_FRAME_RATE); 
	wxChoice *auto_gain_control_choice = (wxChoice*)FindWindow(ID_SET_CAMERA_AUTO_GAIN_CONTRL);

	frame_rate = frame_rate_spin->GetValue();
	quality = quality_choice->GetSelection();
	resolution = resolution_choice->GetSelection();
	brightness = brightness_slider->GetValue();
	frequency = frequency_choice->GetSelection();
	night_mode_state = night_mode_state_choice->GetSelection();
	night_mode_frame_rate = night_mode_frame_rate_choice->GetSelection();
	auto_gain_contrl_state = auto_gain_control_choice->GetSelection();

	return true;
}

void SetCameraDialog::OnReset(wxCommandEvent &event)
{
	Init();
	TransferDataToWindow();
}