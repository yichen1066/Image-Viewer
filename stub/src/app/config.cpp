#include "pugixml_lib/pugixml.hpp"
#include "wx/stdpaths.h"

#include "app/config.h"
#include "app/app.h"

ConfigFile::ConfigFile() {
}

void ConfigFile::write() {
  pugi::xml_document doc;
  pugi::xml_node decl = doc.append_child(pugi::node_declaration);
  decl.append_attribute("version") = "1.0";

  pugi::xml_node message_node = doc.append_child("Message");
  pugi::xml_node position_node = message_node.append_child("Position");

  wxWindow* top_window = wxGetApp().GetTopWindow();
  wxPoint wnidow_pos = top_window->GetPosition();

  int xpos = wnidow_pos.x;
  std::string x = std::to_string(xpos);
  pugi::xml_node x_pos = position_node.append_child("xpos");
  x_pos.append_child(pugi::node_pcdata).set_value(x.c_str());

  int ypos = wnidow_pos.y;
  std::string y = std::to_string(ypos);
  pugi::xml_node y_pos = position_node.insert_child_after("ypos", x_pos);
  y_pos.append_child(pugi::node_pcdata).set_value(y.c_str());

  pugi::xml_node size_node = message_node.insert_child_after("Size", position_node);

  wxSize window_size = top_window->GetSize();
  pugi::xml_node w_pos = size_node.append_child("width");

  int width = window_size.x;
  std::string w = std::to_string(width);
  w_pos.append_child(pugi::node_pcdata).set_value(w.c_str());

  int height = window_size.y;
  std::string h = std::to_string(height);
  pugi::xml_node h_pos = size_node.insert_child_after("height", w_pos);
  h_pos.append_child(pugi::node_pcdata).set_value(h.c_str());

  doc.save_file("E:\\file\\cmake\\Image Viewer\\config.xml");
}

bool ConfigFile::read() {
  pugi::xml_document doc;
  if (!doc.load_file("E:\\file\\cmake\\Image Viewer\\config.xml")) {
    return 0;
  }

  pugi::xml_node message_node = doc.child("Message");
  pugi::xml_node position_node = message_node.child("Position");

  pugi::xml_node xpos_node = position_node.child("xpos");
  std::string xpos = xpos_node.first_child().value();
  pugi::xml_node ypos_node = position_node.child("ypos");
  std::string ypos = ypos_node.first_child().value();

  win_position_ = wxPoint(std::atoi(xpos.c_str()), std::atoi(ypos.c_str()));

  pugi::xml_node size_node = message_node.child("Size");
  pugi::xml_node width_node = size_node.child("width");
  std::string width = width_node.first_child().value();

  pugi::xml_node height_node = size_node.child("height");
  std::string height = height_node.first_child().value();

  win_size_ = wxSize(std::atoi(width.c_str()), std::atoi(height.c_str()));
  return 1;
}
