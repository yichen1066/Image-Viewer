#ifndef IMAGEVIEWER_APP_CONFIG_H_
#define IMAGEVIEWER_APP_CONFIG_H_

#include "app/main_frame.h"

class ConfigFile {
public:
  ConfigFile();
  void write();
  bool read();

  wxPoint position() const {
    return win_position_;
  }
  wxSize size() const {
    return win_size_;
  }

private:
  wxPoint win_position_;
  wxSize win_size_;
};

#endif  // IMAGEVIEWER_APP_CONFIG_H_
