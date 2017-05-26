#ifndef IMAGEVIEWER_APP_THUMBNAIL_MSG_DIALOG_H_
#define IMAGEVIEWER_APP_THUMBNAIL_MSG_DIALOG_H_

#include "wx/dialog.h"

class ThumbMsgDialog : public wxDialog {
  DECLARE_EVENT_TABLE()

public:
  ThumbMsgDialog(wxWindow* parent,
                 wxWindowID id,
                 const wxString &title,
                 wxString path_name,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxDEFAULT_DIALOG_STYLE);

protected:
  void OnPaint(wxPaintEvent& evt);
  void Init();
  bool Create(wxWindow* parent,
              wxWindowID id,
              const wxString &title,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxDEFAULT_DIALOG_STYLE);
  void CreateControls();

  wxBitmap SetIcon();
  void SetTextSizer(wxBoxSizer* sizer);
  wxString PathToImageName();
  wxString ItemType(const wxImage& image);

  void OnButtonYes(wxCommandEvent& evt);
  void OnButtonNo(wxCommandEvent& evt);

private:
  wxString path_name_;
};

#endif  // IMAGEVIEWER_APP_THUMBNAIL_MSG_DIALOG_H_
