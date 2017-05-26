#include "wx/dcbuffer.h"
#include "wx/settings.h"
#include "wx/filefn.h"
#include "wx/msgdlg.h"
#include "wx/clipbrd.h"

#include "app/image_window.h"
#include "app/main_frame.h"
#include "app/dnd_file.h"
#include "app/thumb_msg_dialog.h"
#include "app/resource_id.h"

BEGIN_EVENT_TABLE(ImageWindow, wxScrolledWindow)
EVT_RIGHT_DOWN(ImageWindow::OnRightClicked)
EVT_PAINT(ImageWindow::OnPaint)
EVT_ERASE_BACKGROUND(ImageWindow::OnEraseBackground)
EVT_SIZE(ImageWindow::OnSize)
EVT_LEFT_DCLICK(ImageWindow::OnLeftDclick)
EVT_KEY_DOWN(ImageWindow::OnEscapeKeyDown)

EVT_MENU(wxID_SAVE, ImageWindow::OnMenuFileSave)
EVT_MENU(wxID_SAVEAS, ImageWindow::OnMenuFileSaveAs)
EVT_MENU(wxID_COPY, ImageWindow::OnMenuFileCopy)
EVT_MENU(wxID_DELETE, ImageWindow::OnMenuFileDelete)
EVT_MENU(wxID_CLOSE, ImageWindow::OnMenuFileClose)
EVT_MENU(ID_ROTATE_CLOCK_WISE, ImageWindow::OnRotateClockWise)
EVT_MENU(ID_ROTATE_COUNTER_CLOCK_WISE, ImageWindow::OnRotateCounterClockWise)
EVT_MENU(ID_CONVERT_TO_MONO, ImageWindow::OnConvertToMono)
END_EVENT_TABLE()

ImageWindow::ImageWindow(wxWindow* parent,
                         wxWindowID winid,
                         const wxPoint& mypoint,
                         const wxSize& mysize,
                         const wxString& path_name)
    : wxScrolledWindow(parent, winid, mypoint, mysize, wxVSCROLL | wxHSCROLL)
    , path_name_(path_name) {
  image_.LoadFile(path_name_);
  child_frame_ = (wxFrame*)this->GetParent();
}

ImageWindow::ImageWindow(wxWindow* parent,
                         wxWindowID winid,
                         const wxPoint& mypoint,
                         const wxSize& mysize)
    : wxScrolledWindow(parent, winid, mypoint, mysize, wxVSCROLL | wxHSCROLL){
}

ImageWindow::~ImageWindow() {
}

void ImageWindow::OnRightClicked(wxMouseEvent& evt) {
  wxMenu* menu = new wxMenu;

  menu->Append(wxID_SAVE, "&Save");
  menu->Append(wxID_SAVEAS, "&Save As");
  menu->AppendSeparator();
  menu->Append(ID_ROTATE_CLOCK_WISE, "&Rotate clockwise");
  menu->Append(ID_ROTATE_COUNTER_CLOCK_WISE, "&Rotate counterclockwise");
  menu->Append(ID_CONVERT_TO_MONO, "&ConvertToMono");

  menu->AppendSeparator();
  menu->Append(wxID_COPY, "&Copy");
  menu->Append(wxID_DELETE, "&Delete");
//  menu->Enable(wxID_COPY, false);

  menu->AppendSeparator();
  menu->Append(wxID_CLOSE, "&Close");

  PopupMenu(menu, evt.GetPosition());
}

void ImageWindow::OnPaint(wxPaintEvent& evt) {
  wxBufferedPaintDC dc(this);
  PrepareDC(dc);
  dc.Clear();
  Draw(dc);
}

void ImageWindow::Draw(wxDC& dc) {
  wxBitmap bitmap = image_copy_;
  wxCoord xpos = ((double)client_size_.x - update_image_width_) / 2;
  wxCoord ypos = ((double)client_size_.y - update_image_height_) / 2;
  dc.DrawBitmap(bitmap, xpos, ypos);
}

//Reduce screen flicker
void ImageWindow::OnEraseBackground(wxEraseEvent& evt) {
}

void ImageWindow::OnSize(wxSizeEvent& evt) {
  SetThumbnailSize();
  Refresh(true);
}

