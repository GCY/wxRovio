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

#include "wxRovio.h"

wxRovio::wxRovio(const wxString &ip, const wxString &username, const wxString &password)
{
	base_stream.Printf(wxT("http://%s:%s@%s/"),username.c_str(),password.c_str(),ip.c_str());

	curl_command = curl_easy_init();

    curl_easy_setopt(curl_command,CURLOPT_VERBOSE,0);
    curl_easy_setopt(curl_command,CURLOPT_NOPROGRESS,1);
    curl_easy_setopt(curl_command,CURLOPT_TIMEOUT,COMMUNICATION_TIMEOUT);

	curl_video = curl_easy_init();

    curl_easy_setopt(curl_video,CURLOPT_VERBOSE,0);
    curl_easy_setopt(curl_video,CURLOPT_NOPROGRESS,1);
    curl_easy_setopt(curl_video,CURLOPT_TIMEOUT,COMMUNICATION_TIMEOUT);

	SetAllDefault();

	rovio_buffer.memory = NULL;
	rovio_buffer.size = 0;

    frame.memory = NULL;
    frame.size = 0;
	frame.buffer = NULL;
	frame.buffer_size = 0;
#ifdef linux
	pthread_mutex_init(&(frame.completed),NULL);
#endif
#ifdef _WIN32
	frame.completed = CreateMutex(NULL, FALSE, NULL);
#endif
}

RovioState wxRovio::GetRovioState()
{
	return rovio_state;
}

int wxRovio::SendStream(const wxString &curl_stream)
{
	return SendStream(curl_command,curl_stream);
}

int wxRovio::SendStream(CURL *curl_object,const wxString &curl_stream)
{
	long rovio_return_code;
	CURLcode response;

	wxString send_stream(base_stream);
	send_stream.Append(curl_stream);

	char send_buffer[1024] = {0};
	strcpy_s(send_buffer,1024,send_stream.mb_str());

	curl_easy_setopt(curl_object,CURLOPT_URL,send_buffer);

	response = curl_easy_perform(curl_object);

	response = curl_easy_getinfo(curl_object, CURLINFO_RESPONSE_CODE, &rovio_return_code);

	return (int)rovio_return_code;
}

bool wxRovio::IsConnected()
{
	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

    if (SendStream(wxT("")) == HTTP_ALL_SUCCESS)
    {
        return true;
    }
    return false;
}

void wxRovio::SetSpeed(int speed)
{
	rovio_state.speed = speed;
}

bool wxRovio::ManualDrive(int command,int angle)
{
	bool ret = false;

	wxString send_stream;
	send_stream.Printf(wxT("rev.cgi?Cmd=nav&action=18&drive=%d&speed=%d&angle=%d"),command,rovio_state.speed,angle);

    curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLWriteMemoryCallback);
    curl_easy_setopt(curl_command,CURLOPT_WRITEDATA,(void*)&rovio_buffer);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		ret = true;
	}

	ClearBuffer();
	return ret;
}

