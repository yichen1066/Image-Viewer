#include "wx/sizer.h"
#include "wx/dcbuffer.h"
#include "wx/stattext.h"
#include "wx/statbmp.h"
#include "wx/string.h"
#include "wx/button.h"

#include "app/thumb_msg_dialog.h"

BEGIN_EVENT_TABLE(ThumbMsgDialog, wxDialog)
EVT_BUTTON(wxID_YES, ThumbMsgDialog::OnButtonYes)
EVT_BUTTON(wxID_NO, ThumbMsgDialog::OnButtonNo)
END_EVENT_TABLE()

ThumbMsgDialog::ThumbMsgDialog(wxWindow* parent,
                               wxWindowID id,
                               const wxString &title,
                               wxString path_name,
                               const wxPoint& pos,
                               const wxSize& size,
                               long style) 
    : path_name_(path_name) {
  Create(parent, id, title, pos, size, style);
  CreateControls();
}

void ThumbMsgDialog::Init() {

}

bool ThumbMsgDialog::Create(wxWindow* parent,
                            wxWindowID id,
                            const wxString &title,
                            const wxPoint& pos, 
                            const wxSize& size,
                            long style) {
  if (!wxDialog::Create(parent, id, title, pos, size, style)) {
    return false;
  }
  return true;
}

void ThumbMsgDialog::CreateControls() {
  wxBoxSizer* top_sizer = new wxBoxSizer(wxHORIZONTAL);
  this->SetSizer(top_sizer);

  wxBoxSizer* left_sizer = new wxBoxSizer(wxHORIZONTAL);
  wxBitmap bitmap = SetIcon();
  wxStaticBitmap* static_icon = new wxStaticBitmap(this, wxID_STATIC, bitmap);
  left_sizer->Add(static_icon, 0, wxALIGN_LEFT | wxALL, 5);
  top_sizer->Add(left_sizer, 0, wxTOP | wxALIGN_TOP, 5);

  wxBoxSizer* right_sizer = new wxBoxSizer(wxVERTICAL);
  wxStaticText* descr = new wxStaticText(this,
                                         wxID_STATIC,
                                         _T("Are you sure you want to put this file in the Recycle bin?"));

  right_sizer->Add(descr, 0, wxTOP | wxLEFT, 15);
  wxBoxSizer* thumb_sizer = new wxBoxSizer(wxHORIZONTAL);

  wxImage image(path_name_);
  if (image.GetWidth() > image.GetHeight()) {
    wxBitmap thumb_bitmap = image.Scale(120, 80, wxIMAGE_QUALITY_HIGH);
    wxStaticBitmap* thumbnail = new wxStaticBitmap(this, wxID_STATIC, thumb_bitmap);
    thumb_sizer->Add(thumbnail, 0, wxALL, 5);
  } else {
    wxBitmap thumb_bitmap = image.Scale(80, 90, wxIMAGE_QUALITY_HIGH);
    wxStaticBitmap* thumbnail = new wxStaticBitmap(this, wxID_STATIC, thumb_bitmap);
    thumb_sizer->Add(thumbnail, 0, wxALL, 5);
  }

  wxBoxSizer* text_sizer = new wxBoxSizer(wxVERTICAL);
  SetTextSizer(text_sizer);
  thumb_sizer->Add(text_sizer, 0, wxALL, 5);
  right_sizer->Add(thumb_sizer, 0, wxLEFT | wxTOP, 10);
  right_sizer->AddSpacer(30);

  wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton* button_yes = new wxButton(this, wxID_YES, _T("Yes"));
  button_sizer->Add(button_yes, 0, wxRIGHT, 5);

  wxButton* button_no = new wxButton(this, wxID_NO, _T("No"));
  button_sizer->Add(button_no, 0, wxLEFT, 5);
  right_sizer->Add(button_sizer, 0, wxALIGN_RIGHT, 5);

  top_sizer->Add(right_sizer, 0, wxALIGN_LEFT, 5);
  Layout();
}

wxBitmap ThumbMsgDialog::SetIcon() {
  wxImage image(_T("bitmaps\\6.ico"), wxBITMAP_TYPE_ANY);
  int new_width = image.GetWidth() / 6;
  int new_height = image.GetHeight() / 6;

  wxImage icon_thumbnail = image.Scale(new_width, new_height, wxIMAGE_QUALITY_HIGH);
  return icon_thumbnail;

}

void ThumbMsgDialog::SetTextSizer(wxBoxSizer* sizer) {
  wxString name = PathToImageName();

  wxStaticText* image_name = new wxStaticText(this, wxID_STATIC, name);
  sizer->Add(image_name, 0, wxALIGN_LEFT, 5);

  wxImage image(path_name_);
  wxStaticText* item_type = new wxStaticText(this, wxID_STATIC, _T("Item type: ") + ItemType(image));
  sizer->Add(item_type, 0, wxALIGN_LEFT, 5);

  wxDateTime time(wxFileModificationTime(path_name_));
  wxString time_str = time.FormatTime();
  wxStaticText* date_modified = new wxStaticText(this, wxID_ANY, _T("Date modified: ") + time_str);
  sizer->Add(date_modified, 0, wxALIGN_LEFT, 5);

  wxString image_width = std::to_wstring(image.GetWidth());
  wxString image_height = std::to_wstring(image.GetHeight());
  wxStaticText* dimensions = new wxStaticText(this, wxID_STATIC, _T("Dimensions: ") + image_width + _T("x") + image_height);
  sizer->Add(dimensions, 0, wxALIGN_LEFT, 5);
}

wxString ThumbMsgDialog::PathToImageName() {
  size_t n = path_name_.size();
  size_t index = path_name_.find_last_of('\\');

  return path_name_.SubString(index + 1, n);
}

wxString ThumbMsgDialog::ItemType(const wxImage& image) {
  wxBitmapType bitmap_type = image.GetType();
  wxString str(wxEmptyString);

  switch (bitmap_type) {
    case wxBITMAP_TYPE_BMP:
      str = wxString("Bitmap Image");
      break;
    case wxBITMAP_TYPE_CUR:
      str = wxString("Cursor");
        break;
    case wxBITMAP_TYPE_ICO:
      str = wxString("Icon");
    default:
      break;
  }
  return str;
}

void ThumbMsgDialog::OnButtonYes(wxCommandEvent& evt) {
  Close(true);
  wxRemoveFile(path_name_);

  wxWindow* parent = GetParent();
  parent->Close(true);
}

void ThumbMsgDialog::OnButtonNo(wxCommandEvent& evt) {
  Close(true);
}
