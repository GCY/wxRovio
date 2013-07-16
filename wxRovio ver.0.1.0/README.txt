Windows:
Opencv 2.3.1 up
wxWidgets 2.8.12
libCURL 7.21.3

Linux:
Opencv 2.3.1 up
wxWidgets 2.8.12
libCURL 7.21.3

g++ -o2 -o wxRovio './AddUserDialog.cpp' './App.cpp' './DeleteUserDialog.cpp' 
'./LoggingDialog.cpp' './SetCameraDialog.cpp' 
'./SetOtherDialog.cpp' './wxRovio.cpp' 
`wx-config --cxxflags --libs --unicode` `pkg-config opencv --cflags --libs` -lcurl
