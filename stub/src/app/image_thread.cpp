#include "wx/image.h"
#include "wx/window.h"
#include "wx/dcbuffer.h"
#include "wx/statbmp.h"

#include "app/app.h"
#include "app/image_thread.h"
#include "app/main_frame.h"
#include "app/resource_id.h"

LoadImageThread::LoadImageThread(const wxString& file_path, wxImage* image, wxFrame* image_frame)
    : file_path_(file_path), image_(image), image_frame_(image_frame) {
}

LoadImageThread::~LoadImageThread() {
}

void* LoadImageThread::Entry() {
//   wxImage image(PATH);
//   wxBitmap bitmap = image;
//   int xpos = (image_frame_->GetClientSize().x - image.GetWidth()) / 2;
//   int ypos = (image_frame_->GetClientSize().y - image.GetHeight()) / 2;
// 
//   wxStaticBitmap* static_bitmap = new wxStaticBitmap(image_frame_,
//                                                      wxID_ANY, bitmap,
//                                                      wxPoint(xpos, ypos),
//                                                      wxDefaultSize);
// 
//   static_bitmap->SetBitmap(bitmap);
  bool loaded = image_->LoadFile(file_path_);

  wxWindow* top_window = wxGetApp().GetTopWindow();
  if (top_window != NULL) {
    wxThreadEvent* evt = new wxThreadEvent(wxEVT_THREAD, ID_LOAD_IMAGE_THREAD);
    evt->SetString(file_path_);
    evt->SetInt((int)loaded);
//    delete static_bitmap;
    wxQueueEvent(top_window->GetEventHandler(), evt); 
  }

  return (ExitCode)0;
}
