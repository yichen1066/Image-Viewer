#ifndef IMAGEVIEWER_APP_DND_FILE_H_
#define IMAGEVIEWER_APP_DND_FILE_H_

#include "wx/dnd.h"

class DnDFile : public wxFileDropTarget {
public:
  DnDFile();
  virtual ~DnDFile() {
  };

  virtual bool OnDropFiles(wxCoord x,
                           wxCoord y,
                           const wxArrayString& file_names);

private:
  void SetImage(const wxString& path_name);
};

#endif  // IMAGEVIEWER_APP_DND_FILE_H_
