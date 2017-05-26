#ifndef IMAGEVIEWER_APP_IMAGE_THREAD_H_
#define IMAGEVIEWER_APP_IMAGE_THREAD_H_

#include "wx/thread.h"
#include "wx/string.h"

class wxImage;

class LoadImageThread : public wxThread {

public:
  LoadImageThread(const wxString& file_path, wxImage* image, wxFrame* frame);
  virtual ~LoadImageThread();

  virtual void* Entry() override;

private:
  wxString file_path_;
  wxImage* image_;
  wxFrame* image_frame_;
};

#endif  // IMAGEVIEWER_APP_IMAGE_THREAD_H_