bool wxRovio::SetLight(unsigned char light)
{
	rovio_state.light = light;

	char first_byte = 0,second_byte = 0;
	if(light & 1){first_byte += 2;}
	if(light & 2){first_byte += 1;}
	if(light & 4){second_byte += 8;}
	if(light & 8){second_byte += 1;}
	if(light & 16){second_byte += 2;}
	if(light & 32){second_byte += 4;}

	wxString send_stream;
	send_stream.Printf(wxT("mcu?parameters=114D4D00010053485254000100011A%x%x0000"),first_byte,second_byte);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::ResetNavStateMachine()
{
	wxString send_stream;
	send_stream.Printf(wxT("rev.cgi?Cmd=nav&action=17"));

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetFrameRate(int level)
{

	if((level < 2) || (level > 32)){
		return false;
	}

	rovio_state.frame_rate = level;

	wxString send_stream;
	send_stream.Printf(wxT("ChangeFramerate.cgi?Framerate=%d"),rovio_state.frame_rate);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetResolution(int level)
{
	//	0	=	176 x 144
	//	1	=	320 x 240
	//	2	=	352 x 288
	//	3	=	640 x 480

	if((level < 0) || (level > 3)){
		return false;
	}

	rovio_state.resolution = level;

	wxString send_stream;
	send_stream.Printf(wxT("ChangeResolution.cgi?ResType=%d"),rovio_state.resolution);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetBrightness(int level)
{
	//	Low : 0	~	High : 6

	if((level < 0) || (level > 6)){
		return false;
	}

	rovio_state.brightness = level;

	wxString send_stream;
	send_stream.Printf(wxT("ChangeBrightness.cgi?Brightness=%d"),rovio_state.brightness);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetQuality(int level)
{
	//	Low : 0	~	High : 2

	if((level < 0) || (level > 2)){
		return false;
	}

	rovio_state.quality = level;

	wxString send_stream;
	send_stream.Printf(wxT("ChangeCompressRatio.cgi?Ratio=%d"),rovio_state.quality);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetNightMode(bool state,int frame_rate)
{
	wxString send_stream;
	wxString send_stream2;

	switch(frame_rate){
		case 2:// Framerate/2
			{
				rovio_state.night_mode_frame_rate = frame_rate;
				rovio_state.night_mode_state = state;
				send_stream2.Printf(wxT("debug.cgi?action=write_i2c&address=0x3b&value=0xa2"));
				break;
			}
		case 4:// Framerate/4
			{
				rovio_state.night_mode_frame_rate = frame_rate;
				rovio_state.night_mode_state = state;
				send_stream2.Printf(wxT("debug.cgi?action=write_i2c&address=0x3b&value=0xc2"));
				break;
			}
		case 8:// Framerate/8
			{
				rovio_state.night_mode_frame_rate = frame_rate;
				rovio_state.night_mode_state = state;
				send_stream2.Printf(wxT("debug.cgi?action=write_i2c&address=0x3b&value=0xe2"));
				break;
			}
		default:
			{
				if(state){
					return false;
				}
				break;
			}
	}

	if(!state){
		send_stream2.Printf(wxT("debug.cgi?action=write_i2c&address=0x3b&value=0x82"));
	}

	send_stream.Printf(wxT("debug.cgi?action=write_i2c&address=0x3b&value=0x02"));

	if((SendStream(send_stream) == HTTP_ALL_SUCCESS) &&
		(SendStream(send_stream2) == HTTP_ALL_SUCCESS)){
		return true;
	}

	return false;
}

bool wxRovio::SetFrequency(int level)
{
	//	0	=	Auto
	//	50	=	50Hz
	//	60	=	60Hz

	if((level != 0) || (level != 50) || (level != 60)){
		return false;
	}

	rovio_state.frequency = level;

	wxString send_stream;
	send_stream.Printf(wxT("SetCamera.cgi?Frequency=%d"),rovio_state.frequency);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetAutoGainControl(bool state)
{
	//http://.../debug.cgi?action=write_i2c&address=0x14&value=0xn8
	//where n = 0,1,2,....,6

	rovio_state.auto_gain_contrl_state = state;

	wxString send_stream;

	if(rovio_state.auto_gain_contrl_state){
		send_stream.Printf(wxT("debug.cgi?action=write_i2c&address=0x14&value=0x68"));
	}
	else{
		send_stream.Printf(wxT("debug.cgi?action=write_i2c&address=0x14&value=0x18"));
	}

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::SetHome()
{
	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(wxT("rev.cgi?Cmd=nav&action=14")) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::GoHome()
{
	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(wxT("rev.cgi?Cmd=nav&action=13")) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::AddUser(const wxString &username,const wxString &password,bool privilege)
{
	//Privilege = 0(user) or 1(admin)
	if(username.empty() || password.empty()){
		return false;
	}

	wxString send_stream;
	send_stream.Printf(wxT("SetUser.cgi?User=%s&Pass=%s&Privilege=%i"),username.c_str(),password.c_str(),privilege);

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

bool wxRovio::DeleteUser(const wxString &username)
{
	if(username.empty()){
		return false;
	}

	wxString send_stream;
	send_stream.Printf(wxT("DelUser.cgi?User=%s"),username.c_str());

	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(send_stream) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

wxArrayString wxRovio::GetUser()
{
	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLWriteMemoryCallback);
	curl_easy_setopt(curl_command,CURLOPT_WRITEDATA,(void*)&rovio_buffer);

	if(SendStream(wxT("GetUser.cgi?ShowPrivilege=true")) == HTTP_ALL_SUCCESS){

		wxString src(rovio_buffer.memory,wxConvUTF8);
		wxArrayString temp_list;
		wxStringTokenizer split_name(src,wxT("\n"));
		while(split_name.HasMoreTokens()){
			temp_list.Add(split_name.GetNextToken());;
		}

		wxArrayString return_list;
		for(size_t i = 0;i < (temp_list.GetCount() / 2);++i){
			wxString str;
			str.Printf(wxT("%s\t%s"),temp_list.Item(i).c_str(),temp_list.Item(i + (temp_list.GetCount() / 2)).c_str());
			return_list.Add(str);
		}
		ClearBuffer();

		return return_list;
	}

	ClearBuffer();

	return wxArrayString();
}

bool wxRovio::Reboot()
{
	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLNullWriteFunctionCallback);

	if(SendStream(wxT("Reboot.cgi")) == HTTP_ALL_SUCCESS){
		return true;
	}

	return false;
}

void wxRovio::SetAllDefault()
{
	rovio_state.speed = 6;
	rovio_state.light = 0;

	rovio_state.brightness = 6;
	rovio_state.frame_rate = 25;
	rovio_state.frequency = 0;
	rovio_state.night_mode_frame_rate = 4;
	rovio_state.night_mode_state = false;
	rovio_state.quality = 2;
	rovio_state.resolution = 3;
	rovio_state.auto_gain_contrl_state = false;

	SetLight(rovio_state.light);

	SetFrameRate(rovio_state.frame_rate);
	SetResolution(rovio_state.resolution);
	SetBrightness(rovio_state.brightness);
	SetQuality(rovio_state.quality);
	SetNightMode(rovio_state.night_mode_state,rovio_state.night_mode_frame_rate);
	SetFrequency(rovio_state.frequency);
	SetAutoGainControl(rovio_state.auto_gain_contrl_state);

}

/**************************************************************************************************************************/

cv::Mat wxRovio::GetImage()
{
	curl_easy_setopt(curl_command,CURLOPT_WRITEFUNCTION,CURLWriteMemoryCallback);
	curl_easy_setopt(curl_command,CURLOPT_WRITEDATA,(void*)&rovio_buffer);

	if(SendStream(wxT("Jpeg/CamImg1234.jpg")) == HTTP_ALL_SUCCESS){
	}

	cv::Mat return_data;
    if ((rovio_buffer.size >= ROVIO_JPEG_MIN_SIZE) &&
		(rovio_buffer.memory[0] & JPEG_SOI_MARKER_FIRST_BYTE) &&
		(rovio_buffer.memory[1] & JPEG_SOI_MARKER_SECOND_BYTE)){
			return_data = cv::imdecode(cv::Mat(1,rovio_buffer.size,CV_8UC1,rovio_buffer.memory),CV_LOAD_IMAGE_UNCHANGED).clone();
    }

	ClearBuffer();

	return return_data;
}

void wxRovio::StartVideoStream()
{
#ifdef linux
	if(pthread_create(&(video_stream_handle),NULL,VideoStreamThreadPasser,(void*)this)){}
#endif
#ifdef _WIN32
	video_stream_handle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)VideoStreamThreadPasser,(LPVOID)this,0,NULL);
#endif
}

void wxRovio::StopVideoStream()
{
#ifdef linux
	if(pthread_cancel(video_stream_handle))
#endif
#ifdef _WIN32
	TerminateThread(video_stream_handle, 0);
	CloseHandle(video_stream_handle);
#endif

	ClearFrame();
}

void wxRovio::VideoStreamThread()
{
	curl_easy_setopt(curl_video,CURLOPT_WRITEFUNCTION,CURLWriteMemoryVideoFrameCallback);
	curl_easy_setopt(curl_video,CURLOPT_WRITEDATA,(void*) &frame);

	if(SendStream(curl_video,wxT("GetData.cgi?Status=false")) == HTTP_ALL_SUCCESS){
	}
}

cv::Mat wxRovio::GetFrame()
{
	cv::Mat return_data;

#ifdef linux
    pthread_mutex_lock( &(frame.completed));
    return_data = frame.image.clone();
    pthread_mutex_unlock( &(frame.completed));
#endif
#ifdef _WIN32
    WaitForSingleObject(frame.completed, INFINITE);
    return_data = frame.image.clone();
    ReleaseMutex(frame.completed);
#endif

    return return_data;
}

/**************************************************************************************************************************/

void wxRovio::ClearBuffer()
{
	if((rovio_buffer.size > 0) && (rovio_buffer.memory != NULL)){
		free(rovio_buffer.memory);
		rovio_buffer.memory = NULL;
		rovio_buffer.size = 0;
	}
}

void wxRovio::ClearFrame()
{
    if ((frame.size > 0) && (frame.memory != NULL)){
        frame.size = 0;
        free(frame.memory);
        frame.memory = NULL;
    }

    if ((frame.buffer_size > 0) && (frame.buffer != NULL)){
        frame.buffer_size = 0;
        free(frame.buffer);
        frame.buffer = NULL;
    }
}

/**************************************************************************************************************************/

#ifdef linux
void* VideoStreamThreadPasser(void* args)
{
    wxRovio *rovio = (wxRovio*)args;
    while(true)
    {
        rovio->VideoStreamThread();
        usleep(VIDEO_STREAM_SERVICE_WAIT_TIME * 1000); // assume ~30 fps max.
    }
    sleep(VIDEO_STREAM_STOP_WAIT_TIME);
    return 0;
}
#endif
#ifdef _WIN32
DWORD VideoStreamThreadPasser(LPVOID lpParameter) {
	wxRovio *rovio = (wxRovio*)lpParameter;
    while(true)
    {
        rovio->VideoStreamThread();
        Sleep(VIDEO_STREAM_SERVICE_WAIT_TIME); // assume ~30 fps max.
    }
    Sleep(VIDEO_STREAM_STOP_WAIT_TIME);
    return 0;
}
#endif

/**************************************************************************************************************************/
#ifdef _WIN32
void* memmem(const void *buf,size_t buf_len,const void *byte_sequence,size_t byte_sequence_len)
{

	BYTE *bf = (BYTE *)buf;
	BYTE *bs = (BYTE *)byte_sequence;
	BYTE *p  = bf;

	while (byte_sequence_len <= (buf_len - (p - bf))){
		UINT b = *bs & 0xFF;
		if ((p = (BYTE *) memchr(p, b, buf_len - (p - bf))) != NULL){
			if ((memcmp(p, byte_sequence, byte_sequence_len)) == 0){
				return p;
			}
			else{
				p++;
			}
		}
		else{
			break;
		}
	}
	return NULL;
}
#endif

size_t CURLNullWriteFunctionCallback(void *ptr,size_t size,size_t nmemb,void *stream)
{
	return (nmemb * size);
}

size_t CURLWriteMemoryCallback (void *ptr,size_t size,size_t nmemb,void *data)
{
    size_t realsize = size * nmemb;
    RovioMemory *mem = (RovioMemory*)data;

    mem->memory = (char*)CURLRealloc(mem->memory,mem->size + realsize + 1);
    if (mem->memory){
        memcpy(&(mem->memory[mem->size]),ptr,realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}

//Refrance by https://github.com/tobybreckon/roviolib
size_t CURLWriteMemoryVideoFrameCallback(void *ptr,size_t size,size_t nmemb,void *data)
{
	if(!ptr){return 0;}

    size_t realsize = size * nmemb;
    RovioVideoStreamMemory *frame = (RovioVideoStreamMemory*)data;

	void *buffer_temp;
	size_t buffer_size_temp;

	if(realsize < VIDEO_STREAM_INTERLEAVE_SIZE){
		frame->buffer = (char*)realloc(frame->buffer, frame->buffer_size + realsize);
		memcpy(&(frame->buffer[frame->buffer_size]),ptr,realsize);
		frame->buffer_size += realsize;

		return realsize;
	}

	if(frame->buffer){
		frame->buffer = (char*)realloc(frame->buffer, frame->buffer_size + realsize);
		memcpy(&(frame->buffer[frame->buffer_size]),ptr,realsize);
		frame->buffer_size += realsize;

		buffer_temp = frame->buffer;
		buffer_size_temp = frame->buffer_size;
	}
	else{
		buffer_temp = ptr;
		buffer_size_temp = realsize;
	}

	char *interleave = (char*)memmem((char*)buffer_temp,buffer_size_temp,VIDEO_STREAM_INTERLEAVE,VIDEO_STREAM_INTERLEAVE_SIZE);

	if((interleave) && (frame->size < VIDEO_STREAM_BUFFER_MAX_SIZE)){
		size_t beforeinterleave = (interleave - (char*)buffer_temp);
		frame->memory = (char*)CURLRealloc(frame->memory,(frame->size) + beforeinterleave + 1);
		if(!frame->memory){
			frame->size = 0;
			return 0;
		}

		memcpy(&(frame->memory[frame->size]),buffer_temp,beforeinterleave);
		frame->size +=  beforeinterleave;
		frame->memory[frame->size] = 0;

		char *p_interleave = (char*)memmem((char*)frame->memory,frame->size,VIDEO_STREAM_INTERLEAVE,VIDEO_STREAM_INTERLEAVE_SIZE);

		if(p_interleave){
			size_t img_size = frame->size - (p_interleave - frame->memory) - VIDEO_STREAM_INTERLEAVE_SIZE;

			if((img_size >= ROVIO_JPEG_MIN_SIZE &&
				img_size >= (VIDEO_STREAM_INTERLEAVE_SIZE + VIDEO_STREAM_BUFFER_MIN_SIZE) &&
				(p_interleave[VIDEO_STREAM_INTERLEAVE_SIZE] & JPEG_SOI_MARKER_FIRST_BYTE) &&
				(p_interleave[VIDEO_STREAM_INTERLEAVE_SIZE + 1] & JPEG_SOI_MARKER_SECOND_BYTE))){
					#ifdef linux
					pthread_mutex_lock(&(frame->completed));
					#endif
					#ifdef _WIN32
					WaitForSingleObject(frame->completed, INFINITE);
					#endif

					frame->image = cv::Mat(1, img_size, CV_8UC1, &(p_interleave[VIDEO_STREAM_INTERLEAVE_SIZE]));
					frame->image = cv::imdecode(frame->image, CV_LOAD_IMAGE_UNCHANGED);

					#ifdef linux
					pthread_mutex_unlock(&(frame->completed));
					#endif
					#ifdef _WIN32
					ReleaseMutex(frame->completed);
					#endif
			}
		}

		if(frame->memory){
			free(frame->memory);
		}
		frame->memory = NULL;

		size_t frominterleave = buffer_size_temp - beforeinterleave;
		frame->memory = (char*)CURLRealloc(frame->memory,frominterleave + 1);

        if(!frame->memory){
            frame->size = 0;
            return 0;
        }

		memcpy(frame->memory,interleave,frominterleave);
        frame->size = frominterleave;
        frame->memory[frame->size] = 0;
	}
	else{
		if(frame->size > VIDEO_STREAM_BUFFER_MAX_SIZE){
			frame->size = 0;

            if (frame->memory){
				free(frame->memory);
			}
            frame->memory = NULL;
		}
		
		frame->memory = (char*)CURLRealloc(frame->memory,(frame->size) + buffer_size_temp + 1);
		if(!frame->memory){
			frame->size = 0;
			return 0;
        }
		memcpy(&(frame->memory[frame->size]), buffer_temp,buffer_size_temp);
		frame->size += buffer_size_temp;
		frame->memory[frame->size] = 0;
	}

	if(frame->buffer){
		frame->buffer_size = 0;
        free(frame->buffer);
        frame->buffer = NULL;
	}

	return realsize;
}

void* CURLRealloc(void *ptr,size_t size)
{
	if(ptr){
        return realloc(ptr,size);
	}
	else{
        return malloc(size);
	}
}