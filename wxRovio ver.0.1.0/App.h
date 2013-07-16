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

#ifndef __APP__
#define __APP__

#include <wx/wx.h>

#include "wxRovio.h"
#include "LoggingDialog.h"
#include "AddUserDialog.h"
#include "DeleteUserDialog.h"
#include "SetCameraDialog.h"
#include "SetOtherDialog.h"

class VideoStreamThread;
class MoveThread;
class ControlButton;

enum
{
	ID_LOGGING = 100,

	ID_SET_HOME,
	ID_GO_HOME,

	ID_REBOOT,

	ID_ADD_USER,
	ID_DELETE_USER,
	ID_GET_USER,

	ID_FRAME_GET,
	ID_VIDEO_START,
	ID_VIDEO_STOP,

	ID_SET_CAMERA,
	ID_SET_OTHER,
	

	ID_SCREEN,
	ID_FORWARD,
	ID_BACKWARD,
	ID_LEFT,
	ID_RIGHT,
	ID_STOP,
	ID_TURN_LEFT,
	ID_TURN_RIGHT,
	ID_LOW,
	ID_MID,
	ID_HIGH
};

class App:public wxApp
{
public:
	bool OnInit();
private:
};

class Frame:public wxFrame
{
public:
	Frame(const wxString&);
	~Frame();

	void CreateUI();

	void OnExit(wxCommandEvent&);

	void OnStop(wxCommandEvent&);

	void OnReboot(wxCommandEvent&);

	void OnGoHome(wxCommandEvent&);
	void OnSetHome(wxCommandEvent&);

	void OnAddUser(wxCommandEvent&);
	void OnDeleteUser(wxCommandEvent&);
	void OnGetUser(wxCommandEvent&);

	void OnGetFrame(wxCommandEvent&);
	void OnVideoStart(wxCommandEvent&);
	void OnVideoStop(wxCommandEvent&);

	void OnSetCamera(wxCommandEvent&);
	void OnSetOther(wxCommandEvent&);

	void OnLogging(wxCommandEvent&);

	friend class VideoStreamThread;
	friend class MoveThread;
	friend class ControlButton;
private:
	wxRovio *rovio;

	wxPanel *screen;
	ControlButton *forward;
	ControlButton *backward;
	ControlButton *left;
	ControlButton *right;
	ControlButton *turn_left;
	ControlButton *turn_right;
	ControlButton *low;
	ControlButton *mid;
	ControlButton *high;
	wxButton *stop;

	bool streaming;
	VideoStreamThread *thread;

	CvVideoWriter *video_writer;

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(Frame,wxFrame)
	EVT_BUTTON(ID_STOP,Frame::OnStop)

	EVT_MENU(wxID_EXIT,Frame::OnExit)

	EVT_MENU(ID_REBOOT,Frame::OnReboot)

	EVT_MENU(ID_SET_HOME,Frame::OnSetHome)
	EVT_MENU(ID_GO_HOME,Frame::OnGoHome)

	EVT_MENU(ID_ADD_USER,Frame::OnAddUser)
	EVT_MENU(ID_DELETE_USER,Frame::OnDeleteUser)
	EVT_MENU(ID_GET_USER,Frame::OnGetUser)

	EVT_MENU(ID_FRAME_GET,Frame::OnGetFrame)
	EVT_MENU(ID_VIDEO_START,Frame::OnVideoStart)
	EVT_MENU(ID_VIDEO_STOP,Frame::OnVideoStop)

	EVT_MENU(ID_SET_CAMERA,Frame::OnSetCamera)
	EVT_MENU(ID_SET_OTHER,Frame::OnSetOther)
	
	EVT_MENU(ID_LOGGING,Frame::OnLogging)
END_EVENT_TABLE()

/**************************************************************************************************************************/

class VideoStreamThread:public wxThread
{
public:
	VideoStreamThread(Frame *parent):wxThread(wxTHREAD_DETACHED){
		frame = parent;
	};
	~VideoStreamThread(){};

	void* Entry();
private:
	Frame *frame;
};

class MoveThread:public wxThread
{
public:
	MoveThread(Frame *parent,int mode):wxThread(wxTHREAD_DETACHED){
		frame = parent;
		this->mode = mode;
	};
	~MoveThread(){};

	void* Entry();
private:
	Frame *frame;
	int mode;
};

/**************************************************************************************************************************/

class ControlButton:public wxButton
{
public:
	ControlButton(const int mode,Frame *frame,wxWindow *parent,wxWindowID id,const wxString &label,const wxPoint &pos, const wxSize &size):wxButton(parent,id,label,pos,size)
	{
		this->frame = frame;
		this->mode = mode;
		thread = new MoveThread(frame,mode);
		thread->Create();
		thread->Run();
		thread->Pause();
	}
	~ControlButton()
	{
	}
	void OnLeftDown(wxMouseEvent &event)
	{
		thread->Resume();
	}
	void OnLeftUp(wxMouseEvent &event)
	{
		thread->Pause();
	}
	void Stop()
	{
		if(!thread->IsPaused()){
			thread->Pause();
		}
	}
private:
	Frame *frame;
	int mode;
	MoveThread *thread;
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(ControlButton,wxButton)
	EVT_LEFT_DOWN(ControlButton::OnLeftDown)
	EVT_LEFT_UP(ControlButton::OnLeftUp)
END_EVENT_TABLE()

#endif