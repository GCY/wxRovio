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

#include "SetOtherDialog.h"

#include "SetOtherDialog.h"

IMPLEMENT_CLASS(SetOtherDialog,wxDialog)

BEGIN_EVENT_TABLE(SetOtherDialog,wxDialog)
   EVT_BUTTON(ID_SET_OTHER_RESET,SetOtherDialog::OnReset)
END_EVENT_TABLE()

SetOtherDialog::SetOtherDialog()
{
	Init();
}

SetOtherDialog::SetOtherDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
	Init();
	Create(parent,id,caption,pos,size,style);
}

inline void SetOtherDialog::Init()
{
}

void SetOtherDialog::SetAllValue(RovioState rovio_state)
{
	speed = rovio_state.speed;
	light = rovio_state.light;
}

bool SetOtherDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
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

void SetOtherDialog::CreateControls()
{
	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(top);

	wxBoxSizer *Box = new wxBoxSizer(wxVERTICAL);
	top->Add(Box,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	wxStaticText *speed_lable = new wxStaticText(this,wxID_STATIC,wxT("&Speed¡G"),wxDefaultPosition,wxDefaultSize,0);
	Box->Add(speed_lable,0,wxALIGN_LEFT | wxALL,5);

	wxSlider *speed_slider = new wxSlider(this,ID_SET_OTHER_SPEED,4,1,10,wxDefaultPosition,wxSize(200,-1),wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
	Box->Add(speed_slider,0,wxGROW | wxALL,5);

	wxArrayString items;
	items.Add(wxT("OFF"));
	items.Add(wxT("ON"));

	wxArrayString texts;
	texts.Add(wxT("&Light A"));
	texts.Add(wxT("&Light B"));
	texts.Add(wxT("&Light C"));
	texts.Add(wxT("&Light D"));
	texts.Add(wxT("&Light E"));
	texts.Add(wxT("&Light F"));

	wxStaticText *light_lable[6];
	wxChoice *light_choice[6];
	for(int i = 0;i < 6;++i){
		light_lable[i] = new wxStaticText(this,wxID_STATIC,texts.Item(i),wxDefaultPosition,wxDefaultSize,0);
		Box->Add(light_lable[i],0,wxALIGN_LEFT | wxALL,5);
		light_choice[i] = new wxChoice(this,ID_SET_OTHER_LIGHT_A + i,wxDefaultPosition,wxDefaultSize,items);
		Box->Add(light_choice[i],0,wxGROW | wxALL,5);
	}

	wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
	Box->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	wxButton *reset = new wxButton(this,ID_SET_OTHER_RESET,wxT("&Reset"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(reset,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool SetOtherDialog::TransferDataToWindow()
{
	wxSlider *speed_slider = (wxSlider*)FindWindow(ID_SET_OTHER_SPEED);

	wxChoice *light_choice[6];
	for(int i = 0;i < 6;++i){
		light_choice[i] = (wxChoice*)FindWindow(ID_SET_OTHER_LIGHT_A + i);
	}

	speed_slider->SetValue(speed);

	if(light & 1){light_choice[0]->SetSelection(1);}else{light_choice[0]->SetSelection(0);}
	if(light & 2){light_choice[1]->SetSelection(1);}else{light_choice[1]->SetSelection(0);}
	if(light & 4){light_choice[2]->SetSelection(1);}else{light_choice[2]->SetSelection(0);}
	if(light & 8){light_choice[3]->SetSelection(1);}else{light_choice[3]->SetSelection(0);}
	if(light & 16){light_choice[4]->SetSelection(1);}else{light_choice[4]->SetSelection(0);}
	if(light & 32){light_choice[5]->SetSelection(1);}else{light_choice[5]->SetSelection(0);}

	return true;
}

bool SetOtherDialog::TransferDataFromWindow()
{

	wxSlider *speed_slider = (wxSlider*)FindWindow(ID_SET_OTHER_SPEED);

	wxChoice *light_choice[6];
	for(int i = 0;i < 6;++i){
		light_choice[i] = (wxChoice*)FindWindow(ID_SET_OTHER_LIGHT_A + i);
	}

	speed = speed_slider->GetValue();

	unsigned char total = 0;
	if(light_choice[0]->GetSelection() & 1){total += 1;}
	if(light_choice[1]->GetSelection() & 1){total += 2;}
	if(light_choice[2]->GetSelection() & 1){total += 4;}
	if(light_choice[3]->GetSelection() & 1){total += 8;}
	if(light_choice[4]->GetSelection() & 1){total += 16;}
	if(light_choice[5]->GetSelection() & 1){total += 32;}
	light = total;

	return true;
}

void SetOtherDialog::OnReset(wxCommandEvent &event)
{
	Init();
	TransferDataToWindow();
}