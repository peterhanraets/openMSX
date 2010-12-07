// $Id$

#ifndef FILEPOOL_HH
#define FILEPOOL_HH

#include "FileOperations.hh"
#include "noncopyable.hh"
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <ctime>

namespace openmsx {

class CommandController;
class File;
class StringSetting;
class CliComm;

class FilePool : private noncopyable
{
public:
	explicit FilePool(CommandController& controler);
	~FilePool();

	enum FileType {
		SYSTEM_ROM = 1, ROM = 2, DISK = 4, TAPE = 8
	};

	/** Search file with the given sha1sum.
	 * If found it returns the (already opened) file,
	 * if not found it returns a NULL pointer.
	 */
	std::auto_ptr<File> getFile(FileType fileType, const std::string& sha1sum);

	/** Calculate sha1sum for the given File object.
	 * If possible the result is retrieved from cache, avoiding the
	 * relatively expensive calculation.
	 */
	std::string getSha1Sum(File& file);

	/** Remove sha1sum for this file from the cache.
	 * When the file was written to, sha1sum changes and it should be
	 * removed from the cache.
	 */
	void removeSha1Sum(File& file);

private:
	struct Entry {
		std::string path;
		int types;
	};
	typedef std::multimap<int, Entry> Directories;

	// Manually implement a collection of <sha1sum, timestamp, filename>
	// tuples, that is indexed on both sha1sum and filename. Using
	// something like boost::multi_index would be both faster and more
	// compact in memory.
	//   <sha1sum, <timestamp, filename> >
	typedef std::multimap<std::string, std::pair<time_t, std::string> > Pool;
	//   <filename, Pool::iterator>
	typedef std::map<std::string, Pool::iterator> ReversePool;

	void insert(const std::string& sum, time_t time, const std::string& filename);
	void remove(Pool::iterator it);

	void readSha1sums();
	void writeSha1sums();

	std::auto_ptr<File> getFromPool(const std::string& sha1sum);
	std::auto_ptr<File> scanDirectory(const std::string& sha1sum,
	                                  const std::string& directory);
	std::auto_ptr<File> scanFile(const std::string& sha1sum,
	                             const std::string& filename,
	                             const FileOperations::Stat& st);
	Pool::iterator findInDatabase(const std::string& filename);

	void getDirectories(Directories& result) const;


	const std::auto_ptr<StringSetting> filePoolSetting;
	CliComm& cliComm;

	Pool pool;
	ReversePool reversePool;
};

} // namespace openmsx

#endif
