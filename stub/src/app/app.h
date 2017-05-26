#ifndef IMAGEVIEWER_APP_APP_H_
#define IMAGEVIEWER_APP_APP_H_

#include "wx/app.h"

class MainFrame;

class App : public wxApp {
public:
  virtual bool OnInit() override;

private:
  void CreateToolBar(MainFrame* main_frame);
};

DECLARE_APP(App)

#endif  // IMAGEVIEWER_APP_APP_H_
