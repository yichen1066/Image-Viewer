#include "wx/image.h"
#include "wx/filedlg.h"
#include "wx/dirdlg.h"
#include "wx/string.h"
#include "wx/dcclient.h"
#include "wx/dcbuffer.h"
#include "wx/bmpbuttn.h"

#include "app/app.h"
#include "app/main_frame.h"
#include "app/image_window.h"
#include "app/image_thread.h"
#include "app/dnd_file.h"
#include "app/config.h"
#include "app/resource_id.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(wxID_OPEN, MainFrame::OnPictureFileOpen)
EVT_MENU(wxID_EXIT, MainFrame::OnMenuFileQuit)
EVT_MENU(ID_MENU_INFO_ABOUT, MainFrame::OnMenuInfoAbout)
EVT_MENU(ID_MENU_SELECT_DIRECTORY, MainFrame::OnMenuOptionDirectory)

EVT_THREAD(ID_LOAD_IMAGE_THREAD, MainFrame::OnLoadImageThread)
EVT_MENU(ID_MENU_IMPORT, MainFrame::OnImportImage)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title,
                     const wxPoint& mypoint,
                     const wxSize& mysize)
    : wxFrame(NULL, wxID_ANY, title, mypoint, mysize)
    , child_frame_(NULL)
    , image_window_(NULL) {
  CreateControls();

  DnDFile* dnd_file = new DnDFile();
  SetDropTarget(dnd_file);
}

MainFrame::~MainFrame() {
  OnExit();
}

void MainFrame::OnPictureFileOpen(wxCommandEvent& evt) {
  wxString caption = _T("Choose a picture file");
  wxString wildcard = _T("BMP files (*.bmp)|*.bmp|GIF files (*.ico)|*.ico|PNG files (*.png)|*.png");
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
    wxString path = dlg->GetDirectory();
    image_name_ = dlg->GetFilename();
    path_name_ = dlg->GetPath();

    wxImage image(path_name_);
    images_map_[path_name_] = wxImage();

    child_frame_ = new wxFrame(this,
                               wxID_ANY,
                               _T(""),
                               wxDefaultPosition,
                               wxSize(image.GetWidth(), image.GetHeight()),
                               wxCAPTION | wxRESIZE_BORDER | wxCLOSE_BOX);

    child_frame_->SetSizeHints(wxSize(475, 350));
    child_frame_->SetBackgroundColour(*wxWHITE);
    child_frame_->Center();
    child_frame_->Show(true);

    // Load the image in another thread.
    LoadImageThread* load_image_thread = new LoadImageThread(path_name_, &images_map_[path_name_], child_frame_);
    load_image_thread->Run();
  }
}

void MainFrame::OnMenuFileQuit(wxCommandEvent& evt) {
  Close(FALSE);
}

void MainFrame::OnMenuInfoAbout(wxCommandEvent& evt) {
  wxLogMessage("File About Menu Selected");
}

void MainFrame::OnMenuOptionDirectory(wxCommandEvent& evt) {
  wxDirDialog *dlg = new wxDirDialog(this, "Select a new working directory", wxGetCwd());
  if (dlg->ShowModal() == wxID_OK) {
    wxSetWorkingDirectory(dlg->GetPath());
  }
  dlg->Destroy();
}

void MainFrame::OnLoadImageThread(wxThreadEvent& evt) {
  wxString file_path = evt.GetString();
  std::map<wxString, wxImage>::iterator it = images_map_.find(file_path);
  if (it == images_map_.end()) {
    // TODO: Log
    return;
  }

  wxImage image = it->second;
  images_map_.erase(it);

  image_window_ = new ImageWindow(child_frame_,
                                  wxID_ANY,
                                  wxDefaultPosition,
                                  child_frame_->GetClientSize(),
                                  path_name_);
  child_frame_->SetClientSize(image.GetWidth(), image.GetHeight());
  child_frame_->SetTitle(image_name_);
}

void MainFrame::OnImportImage(wxCommandEvent& evt) {
  wxString caption = _T("Choose a picture file");
  wxString wildcard = _T("BMP files (*.bmp)|*.bmp|GIF files (*.ico)|*.ico|PNG files (*.png)|*.png");
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
    wxString image_path = dlg->GetPath();
    wxImage image(image_path);
    import_image_.push_back(image.Scale(100, 90));
  }

  wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  for (size_t i = 0; i < import_image_.size(); ++i) {
    wxBitmap bitmap = import_image_[i];
    wxStaticBitmap* static_bitmap = new wxStaticBitmap(this, wxID_STATIC, bitmap);
    sizer->Add(static_bitmap, 0, wxALL, 10);
  }
  SetSizer(sizer);

  Layout();
}

void MainFrame::OnExit() {
  ConfigFile* config_file = new ConfigFile();
  config_file->write();
  Destroy();
}

void MainFrame::CreateControls() {
  wxMenu* stock_menu = new wxMenu();
  stock_menu->Append(wxID_OPEN, "&Open\tCtrl+O", "Open a picture file");

  wxMenu* file_menu = new wxMenu();
  file_menu->AppendSubMenu(stock_menu, "&File");
  file_menu->AppendSeparator();
  file_menu->Append(wxID_EXIT, "&Exit\tCtrl+Q", "Quit the application");

  wxMenuBar* menu_bar = new wxMenuBar();
  menu_bar->Append(file_menu, "&Menu");

  wxMenu* info_menu = new wxMenu();
  info_menu->Append(ID_MENU_INFO_ABOUT, "&About", "Shows information about the application");

  wxMenu* options_menu = new wxMenu();
  options_menu->Append(ID_MENU_SELECT_DIRECTORY, "&Working Space");
  menu_bar->Append(options_menu, "&Options");
  menu_bar->Append(info_menu, "&Info");

  SetMenuBar(menu_bar);
  CreateStatusBar(1);
  SetStatusText("Ready", 0);

//  CreateBitmapButton();
}

void MainFrame::CreateBitmapButton() {
  wxBitmap bitmap_adult = wxImage(ADULT_BITMAP);
  wxBitmap bitmap_child = wxImage(CHILD_BITMAP);

  wxBitmapButton* bitmap_button = new wxBitmapButton(this, wxID_ANY, bitmap_adult);
}