void ImageWindow::OnLeftDclick(wxMouseEvent& evt) {
  child_frame_->SetWindowStyle(wxDEFAULT_FRAME_STYLE & ~(wxSYSTEM_MENU | wxRESIZE_BORDER | wxMAXIMIZE_BOX | 
                               wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN));
  SetBackgroundColour(*wxBLACK);
  child_frame_->Maximize(true);
  Refresh(true);
}

void ImageWindow::OnEscapeKeyDown(wxKeyEvent& evt) {
  if (evt.GetUnicodeKey() == WXK_ESCAPE) {
    child_frame_->Maximize(false);
    child_frame_->SetWindowStyle(wxCAPTION | wxRESIZE_BORDER | wxCLOSE_BOX);
  }
  SetBackgroundColour(*wxWHITE);
}

void ImageWindow::OnMenuFileSave(wxCommandEvent& eve) {
  image_.SaveFile(path_name_);
}

void ImageWindow::OnMenuFileSaveAs(wxCommandEvent& evt) {
  wxString caption = _T("Choose a picture file");
  wxString wildcard = _T("BMP files (*.bmp)|*.bmp|GIF files (*.gif)|*.gif|PNG files (*.png)|*.png");
  wxString default_dir;
  wxString default_filename;

  wxFileDialog* dlg = new wxFileDialog(this,
                                       caption,
                                       default_dir,
                                       default_filename,
                                       wildcard,
                                       0,
                                       wxDefaultPosition);

  if (dlg->ShowModal() == wxID_OK) {
    wxString path_name = dlg->GetPath();
    wxString dir = dlg->GetDirectory();

    image_.SaveFile(path_name, wxBITMAP_TYPE_BMP);
  }
}

void ImageWindow::OnMenuFileCopy(wxCommandEvent& evt) {
  wxBitmap  bitmap = image_;
  if (wxTheClipboard->Open()) {
    wxTheClipboard->Clear();
    wxTheClipboard->SetData(new wxBitmapDataObject(bitmap));
    wxTheClipboard->Close();
  }
}

void ImageWindow::OnMenuFileDelete(wxCommandEvent& evt) {
  ThumbMsgDialog dialog(child_frame_,
                         wxID_ANY,
                         _T("Delete file"),
                         path_name_);
  dialog.Center();
  dialog.ShowModal();
}

void ImageWindow::OnMenuFileClose(wxCommandEvent& evt) {
  child_frame_->Close(true);
}

void ImageWindow::OnRotateClockWise(wxCommandEvent& evt) {
  image_ = image_.Rotate90(true);
  image_copy_ = image_;
  SetThumbnailSize();

  Refresh(true);
}

void ImageWindow::OnRotateCounterClockWise(wxCommandEvent& evt) {
  image_ = image_.Rotate90(false);
  image_copy_ = image_;
  SetThumbnailSize();

  Refresh(true);
}

void ImageWindow::OnConvertToMono(wxCommandEvent& evt) {
  image_copy_ = image_;
  ConvertToGrayImage(image_copy_);

  Refresh(true);
}

void ImageWindow::SetThumbnailSize() {
  client_size_ = GetClientSize();

  double scale_x = ((double)client_size_.x) / ((double)image_.GetWidth());
  double scale_y = ((double)client_size_.y) / ((double)image_.GetHeight());
  double scale = wxMin(scale_x, scale_y);

  int new_width = (int)(scale * image_.GetWidth());
  int new_height = (int)(scale* image_.GetHeight());

  image_copy_ = image_.Scale(new_width, new_height);
  update_image_width_ = (double)image_copy_.GetWidth();
  update_image_height_ = (double)image_copy_.GetHeight();
}

void ImageWindow::ConvertToGrayImage(const wxImage& image) {
  double red2gray = 0.297;
  double green2gray = 0.589;
  double blue2gray = 0.114;

  int width = image.GetWidth();
  int height = image.GetHeight();

  unsigned char* data = image.GetData();

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      long pos = (y * width + x) * 3;
      char g = (char)(data[pos] * red2gray + data[pos + 1] * green2gray + data[pos + 2] * blue2gray);
      data[pos] = data[pos + 1] = data[pos + 2] = g;
    }
  }
}
