#include "wx/sizer.h"
#include "wx/statbmp.h"
#include "wx/bitmap.h"

#include "app/main_frame.h"
#include "app/image_window.h"
#include "app/dnd_file.h"
#include "app/app.h"

DnDFile::DnDFile() {
}

bool DnDFile::OnDropFiles(wxCoord x,
                          wxCoord y,
                          const wxArrayString& file_names) {
  SetImage(file_names[0]);
  return true;
}

void DnDFile::SetImage(const wxString& path_name) {
  wxWindow* top_window = wxGetApp().GetTopWindow();
  wxImage image(path_name);

  wxFrame* frame = new wxFrame(top_window, wxID_ANY,
                               _T(""),
                               wxDefaultPosition,
                               wxDefaultSize,
                               wxCAPTION | wxRESIZE_BORDER | wxCLOSE_BOX);
  frame->Show(true);

  ImageWindow* image_window_ = new ImageWindow(frame, wxID_ANY,
                                               wxDefaultPosition,
                                               frame->GetClientSize(),
                                               path_name);

  frame->SetClientSize(image.GetWidth(), image.GetHeight());
  frame->SetSizeHints(wxSize(500, 350));
}
