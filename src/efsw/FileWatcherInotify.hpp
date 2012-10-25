#ifndef _FW_FILEWATCHERLINUX_H_
#define _FW_FILEWATCHERLINUX_H_
#pragma once

#include "FileWatcherImpl.hpp"

#if EFSW_PLATFORM == EFSW_PLATFORM_INOTIFY

#include <map>
#include <sys/types.h>

namespace efsw
{
	/// Implementation for Linux based on inotify.
	/// @class FileWatcherInotify
	class FileWatcherInotify : public FileWatcherImpl
	{
	public:
		/// type for a map from WatchID to WatchStruct pointer
		typedef std::map<WatchID, WatchStruct*> WatchMap;

	public:
		FileWatcherInotify();

		virtual ~FileWatcherInotify();

		/// Add a directory watch
		/// @exception FileNotFoundException Thrown when the requested directory does not exist
		WatchID addWatch(const std::string& directory, FileWatchListener* watcher, bool recursive);

		/// Remove a directory watch. This is a brute force lazy search O(nlogn).
		void removeWatch(const std::string& directory);

		/// Remove a directory watch. This is a map lookup O(logn).
		void removeWatch(WatchID watchid);

		/// Updates the watcher. Must be called often.
		void watch();

		/// Handles the action
		void handleAction(WatchStruct* watch, const std::string& filename, unsigned long action);

		/// @return Returns a list of the directories that are being watched
		std::list<std::string> directories();
	private:
		/// Map of WatchID to WatchStruct pointers
		WatchMap mWatches;

		/// inotify file descriptor
		int mFD;

		Thread * mThread;

		Mutex mWatchesLock;

		void run();

	};
}

#endif

#endif