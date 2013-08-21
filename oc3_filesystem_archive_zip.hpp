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

#ifndef __OC3_ZIP_READER_H_INCLUDED__
#define __OC3_ZIP_READER_H_INCLUDED__

#include "oc3_filesystem.hpp"
#include "oc3_filesystem_file.hpp"
#include "oc3_filesystem_archive.hpp"
#include "oc3_filesystem_filelist.hpp"

namespace io
{

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif


// set if the file is encrypted
const short ZIP_FILE_ENCRYPTED =		0x0001;
// the fields crc-32, compressed size and uncompressed size are set to
// zero in the local header
const short ZIP_INFO_IN_DATA_DESCRIPTOR =	0x0008;

struct SZIPFileDataDescriptor
{
    unsigned int CRC32;
    unsigned int CompressedSize;
    unsigned int UncompressedSize;
} PACK_STRUCT;

struct SZIPFileHeader
{
    unsigned int Sig;				// 'PK0304' little endian (0x04034b50)
    short VersionToExtract;
    short GeneralBitFlag;
    short CompressionMethod;
    short LastModFileTime;
    short LastModFileDate;
	SZIPFileDataDescriptor DataDescriptor;
    short FilenameLength;
    short ExtraFieldLength;
	// filename (variable size)
	// extra field (variable size )
} PACK_STRUCT;

struct SZIPFileCentralDirFileHeader
{
    unsigned int Sig;	// 'PK0102' (0x02014b50)
    unsigned short VersionMadeBy;
    unsigned short VersionToExtract;
    unsigned short GeneralBitFlag;
    unsigned short CompressionMethod;
    unsigned short LastModFileTime;
    unsigned short LastModFileDate;
    unsigned int CRC32;
    unsigned int CompressedSize;
    unsigned int UncompressedSize;
    unsigned short FilenameLength;
    unsigned short ExtraFieldLength;
    unsigned short FileCommentLength;
    unsigned short DiskNumberStart;
    unsigned short InternalFileAttributes;
    unsigned int ExternalFileAttributes;
    unsigned int RelativeOffsetOfLocalHeader;

	// filename (variable size)
	// extra field (variable size)
	// file comment (variable size)

} PACK_STRUCT;

struct SZIPFileCentralDirEnd
{
    unsigned int Sig;			// 'PK0506' end_of central dir signature			// (0x06054b50)
    unsigned short NumberDisk;		// number of this disk
    unsigned short NumberStart;	// number of the disk with the start of the central directory
    unsigned short TotalDisk;		// total number of entries in the central dir on this disk
    unsigned short TotalEntries;	// total number of entries in the central dir
    unsigned int Size;			// size of the central directory
    unsigned int Offset;			// offset of start of centraldirectory with respect to the starting disk number
    unsigned short CommentLength;	// zipfile comment length
	// zipfile comment (variable size)
} PACK_STRUCT;

struct SZipFileExtraHeader
{
    short ID;
    short Size;
} PACK_STRUCT;

struct SZipFileAESExtraData
{
    short Version;
    unsigned char Vendor[2];
    unsigned char EncryptionStrength;
    short CompressionMode;
} PACK_STRUCT;

enum E_GZIP_FLAGS
{
	EGZF_TEXT_DAT      = 1,
	EGZF_CRC16         = 2,
	EGZF_EXTRA_FIELDS  = 4,
	EGZF_FILE_NAME     = 8,
	EGZF_COMMENT       = 16
};

struct SGZIPMemberHeader
{
    unsigned short sig; // 0x8b1f
    unsigned char  compressionMethod; // 8 = deflate
    unsigned char  flags;
    unsigned int time;
    unsigned char  extraFlags; // slow compress = 2, fast compress = 4
    unsigned char  operatingSystem;
} PACK_STRUCT;

#if defined(_MSC_VER) || defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

//! Contains extended info about zip files in the archive
struct SZipFileEntry
{
  //! Position of data in the archive file
  int Offset;

  //! The header for this file containing compression info etc
  SZIPFileHeader header;
};

//! Archiveloader capable of loading ZIP Archives
class ZipArchiveLoader : public ArchiveLoader
{
public:

      //! Constructor
  ZipArchiveLoader( io::FileSystem* fs );

      //! returns true if the file maybe is able to be loaded by this class
      //! based on the file extension (e.g. ".zip")
      virtual bool isALoadableFileFormat(const FilePath& filename) const;

      //! Check if the file might be loaded by this class
      /** Check might look into the file.
      \param file File handle to check.
      \return True if file seems to be loadable. */
  virtual bool isALoadableFileFormat( NFile file ) const;

      //! Check to see if the loader can create archives of this type.
      /** Check based on the archive type.
      \param fileType The archive type to check.
      \return True if the archile loader supports this type, false if not */
  virtual bool isALoadableFileFormat( Archive::Type fileType) const;

      //! Creates an archive from the filename
      /** \param file File handle to check.
      \return Pointer to newly created archive, or 0 upon error. */
  virtual ArchivePtr createArchive(const FilePath& filename, bool ignoreCase, bool ignorePaths) const;

      //! creates/loads an archive from the file.
      //! \return Pointer to the created archive. Returns 0 if loading failed.
  virtual ArchivePtr createArchive(NFile file, bool ignoreCase, bool ignorePaths) const;

private:
  io::FileSystem* _fileSystem;
};

class ZipArchiveReader : public virtual Archive, virtual FileList
{
public:
  //! constructor
  ZipArchiveReader( NFile file, bool ignoreCase, bool ignorePaths, bool isGZip=false);

  //! destructor
  virtual ~ZipArchiveReader();

  //! opens a file by file name
  virtual NFile createAndOpenFile(const FilePath& filename);

  //! opens a file by index
  virtual NFile createAndOpenFile(unsigned int index);

  //! returns the list of files
  virtual const FileList* getFileList() const;

  //! get the archive type
  virtual std::string getTypeName() const;

  Archive::Type getType() const;
protected:

  //! reads the next file header from a ZIP file, returns false if there are no more headers.
  /* if ignoreGPBits is set, the item will be read despite missing
     file information. This is used when reading items from the central
     directory. */
  bool scanZipHeader(bool ignoreGPBits=false);

  //! the same but for gzip files
  bool scanGZipHeader();

  bool scanCentralDirectoryHeader();

  NFile File;

  // holds extended info about files
  std::vector< SZipFileEntry > FileInfo;

  bool IsGZip;
};

}

#endif // __OC3_ZIP_READER_H_INCLUDED__
