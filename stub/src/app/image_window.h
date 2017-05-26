#ifndef IMAGEVIEWER_APP_IMAGE_WINDOW_H_
#define IMAGEVIEWER_APP_IMAGE_WINDOW_H_

#include "wx/scrolwin.h"
#include "wx/menu.h"
#include "app/main_frame.h"


class ImageWindow : public wxScrolledWindow {
  DECLARE_EVENT_TABLE();

public:
  ImageWindow(wxWindow* parent,
              wxWindowID winid,
              const wxPoint& mypoint,
              const wxSize& mysize,
              const wxString& path_name);

  ImageWindow(wxWindow* parent,
              wxWindowID winid,
              const wxPoint& mypoint,
              const wxSize& mysize);

  virtual ~ImageWindow();

protected:
  void OnRightClicked(wxMouseEvent& evt);
  void OnEraseBackground(wxEraseEvent& evt);
  void OnPaint(wxPaintEvent& evt);
  void Draw(wxDC& dc);

  void OnSize(wxSizeEvent& evt);
  void OnLeftDclick(wxMouseEvent& evt);
  void OnEscapeKeyDown(wxKeyEvent& evt);

  void OnMenuFileSave(wxCommandEvent& eve);
  void OnMenuFileSaveAs(wxCommandEvent& evt);
  void OnMenuFileCopy(wxCommandEvent& evt);
  void OnMenuFileDelete(wxCommandEvent& evt);
  void OnMenuFileClose(wxCommandEvent& evt);

  void OnRotateClockWise(wxCommandEvent& evt);
  void OnRotateCounterClockWise(wxCommandEvent& evt);
  void OnConvertToMono(wxCommandEvent& evt);
  void SetThumbnailSize();

  void ConvertToGrayImage(const wxImage& image);

private:
  wxImage image_;
  wxImage image_copy_;
  wxSize client_size_;
  wxString path_name_;

  double update_image_width_;
  double update_image_height_;

  wxFrame* child_frame_;
};

#endif  // IMAGEVIEWER_APP_IMAGE_WINDOW_H_
