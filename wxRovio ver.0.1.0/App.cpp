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

#include "App.h"

DECLARE_APP(App)
IMPLEMENT_APP(App)

bool App::OnInit()
{
	Frame *frame = new Frame(wxT("wxRovio"));

	frame->Show(true);

	return true;
}

Frame::Frame(const wxString &title):wxFrame(NULL,wxID_ANY,title,wxDefaultPosition,wxSize(600,750),wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU)
{
	wxInitAllImageHandlers();

	streaming = false;

	rovio = NULL;
	thread = NULL;
	video_writer = NULL;

	CreateUI();

	forward->Enable(false);
	backward->Enable(false);
	left->Enable(false);
	right->Enable(false);
	turn_left->Enable(false);
	turn_right->Enable(false);
	stop->Enable(false);
	low->Enable(false);
	mid->Enable(false);
	high->Enable(false);
}

Frame::~Frame()
{
	if(thread){
		thread->Delete();
		thread = NULL;
	}
	if(streaming){
		rovio->StopVideoStream();
	}
	if(video_writer){
		cvReleaseVideoWriter(&video_writer);
		video_writer = NULL;
	}
	if(rovio){
		delete rovio;
	}
}

void Frame::CreateUI()
{
	wxMenu *file = new wxMenu;
	file->Append(ID_LOGGING,wxT("Loggingt\tAlt-l"),wxT("logging"));
	file->Append(wxID_EXIT,wxT("E&xit\tAlt-q"),wxT("exit"));

	wxMenu *setting = new wxMenu;
	setting->Append(ID_SET_CAMERA,wxT("Set Camera\tAlt-c"),wxT("Set Camera"));
	setting->Append(ID_SET_OTHER,wxT("Set Other\tAlt-o"),wxT("Set Other"));

	wxMenu *tools = new wxMenu;
	wxMenu *home = new wxMenu;
	home->Append(ID_SET_HOME,wxT("Set Home\tCtrl-s"),wxT("Set Home"));
	home->Append(ID_GO_HOME,wxT("Go Home\tCtrl-g"),wxT("Go Home"));
	tools->AppendSubMenu(home,wxT("Home"),wxT("Home"));

	wxMenu *camera = new wxMenu;
	camera->Append(ID_FRAME_GET,wxT("Get Frame\tCtrl-p"),wxT("Get Frame"));
	camera->Append(ID_VIDEO_START,wxT("Video Start\tCtrl-v"),wxT("Video Start"));
	camera->Append(ID_VIDEO_STOP,wxT("Video Stop\tCtrl-b"),wxT("Video Stop"));
	tools->AppendSubMenu(camera,wxT("Camera"),wxT("Camera"));

	wxMenu *user_manager = new wxMenu;
	user_manager->Append(ID_ADD_USER,wxT("Add User\tCtrl-a"),wxT("Add User"));
	user_manager->Append(ID_DELETE_USER,wxT("Delete User\tCtrl-d"),wxT("Delete User"));
	user_manager->Append(ID_GET_USER,wxT("Get User\tCtrl-g"),wxT("Get User"));
	tools->AppendSubMenu(user_manager,wxT("User Manager"),wxT("User Manager"));

	tools->Append(ID_REBOOT,wxT("Reboot\tCtrl-r"),wxT("Reboot"));

	wxMenuBar *bar = new wxMenuBar;
	bar->Append(file,wxT("file"));
	bar->Append(setting,wxT("setting"));
	bar->Append(tools,wxT("tools"));
	SetMenuBar(bar);

	wxBoxSizer *top = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(top);

	wxBoxSizer *screen_box = new wxBoxSizer(wxHORIZONTAL);
	top->Add(screen_box,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);
	screen = new wxPanel(this,wxID_ANY,wxDefaultPosition,wxSize(600,470));
	screen_box->Add(screen,0,wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL,5);

	wxBoxSizer *button_box4 = new wxBoxSizer(wxHORIZONTAL);
	top->Add(button_box4,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);
	low = new ControlButton(MOVE_HEADDOWN,this,this,ID_TURN_LEFT,wxT("&Low"),wxDefaultPosition,wxDefaultSize);
	button_box4->Add(low,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	mid = new ControlButton(MOVE_HEADMIDDLE,this,this,ID_FORWARD,wxT("&Mid"),wxDefaultPosition,wxDefaultSize);
	button_box4->Add(mid,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	high = new ControlButton(MOVE_HEADUP,this,this,ID_TURN_RIGHT,wxT("&High"),wxDefaultPosition,wxDefaultSize);
	button_box4->Add(high,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxBoxSizer *button_box1 = new wxBoxSizer(wxHORIZONTAL);
	top->Add(button_box1,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);
	turn_left = new ControlButton(MOVE_TURNLEFT,this,this,ID_TURN_LEFT,wxT("&Turn Left"),wxDefaultPosition,wxDefaultSize);
	button_box1->Add(turn_left,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	forward = new ControlButton(MOVE_FORWARD,this,this,ID_FORWARD,wxT("&Forward"),wxDefaultPosition,wxDefaultSize);
	button_box1->Add(forward,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	turn_right = new ControlButton(MOVE_TURNRIGHT,this,this,ID_TURN_RIGHT,wxT("&Turn Right"),wxDefaultPosition,wxDefaultSize);
	button_box1->Add(turn_right,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxBoxSizer *button_box2 = new wxBoxSizer(wxHORIZONTAL);
	top->Add(button_box2,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);
	left = new ControlButton(MOVE_LEFT,this,this,ID_LEFT,wxT("&Left"),wxDefaultPosition,wxDefaultSize);
	button_box2->Add(left,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	stop = new wxButton(this,ID_STOP,wxT("&Stop"),wxDefaultPosition,wxDefaultSize);
	button_box2->Add(stop,0,wxALIGN_CENTER_VERTICAL | wxALL,5);
	right = new ControlButton(MOVE_RIGHT,this,this,ID_RIGHT,wxT("&Right"),wxDefaultPosition,wxDefaultSize);
	button_box2->Add(right,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	wxBoxSizer *button_box3 = new wxBoxSizer(wxHORIZONTAL);
	top->Add(button_box3,0,wxALIGN_CENTER_HORIZONTAL | wxALL,5);
	backward = new ControlButton(MOVE_BACKWARD,this,this,ID_BACKWARD,wxT("&Backward"),wxDefaultPosition,wxDefaultSize);
	button_box3->Add(backward,0,wxALIGN_CENTER_VERTICAL | wxALL,5);

	CreateStatusBar(2);
	SetStatusText(wxDateTime::Now().Format());
}

void Frame::OnStop(wxCommandEvent &event)
{
	forward->Stop();
	backward->Stop();
	left->Stop();
	right->Stop();
	turn_left->Stop();
	turn_right->Stop();
	low->Stop();
	mid->Stop();
	high->Stop();

	rovio->ManualDrive(MOVE_STOP);

	rovio->ResetNavStateMachine();
}

void Frame::OnGoHome(wxCommandEvent &event)
{
	rovio->GoHome();
}

void Frame::OnSetHome(wxCommandEvent &event)
{
	if(wxMessageBox(wxT("Set Home?"),wxT("Set Home"),wxYES_NO,this) == wxYES){
		rovio->SetHome();
	}
}
void Frame::OnAddUser(wxCommandEvent &event)
{
	AddUserDialog dlg(this,wxID_ANY,wxT("Add User"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

	if(dlg.ShowModal() == wxID_OK){
		rovio->AddUser(dlg.GetUsername(),dlg.GetPassword(),dlg.GetPrivilege());
	}
}

void Frame::OnDeleteUser(wxCommandEvent &event)
{
	DeleteUserDialog dlg(this,wxID_ANY,wxT("Delete User"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

	if(dlg.ShowModal() == wxID_OK){
		if(wxMessageBox(wxT("Delete User?"),wxT("Check"),wxYES_NO,this) == wxYES){
			rovio->DeleteUser(dlg.GetUsername());
		}
	}
}

void Frame::OnGetUser(wxCommandEvent &event)
{
	wxArrayString user_list = rovio->GetUser();
	wxString str;
	for(size_t i = 0;i < user_list.GetCount();++i){
		str += user_list.Item(i);
		str += wxT("\n");
	}
	wxMessageBox(str,wxT("User Info"),wxICON_INFORMATION,this);
}

void Frame::OnVideoStart(wxCommandEvent &event)
{
	if(video_writer == NULL){
		wxFileDialog dlg(this,wxT("Save Video"),wxT("."),wxT("*.avi"),wxT("JPEG files (*.avi)|*.avi"),wxSAVE | wxOVERWRITE_PROMPT);
		if(dlg.ShowModal() == wxID_OK){
			video_writer = cvCreateVideoWriter(dlg.GetPath().mb_str(),CV_FOURCC('P','I','M','1'),25,cvSize(640,480),1);
		}
	}
	else{
		wxMessageBox(wxT("Starting!"),wxT("Error"));
	}
}

void Frame::OnVideoStop(wxCommandEvent &event)
{
	if(video_writer){
		cvReleaseVideoWriter(&video_writer);
		video_writer = NULL;
	}
}

void Frame::OnSetCamera(wxCommandEvent &event)
{
	SetCameraDialog dlg(this,wxID_ANY,wxT("Set Camera"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
	dlg.SetAllValue(rovio->GetRovioState());

	if(dlg.ShowModal() == wxID_OK){
		rovio->SetFrameRate(dlg.GetFrameRate());
		rovio->SetQuality(dlg.GetQuality());
		rovio->SetResolution(dlg.GetResolution());
		rovio->SetBrightness(dlg.GetBrightness());
		rovio->SetFrequency(dlg.GetFrequency());
		rovio->SetNightMode(dlg.GetNightModeState(),dlg.GetNightModeFrameRate());
		rovio->SetAutoGainControl(dlg.GetAutoGainContrl());
	}
}

void Frame::OnSetOther(wxCommandEvent &event)
{
	SetOtherDialog dlg(this,wxID_ANY,wxT("Set Other"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);
	dlg.SetAllValue(rovio->GetRovioState());

	if(dlg.ShowModal() == wxID_OK){
		rovio->SetSpeed(dlg.GetSpeed());
		rovio->SetLight(dlg.GetLight());
	}
}

void Frame::OnReboot(wxCommandEvent &event)
{
	if(wxMessageBox(wxT("Reboot?"),wxT("Check"),wxYES_NO,this) == wxYES){
		rovio->Reboot();
	}
}

void Frame::OnExit(wxCommandEvent &event)
{
	Close();
}

void Frame::OnLogging(wxCommandEvent &event)
{
	LoggingDialog dlg(this,wxID_ANY,wxT("Logging"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE);

	if(dlg.ShowModal() == wxID_OK){
		if(streaming){
			streaming = false;
			rovio->StopVideoStream();
		}
		if(thread){
			thread->Delete();
			thread = NULL;
		}
		if(rovio){
			delete rovio;
			rovio = NULL;
		}

		rovio = new wxRovio(dlg.GetIP(),dlg.GetUsername(),dlg.GetPassword());

		if(rovio->IsConnected()){
			streaming = true;
			rovio->StartVideoStream();
			thread = new VideoStreamThread(this);
			thread->Create();
			thread->Run();

			forward->Enable(true);
			backward->Enable(true);
			left->Enable(true);
			right->Enable(true);
			turn_left->Enable(true);
			turn_right->Enable(true);
			stop->Enable(true);
			low->Enable(true);
			mid->Enable(true);
			high->Enable(true);
		}
		else{
			wxMessageBox(wxT("Logging Error"));

			delete rovio;
			rovio = NULL;

			forward->Enable(false);
			backward->Enable(false);
			left->Enable(false);
			right->Enable(false);
			turn_left->Enable(false);
			turn_right->Enable(false);
			stop->Enable(false);
			low->Enable(false);
			mid->Enable(false);
			high->Enable(false);
		}
	}
}

void Frame::OnGetFrame(wxCommandEvent &event)
{
	cv::Mat mat = rovio->GetImage();
	if(!mat.empty()){
		wxFileDialog dlg(this,wxT("Save Image"),wxT("."),wxT("*.jpg"),wxT("JPEG files (*.jpg)|*.jpg"),wxSAVE | wxOVERWRITE_PROMPT);
		if(dlg.ShowModal() == wxID_OK){
			cvSaveImage(dlg.GetPath().mb_str(),&IplImage(mat));
		}
	}
}

void* VideoStreamThread::Entry()
{
	while(!TestDestroy()){
		wxMutexGuiEnter();
		cv::Mat mat = frame->rovio->GetFrame();
		if(!mat.empty()){
			IplImage *temp = new IplImage(mat);

			if(frame->video_writer){
				cvWriteFrame(frame->video_writer,temp);
			}

			wxClientDC dc(frame->screen);
			cvConvertImage(temp,temp,CV_CVTIMG_SWAP_RB);
			unsigned char *data;
			cvGetRawData(temp,&data);
			wxImage *image = new wxImage(temp->width,temp->height,data,true);
			wxBitmap *bitmap = new wxBitmap(*image);
			int x,y,width,height;
			dc.GetClippingBox(&x,&y,&width,&height);
			dc.DrawBitmap(*bitmap,x,y);
			delete temp;
			delete image;
			delete bitmap;
		}
		wxMutexGuiLeave();
		Sleep(25);
	}

	return NULL;
}

void* MoveThread::Entry()
{
	while(!TestDestroy()){
		if((mode == MOVE_HEADUP) || (mode == MOVE_HEADDOWN) || (mode == MOVE_HEADMIDDLE)){
			frame->rovio->ManualDrive(mode);
			frame->rovio->ManualDrive(MOVE_STOP);
		}
		else{
			frame->rovio->ManualDrive(mode);
		}
	}
	return NULL;
}