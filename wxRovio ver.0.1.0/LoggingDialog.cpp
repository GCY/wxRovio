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

#include "LoggingDialog.h"

IMPLEMENT_CLASS(LoggingDialog,wxDialog)

BEGIN_EVENT_TABLE(LoggingDialog,wxDialog)
   EVT_BUTTON(ID_RESET,LoggingDialog::OnReset)
END_EVENT_TABLE()

LoggingDialog::LoggingDialog()
{
	Init();
}

LoggingDialog::LoggingDialog(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
{
	Init();
	Create(parent,id,caption,pos,size,style);
}

inline void LoggingDialog::Init()
{
}

bool LoggingDialog::Create(wxWindow *parent,wxWindowID id,const wxString &caption,const wxPoint &pos,const wxSize &size,long style)
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

void LoggingDialog::CreateControls()
{
	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(top);

	wxBoxSizer *TextCtrlBox = new wxBoxSizer(wxVERTICAL);
	top->Add(TextCtrlBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	wxStaticText *ip_lable = new wxStaticText(this,wxID_STATIC,wxT("&IP¡G"),wxDefaultPosition,wxDefaultSize,0);
	TextCtrlBox->Add(ip_lable,0,wxALIGN_LEFT | wxALL,5);

	wxTextCtrl *ip_ctrl = new wxTextCtrl(this,ID_IP,wxT("IP"),wxDefaultPosition,wxDefaultSize,0);
	TextCtrlBox->Add(ip_ctrl,0,wxGROW | wxALL,5);

	wxStaticText *username_lable = new wxStaticText(this,wxID_STATIC,wxT("&Username¡G"),wxDefaultPosition,wxDefaultSize,0);
	TextCtrlBox->Add(username_lable,0,wxALIGN_LEFT | wxALL,5);

	wxTextCtrl *username_ctrl = new wxTextCtrl(this,ID_USERNAME,wxT("Username"),wxDefaultPosition,wxDefaultSize,0);
	TextCtrlBox->Add(username_ctrl,0,wxGROW | wxALL,5);

	wxStaticText *password_lable = new wxStaticText(this,wxID_STATIC,wxT("&Password¡G"),wxDefaultPosition,wxDefaultSize,0);
	TextCtrlBox->Add(password_lable,0,wxALIGN_LEFT | wxALL,5);

	wxTextCtrl *password_ctrl = new wxTextCtrl(this,ID_PASSWORD,wxT("Password"),wxDefaultPosition,wxDefaultSize,wxTE_PASSWORD);
	TextCtrlBox->Add(password_ctrl,0,wxGROW | wxALL,5);

	wxBoxSizer *ResetOkCancelBox = new wxBoxSizer(wxHORIZONTAL);
	TextCtrlBox->Add(ResetOkCancelBox,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);

	wxButton *reset = new wxButton(this,ID_RESET,wxT("&Reset"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(reset,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxButton *ok = new wxButton(this,wxID_OK,wxT("&Ok"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(ok,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxButton *cancel = new wxButton(this,wxID_CANCEL,wxT("&Cancel"),wxDefaultPosition,wxDefaultSize,0);
	ResetOkCancelBox->Add(cancel,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
}

bool LoggingDialog::TransferDataToWindow()
{
	wxTextCtrl *ip_ctrl = (wxTextCtrl*)FindWindow(ID_IP);
	wxTextCtrl *username_ctrl = (wxTextCtrl*)FindWindow(ID_USERNAME);
	wxTextCtrl *password_ctrl = (wxTextCtrl*)FindWindow(ID_PASSWORD);

	ip_ctrl->SetValue(wxT(""));
	username_ctrl->SetValue(wxT(""));
	password_ctrl->SetValue(wxT(""));
	return true;
}

bool LoggingDialog::TransferDataFromWindow()
{
	wxTextCtrl *ip_ctrl = (wxTextCtrl*)FindWindow(ID_IP);
	wxTextCtrl *username_ctrl = (wxTextCtrl*)FindWindow(ID_USERNAME);
	wxTextCtrl *password_ctrl = (wxTextCtrl*)FindWindow(ID_PASSWORD);

	ip = ip_ctrl->GetValue();
	username = username_ctrl->GetValue();
	password = password_ctrl->GetValue();
	return true;
}

void LoggingDialog::OnReset(wxCommandEvent &event)
{
	Init();
	TransferDataToWindow();
}