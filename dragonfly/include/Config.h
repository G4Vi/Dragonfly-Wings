//
// Config.h - Global configuration parameters for Dragonfly
//

#ifndef __CONFIG_H__
#define __CONFIG_H__

// System includes.
#include <string>

// Engine includes.
#include "Color.h"

namespace df {

class Config {

 private:
  Config();			 ///< Private since a singleton.
  Config (Config const&);	 ///< Don't allow copy.
  void operator=(Config const&); ///< Don't allow assignment.
  int window_horizontal_pixels;  ///< Horizontal pixels in window.
  int window_vertical_pixels;    ///< Vertical pixels in window.
  int window_horizontal_chars;   ///< Horizontal ASCII spaces in window.
  int window_vertical_chars;     ///< Vertical ASCII spaces in window.
  int window_style;		 ///< Style of window titlebar.
  enum Color window_background_color; ///< Window background color.
  int frame_time;		 ///< Target time for 1 game loop (in mlsecs).
  std::string window_title;	 ///< Title of window.
  bool show_mouse;               ///< True if should show mouse cursor.
  bool headless;                 ///< True if run without graphics or input.
  std::string font_file;	 ///< Filename for graphics fonts.
  int random_seed;		 ///< Seed for random number generation.

 public:
  /// Get the singleton instance of the Config.
  static Config &getInstance();

  /// Get attributes.
  int getWindowHorizontalPixels() const;
  int getWindowVerticalPixels() const;
  int getWindowHorizontalChars() const;
  int getWindowVerticalChars() const;
  enum Color getWindowBackgroundColor() const;
  int getWindowStyle() const;
  int getFrameTime() const;
  std::string getWindowTitle() const;
  bool getShowMouse() const;
  bool getHeadless() const;
  std::string getFontFile() const;
  int getRandomSeed() const;
  
  /// Set attributes.
  void setWindowHorizontalPixels(int window_horizontal_pixels);
  void setWindowVerticalPixels(int window_vertical_pixels);
  void setWindowHorizontalChars(int window_horizontal_chars);
  void setWindowVerticalChars(int window_vertical_chars);
  void setWindowBackgroundColor(enum Color window_background_color);
  void setWindowStyle(int window_style);
  void setFrameTime(int frame_time);
  void setWindowTitle(std::string window_title);
  void setHeadless(bool headless=true);
  void setShowMouse(bool show_mouse=true);
  void setFontFile(std::string font_file);
  void setRandomSeed(int random_seed);

  // Write all parameters to logfile.
  void writeToLog() const;
};

} // end of namespace df
#endif // __CONFIG_H__
