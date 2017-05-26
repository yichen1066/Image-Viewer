#include "wx/toolbar.h"

#include "app/app.h"
#include "app/main_frame.h"
#include "app/config.h"
#include "app/resource_id.h"

IMPLEMENT_APP(App)

bool App::OnInit() {
  if (!wxApp::OnInit()) {
    return false;
  }
  wxInitAllImageHandlers();

  ConfigFile* config_file = new ConfigFile();
  if (!config_file->read()) {
    wxLogMessage("CONFIG FILE NOT FOUND!");
    MainFrame* image_frame = new MainFrame(_T("Image Viewer"), wxPoint(200, 100), wxSize(700, 450));
    wxIcon icon1("bitmaps\\w9.ico", wxBITMAP_TYPE_ICO, 16, 16);

    image_frame->SetBackgroundColour(*wxWHITE);
    image_frame->SetIcon(icon1);
    image_frame->Show(true);
    return true;
  }

  MainFrame* image_frame = new MainFrame(_T("Image Viewer"), config_file->position(), config_file->size());
  wxIcon icon1("bitmaps\\w9.ico", wxBITMAP_TYPE_ICO, 16, 16);

  image_frame->SetBackgroundColour(*wxWHITE);
  CreateToolBar(image_frame);

  image_frame->SetIcon(icon1);
  image_frame->Show(true);

  //image_frame->SetSizeHints(wxSize(500, 350), wxSize(1000, 600));
  return true;
}

void App::CreateToolBar(MainFrame* main_frame) {
  wxToolBar* tool_bar = new wxToolBar(main_frame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxNO_BORDER);

  wxBitmap open_bitmap = wxImage("bitmaps\\open.xpm");
  wxBitmap save_bitmap = wxImage("bitmaps\\save.xpm");

  tool_bar->AddSeparator();
  tool_bar->AddTool(wxID_OPEN, open_bitmap, _T("Open Image"));
  tool_bar->AddSeparator();
  tool_bar->AddTool(ID_MENU_IMPORT, save_bitmap, _T("Import Image"));

  tool_bar->Realize();
  main_frame->SetToolBar(tool_bar);
}
