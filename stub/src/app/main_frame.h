#ifndef IMAGEVIEWER_APP_MAIN_FRAME_H_
#define IMAGEVIEWER_APP_MAIN_FRAME_H_

#include <map>

#include "wx/sizer.h"
#include "wx/scrolwin.h"
#include "wx/frame.h"
#include "wx/statbmp.h"
#include "wx/filedlg.h"
#include "wx/dnd.h"

#define ADULT_BITMAP _T("E:\\file\\picure\\adult.png")
#define CHILD_BITMAP _T("E:\\file\\picure\\child.png")

class wxImage;
class ImageWindow;

class MainFrame : public wxFrame {
  DECLARE_EVENT_TABLE()

public:
  // TODO: Use wxPoint and wxSize as parameters.
  // TODO: Use const wxString& instead of const wxChar* for title.
  MainFrame(const wxString& title, const wxPoint& mypoint, const wxSize& mysize);
  virtual ~MainFrame();
protected:
  void OnMenuFileQuit(wxCommandEvent& evt);
  void OnMenuInfoAbout(wxCommandEvent& evt);
  void OnMenuOptionDirectory(wxCommandEvent& evt);
  void OnPictureFileOpen(wxCommandEvent& evt);
  void OnLoadImageThread(wxThreadEvent& evt);
  void OnImportImage(wxCommandEvent& evt);

  void OnExit();
  void CreateControls();
  void CreateBitmapButton();

private:
  ImageWindow* image_window_;
  wxFrame* child_frame_;
  std::map<wxString, wxImage> images_map_;
  std::vector<wxImage> import_image_;

  wxString image_name_;
  wxString path_name_;
};

#endif  // IMAGEVIEWER_APP_MAIN_FRAME_H_
