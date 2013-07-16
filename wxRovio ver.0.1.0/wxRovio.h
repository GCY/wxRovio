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

#ifndef __ROVIO__
#define __ROVIO__

#ifdef linux
#include <unistd.h>
#include <pthread.h>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <wx/wx.h>
#include <wx/tokenzr.h>

#include <curl/curl.h>

#include <cv.h>
#include <highgui.h>

#ifdef linux
void* VideoStreamThreadPasser(void*);
#define strcpy_s(dest,len,src) (strcpy(dest,src))
#endif
#ifdef _WIN32
DWORD VideoStreamThreadPasser(LPVOID);
void* memmem(const void*,size_t,const void*,size_t);
#endif

size_t CURLNullWriteFunctionCallback(void*,size_t,size_t,void*);
size_t CURLWriteMemoryCallback(void*,size_t,size_t,void*);
size_t CURLWriteMemoryVideoFrameCallback(void*,size_t,size_t,void*);
void* CURLRealloc(void*,size_t);

const char VIDEO_STREAM_INTERLEAVE[] = "--WINBONDBOUDARY\r\nContent-Type: image/jpeg\r\n\r\n";
const int VIDEO_STREAM_INTERLEAVE_SIZE = 46;
const int VIDEO_STREAM_STOP_WAIT_TIME = 1000;
const int VIDEO_STREAM_SERVICE_WAIT_TIME = 30;
const int VIDEO_STREAM_BUFFER_MAX_SIZE = 51200; //50k max.
const int VIDEO_STREAM_BUFFER_MIN_SIZE = 3500; //10k min.

const int MAX_STREAM_SIZE = 255;
const int COMMUNICATION_TIMEOUT = 180;
const int HTTP_ALL_SUCCESS = 200;

const int ROVIO_JPEG_MIN_SIZE = 134;
const char JPEG_SOI_MARKER_FIRST_BYTE = 0xFF;
const char JPEG_SOI_MARKER_SECOND_BYTE = 0xD8;

const int MOVE_STOP =				0;
const int MOVE_FORWARD =			1;
const int MOVE_BACKWARD =			2;
const int MOVE_LEFT =				3;
const int MOVE_RIGHT =				4;
const int MOVE_TURNLEFT =			5;
const int MOVE_TURNRIGHT =			6;
const int MOVE_FORWARDLEFT =		7;
const int MOVE_FORWARDRIGHT =		8;
const int MOVE_BACKWARDLEFT =		9;
const int MOVE_BACKWARDRIGHT =		10;
const int MOVE_HEADUP =				11;
const int MOVE_HEADDOWN =			12;
const int MOVE_HEADMIDDLE =			13;
const int MOVE_ROTATELEFT20 =		17;
const int MOVE_ROTATERIGHT20 =		18;

typedef struct
{
    char *memory;
    size_t size;
}RovioMemory;

typedef struct
{
    char *memory;
    size_t size;
#ifdef linux
        pthread_mutex_t completed;
#endif
#ifdef _WIN32
        HANDLE completed;
#endif
    cv::Mat image;
    char *buffer;
	size_t buffer_size;
}RovioVideoStreamMemory;

typedef struct
{
	int speed;
	unsigned char light;

	int frame_rate;
	int quality;
	int resolution;
	int brightness;
	int frequency;
	bool night_mode_state;
	int night_mode_frame_rate;
	bool auto_gain_contrl_state;
}RovioState;

class wxRovio
{
public:
	wxRovio(const wxString&,const wxString&,const wxString&);

	RovioState GetRovioState();

	int SendStream(const wxString&);
	int SendStream(CURL *,const wxString&);

	bool IsConnected();

	void SetSpeed(int);
	bool ManualDrive(int,int = 0);

	bool SetLight(unsigned char);

	bool ResetNavStateMachine();

	bool Reboot();

	bool SetHome();
	bool GoHome();

	bool AddUser(const wxString&,const wxString&,bool);
	bool DeleteUser(const wxString&);
	wxArrayString GetUser();

	bool SetFrameRate(int);
	bool SetResolution(int);
	bool SetBrightness(int);
	bool SetQuality(int);
	bool SetNightMode(bool,int);
	bool SetFrequency(int);
	bool SetAutoGainControl(bool);

	void StartVideoStream();
	void StopVideoStream();
	void VideoStreamThread();
	cv::Mat GetImage();
	cv::Mat GetFrame();

	void SetAllDefault();

	void ClearBuffer();
	void ClearFrame();

#ifdef linux
    friend void* VideoStreamThreadPasser(void*);
#endif
#ifdef _WIN32
	friend DWORD VideoStreamThreadPasser(LPVOID);
#endif
private:
	CURL *curl_command;
	CURL *curl_video;

	wxString base_stream;

	RovioState rovio_state;
	RovioMemory rovio_buffer;
	RovioVideoStreamMemory frame;

#ifdef linux
	pthread_t video_stream_handle;
#endif
#ifdef _WIN32
	HANDLE video_stream_handle;
#endif
};

#endif