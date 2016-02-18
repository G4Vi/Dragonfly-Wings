///
/// The log manager
/// 

#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

#include <stdio.h>

#include "Manager.h"
#include "utility.h"

namespace df {

const std::string LOGFILE_NAME = "dragonfly.log";

class LogManager : public Manager {

 private:
  LogManager();                      ///< Private since a singleton.
  LogManager (LogManager const&);    ///< Don't allow copy.
  void operator=(LogManager const&); ///< Don't allow assignment.
  bool do_flush;                ///< True if flush to disk after each write.
  FILE *p_f;                    ///< Pointer to logfile struct.
  int log_level;                ///< Logging level.
  bool log_time_string;		///< True if prepend time.
  bool log_step_count;		///< True if prepend step count.

 public:
  /// If logfile is open, close it.
  ~LogManager();

  /// Get the one and only instance of the LogManager.
  static LogManager &getInstance();

  /// Start up LogManager (open logfile "dragonfly.log").
  int startUp();

  /// Shut down LogManager (close logfile).
  void shutDown();

  /// Write to logfile.
  /// Supports printf() formatting of strings.
  /// Return number of bytes written (excluding prepends), -1 if error.
  int writeLog(const char *fmt, ...) const;

  /// Write to logfile.
  /// Only write if indicated log level >= LogManager log level.
  /// Supports printf() formatting of strings.
  /// Return number of bytes written (excluding prepends), -1 if error.
  int writeLog(int log_level, const char *fmt, ...) const;

  /// Set logging level.
  void setLogLevel(int log_level);

  /// Get logging level.
  int getLogLevel() const;
  
  /// Set flush of logfile after each write.
  void setFlush(bool do_flush=true);
  
  /// Set prepend time string to log messages.
  void setLogTimeString(bool log_time_string=true);
  
  /// Set prepend step count to log messages.
  void setLogStepCount(bool log_step_count=true);
};

} // end of namespace df
#endif // __LOG_MANAGER_H__
