// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.


#include "oc3_filesystem.hpp"
#include "oc3_filenative_impl.hpp"
#include "oc3_filesystem_archive.hpp"
#include "oc3_filesystem_filelist.hpp"
#include "oc3_filesystem_archive_zip.hpp"
#include "oc3_stringhelper.hpp"

#if defined (OC3_PLATFORM_WIN)
	#include <direct.h> // for _chdir
	#include <io.h> // for _access
	#include <tchar.h>
    #include <stdio.h>
#elif defined(OC3_PLATFORM_UNIX)
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <limits.h>
	#include <sys/types.h>
	#include <dirent.h>
	#include <sys/stat.h>
	#include <unistd.h>
#endif

namespace io
{

class FileSystem::Impl
{
public:
  //! currently attached ArchiveLoaders
  std::vector< ArchiveLoaderPtr > archiveLoaders;
	//! currently attached Archives
  std::vector< ArchivePtr > openArchives;

  //! WorkingDirectory for Native and Virtual filesystems	
  FilePath workingDirectory[2];

  Mode fileSystemType;

  ArchivePtr changeArchivePassword( const FilePath& filename, const std::string& password );
};

ArchivePtr FileSystem::Impl::changeArchivePassword(const FilePath& filename, const std::string& password )
{
  for (int idx = 0; idx < (int)openArchives.size(); ++idx)
  {
    // TODO: This should go into a path normalization method
    // We need to check for directory names with trailing slash and without
    const FilePath absPath = filename.getAbsolutePath();
    const FilePath arcPath = openArchives[idx]->getFileList()->getPath();
    if( (absPath == arcPath) || (arcPath == (absPath.toString() + "/")) )
    {
      if( password.size() )
      {
        openArchives[idx]->Password=password;
      }

      return openArchives[idx];
    }
  }

  return ArchivePtr();
}

//! constructor
FileSystem::FileSystem() : _d( new Impl )
{
  setFileListSystem( fsNative );
  //! reset current working directory

  getWorkingDirectory();

  _d->archiveLoaders.push_back(new ZipArchiveLoader(this));
}


//! destructor
FileSystem::~FileSystem()
{
}

NFile FileSystem::loadFileFromArchive( const FilePath& filePath )
{
  for( unsigned int i=0; i< _d->openArchives.size(); ++i)
  {
    NFile file = _d->openArchives[i]->createAndOpenFile( filePath );
    if( file.isOpen() )
    {
      return file;
    }
  }

  return NFile();
}

//! opens a file for read access
NFile FileSystem::createAndOpenFile(const FilePath& filename, FSEntity::Mode mode)
{
  NFile file = loadFileFromArchive( filename );

  if( file.isOpen() )
  {
    return file;
  }
	
  // Create the file using an absolute path so that it matches
  return NFile( new FileNative( filename.getAbsolutePath(), mode ) );
}

//! Adds an external archive loader to the engine.
void FileSystem::addArchiveLoader( ArchiveLoaderPtr loader)
{
  if( loader.isNull() )
  {
    return;
  }

  _d->archiveLoaders.push_back(loader);
}

//! Returns the total number of archive loaders added.
unsigned int FileSystem::getArchiveLoaderCount() const
{
	return _d->archiveLoaders.size();
}

//! Gets the archive loader by index.
ArchiveLoaderPtr FileSystem::getArchiveLoader(unsigned int index) const
{
	if (index < _d->archiveLoaders.size())
  {
		return _d->archiveLoaders[index];
  }
	else
		return ArchiveLoaderPtr();
}

//! move the hirarchy of the filesystem. moves sourceIndex relative up or down
bool FileSystem::moveFileArchive(unsigned int sourceIndex, int relative)
{
	bool r = false;
	const int dest = (int) sourceIndex + relative;
	const int dir = relative < 0 ? -1 : 1;
	const int sourceEnd = ((int) _d->openArchives.size() ) - 1;
	ArchivePtr t;

	for (int s = (int) sourceIndex;s != dest; s += dir)
	{
		if (s < 0 || s > sourceEnd || s + dir < 0 || s + dir > sourceEnd)
			continue;

		t = _d->openArchives[s + dir];
		_d->openArchives[s + dir] = _d->openArchives[s];
		_d->openArchives[s] = t;
		r = true;
	}

	return r;
}


//! Adds an archive to the file system.
ArchivePtr FileSystem::mountArchive(  const FilePath& filename,
				      Archive::Type archiveType,
				      bool ignoreCase,
				      bool ignorePaths,
				      const std::string& password)
{
  ArchivePtr archive;

  // see if archive is already added
  archive = _d->changeArchivePassword(filename, password );
  if( archive.isValid() )
  {
    return archive;
  }

  int i;

  // do we know what type it should be?
  if (archiveType == Archive::unknown || archiveType == Archive::folder)
  {
    // try to load archive based on file name
    for (i = _d->archiveLoaders.size()-1; i >=0 ; --i)
    {
      if (_d->archiveLoaders[i]->isALoadableFileFormat(filename))
      {
        archive = _d->archiveLoaders[i]->createArchive(filename, ignoreCase, ignorePaths);
        if( archive.isValid() )
        {
          break;
        }
      }
    }

    // try to load archive based on content
    if( archive.isNull() )
    {
      NFile file = createAndOpenFile( filename, FSEntity::fmRead );
      if( file.isOpen() )
      {
        for (i = _d->archiveLoaders.size()-1; i >= 0; --i)
        {
          file.seek(0);
          if (_d->archiveLoaders[i]->isALoadableFileFormat( file ) )
          {
            file.seek(0);
            archive = _d->archiveLoaders[i]->createArchive( file, ignoreCase, ignorePaths);
            if( archive.isValid() )
            {
                break;
            }
          }
        }
      }
    }
  }
  else
  {
    // try to open archive based on archive loader type
    NFile file;
    for (i = _d->archiveLoaders.size()-1; i >= 0; --i)
    {
      if (_d->archiveLoaders[i]->isALoadableFileFormat(archiveType))
      {
        // attempt to open file
        if( !file.isOpen() )
        {
          file = createAndOpenFile(filename, FSEntity::fmRead );
        }

        // is the file open?
        if( file.isOpen() )
        {
          // attempt to open archive
          file.seek(0);
          if (_d->archiveLoaders[i]->isALoadableFileFormat( file))
          {
            file.seek(0);
            archive = _d->archiveLoaders[i]->createArchive( file, ignoreCase, ignorePaths);
            if( archive.isValid() )
            {
                break;
            }
          }
        }
        else
        {
            // couldn't open file
            break;
        }
      }
    }
  }

  if( archive.isValid() )
  {
    _d->openArchives.push_back( archive );
    if( password.size() )
    {
        archive->Password=password;
    }

    return archive;
  }
  else
  {
    StringHelper::debug( 0xff, "Could not create archive for %s", filename.toString().c_str() );
  }

  return ArchivePtr();
}

ArchivePtr FileSystem::mountArchive(NFile file, Archive::Type archiveType,
                                 bool ignoreCase,
                                 bool ignorePaths,
                                 const std::string& password)
{
  if( !file.isOpen() || archiveType == Archive::folder)
  return ArchivePtr();

  if( file.isOpen() )
  {
    ArchivePtr archive = _d->changeArchivePassword( file.getFileName(), password );

    if( archive.isValid() )
    {
        return archive;
    }

    int i;

    if (archiveType == Archive::unknown)
    {
      // try to load archive based on file name
      for (i = _d->archiveLoaders.size()-1; i >=0 ; --i)
      {
        if (_d->archiveLoaders[i]->isALoadableFileFormat( file.getFileName() ) )
        {
          archive = _d->archiveLoaders[i]->createArchive( file, ignoreCase, ignorePaths );
          if (archive.isValid())
          {
            break;
          }
        }
      }

      // try to load archive based on content
      if( archive.isNull() )
      {
        for (i = _d->archiveLoaders.size()-1; i >= 0; --i)
        {
          file.seek(0);
          if (_d->archiveLoaders[i]->isALoadableFileFormat( file ) )
          {
            file.seek(0);
            archive = _d->archiveLoaders[i]->createArchive( file, ignoreCase, ignorePaths);

            if( archive.isValid() )
            {
              break;
            }
          }
        }
      }
    }
    else
    {
      // try to open archive based on archive loader type
      for (i = _d->archiveLoaders.size()-1; i >= 0; --i)
      {
        if (_d->archiveLoaders[i]->isALoadableFileFormat(archiveType))
        {
          // attempt to open archive
          file.seek(0);

          if (_d->archiveLoaders[i]->isALoadableFileFormat( file))
          {
            file.seek(0);
            archive = _d->archiveLoaders[i]->createArchive( file, ignoreCase, ignorePaths);
            if( archive.isValid() )
            {
              break;
            }
          }
        }
      }
    }

    if( archive.isValid() )
    {
      StringHelper::debug( 0xff, "Mount archive %s", file.getFileName().toString().c_str() );
      _d->openArchives.push_back(archive);

      if (password.size())
      {
        archive->Password=password;
      }

      return archive;
    }
    else
    {
      StringHelper::debug( 0xff, "Could not create archive for %s", file.getFileName().toString().c_str() );
    }
  }

  return ArchivePtr();
}


//! Adds an archive to the file system.
ArchivePtr FileSystem::mountArchive( ArchivePtr archive)
{
	for (unsigned int i=0; i < _d->openArchives.size(); ++i)
	{
		if( archive == _d->openArchives[i])
		{
            return archive;
		}
	}

	_d->openArchives.push_back(archive);
    return archive;
}


//! removes an archive from the file system.
bool FileSystem::unmountArchive(unsigned int index)
{
	bool ret = false;
	if (index < _d->openArchives.size())
	{
		_d->openArchives[index]->drop();
		_d->openArchives.erase( _d->openArchives.begin() + index );
		ret = true;
	}

	return ret;
}


//! removes an archive from the file system.
bool FileSystem::unmountArchive(const FilePath& filename)
{
	for (unsigned int i=0; i < _d->openArchives.size(); ++i)
	{
		if (filename == _d->openArchives[i]->getFileList()->getPath())
			return unmountArchive(i);
	}

	return false;
}


//! Removes an archive from the file system.
bool FileSystem::unmountArchive( ArchivePtr archive)
{
	for (unsigned int i=0; i < _d->openArchives.size(); ++i)
	{
		if( archive == _d->openArchives[i] )
		{
			return unmountArchive(i);
		}
	}

	return false;
}


//! gets an archive
unsigned int FileSystem::getFileArchiveCount() const
{
	return _d->openArchives.size();
}


ArchivePtr FileSystem::getFileArchive(unsigned int index)
{
	return index < getFileArchiveCount() ? _d->openArchives[index] : 0;
}


//! Returns the string of the current working directory
const FilePath& FileSystem::getWorkingDirectory()
{
	int type = 0;

	if (type != fsNative)
	{
		type = fsVirtual;
	}
	else
	{
		#if defined(OC3_PLATFORM_WIN)
			char tmp[_MAX_PATH];
			_getcwd(tmp, _MAX_PATH);
      _d->workingDirectory[type] = StringHelper::replace( tmp, "\\", "/" );
		#elif defined(OC3_PLATFORM_UNIX)
			// getting the CWD is rather complex as we do not know the size
			// so try it until the call was successful
			// Note that neither the first nor the second parameter may be 0 according to POSIX
			unsigned int pathSize=256;
            ScopedPtr< char > tmpPath( new char[pathSize] );
      
            while( (pathSize < (1<<16)) && !( getcwd( tmpPath.data(), pathSize)))
			{
				pathSize *= 2;
				tmpPath.reset( new char[pathSize] );
			}

            if( tmpPath )
			{
                _d->workingDirectory[fsNative] = FilePath( tmpPath.data() );
			}
		#endif //OC3_PLATFORM_UNIX

		//_d->workingDirectory[type].validate();
	}

	return _d->workingDirectory[type];
}


//! Changes the current Working Directory to the given string.
bool FileSystem::changeWorkingDirectoryTo(const FilePath& newDirectory)
{
	bool success=false;

    if ( _d->fileSystemType != fsNative)
    {
      // is this empty string constant really intended?
      _d->workingDirectory[fsVirtual] = newDirectory.flattenFilename( "" );
      success = true;
    }
    else
    {
        _d->workingDirectory[ fsNative ] = newDirectory;
#if defined(OC3_PLATFORM_WIN)
        success = ( _chdir( newDirectory.toString().c_str() ) == 0 );
#elif defined(OC3_PLATFORM_UNIX)
        success = ( chdir( newDirectory.toString().c_str() ) == 0 );
#endif //OC3_PLATFORM_UNIX
    }

    return success;
}

//! Sets the current file systen type
FileSystem::Mode FileSystem::setFileListSystem( Mode listType)
{
	Mode current = _d->fileSystemType;
	_d->fileSystemType = listType;
	return current;
}

//! looks if file is in the same directory of path. returns offset of directory.
//! 0 means in same directory. 1 means file is direct child of path
inline int isInSameDirectory ( const FilePath& path, const FilePath& file )
{
	int subA = 0;
	int subB = 0;
	int pos;

  if ( path.toString().size() && !StringHelper::isEqualen( path.toString(), file.toString(), path.toString().size() ) )
		return -1;

	pos = 0;
	while ( (pos = path.toString().find( '/', pos )) >= 0 )
	{
		subA += 1;
		pos += 1;
	}

	pos = 0;
	while ( (pos = file.toString().find ( '/', pos )) >= 0 )
	{
		subB += 1;
		pos += 1;
	}

	return subB - subA;
}

//! Creates a list of files and directories in the current working directory
FileList FileSystem::getFileList()
{
  FileList ret;
  FilePath rpath = StringHelper::replace( getWorkingDirectory().toString(), "\\", "/" );
  rpath = rpath.addEndSlash();

	//! Construct from native filesystem
  if ( _d->fileSystemType == fsNative )
	{
		// --------------------------------------------
		//! Windows version
		#if defined(OC3_PLATFORM_WIN)
			ret.setIgnoreCase( true );

			struct _finddata_t c_file;
			long hFile;

			if( (hFile = _findfirst( "*", &c_file )) != -1L )
			{
				do
				{
					ret.addItem( FilePath( rpath.toString() + c_file.name ), 0, c_file.size, (_A_SUBDIR & c_file.attrib) != 0, 0);
				}
				while( _findnext( hFile, &c_file ) == 0 );

				_findclose( hFile );
			}

			//TODO add drives
			//entry.Name = "E:\\";
			//entry.isDirectory = true;
			//Files.push_back(entry);
		#elif defined(OC3_PLATFORM_UNIX)

			// --------------------------------------------
			//! Linux version
            ret.setIgnoreCase( false );

            ret.addItem( FilePath( rpath.toString() + ".." ), 0, 0, true, 0);

			//! We use the POSIX compliant methods instead of scandir
            DIR* dirHandle=opendir( rpath.toString().c_str());
			if (dirHandle)
			{
				struct dirent *dirEntry;
				while ((dirEntry=readdir(dirHandle)))
				{
					unsigned int size = 0;
					bool isDirectory = false;

					if((strcmp(dirEntry->d_name, ".")==0) || (strcmp(dirEntry->d_name, "..")==0))
					{
						continue;
					}
					struct stat buf;
					if (stat(dirEntry->d_name, &buf)==0)
					{
						size = buf.st_size;
						isDirectory = S_ISDIR(buf.st_mode);
					}
					/*
					#if !defined(__CYGWIN__)
					// only available on some systems
					else
					{
						isDirectory = dirEntry->d_type == DT_DIR;
					}
					#endif*/
					
                    ret.addItem( FilePath( rpath.toString() + dirEntry->d_name ), 0, size, isDirectory, 0);
				}
				closedir(dirHandle);
			}
		#endif //OC3_PLATFORM_UNIX
	}
	else
	{
		//! create file list for the virtual filesystem
		ret.setIgnoreCase( false );

		//! add relative navigation
		FileListItem e2;
		FileListItem e3;

		//! PWD
		ret.addItem( FilePath( rpath.toString() + "." ), 0, 0, true, 0);

		//! parent
		ret.addItem( FilePath( rpath.toString() + ".." ), 0, 0, true, 0);

		//! merge archives
		for (unsigned int i=0; i < _d->openArchives.size(); ++i)
		{
			const FileList *merge = _d->openArchives[i]->getFileList();

			for (unsigned int j=0; j < merge->getFileCount(); ++j)
			{
				if ( isInSameDirectory(rpath, merge->getFullFileName(j)) == 0)
				{
					ret.addItem(merge->getFullFileName(j), merge->getFileOffset(j), merge->getFileSize(j), merge->isDirectory(j), 0);
				}
			}
		}
	}

	ret.sort();

  return ret;
}

//! determines if a file exists and would be able to be opened.
bool FileSystem::existFile(const FilePath& filename) const
{
  for (unsigned int i=0; i < _d->openArchives.size(); ++i)
      if (_d->openArchives[i]->getFileList()->findFile(filename)!=-1)
              return true;

#if defined(OC3_PLATFORM_WIN)
  return ( _access( filename.toString().c_str(), 0) != -1);
#elif defined(OC3_PLATFORM_UNIX)
  return ( access( filename.toString().c_str(), 0 ) != -1);
#endif //OC3_PLATFORM_UNIX
}

FileSystem& FileSystem::instance()
{
	static FileSystem _instanceFileSystem;
	return _instanceFileSystem;
}

} //end namespace io
