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

#include "oc3_nfile.hpp"
#include "oc3_filesystem.hpp"
#include "oc3_filepath.hpp"

#ifdef _WIN32
#define getline_def getline_win
#else
#define getline_def getline
#endif

namespace io
{

static const FilePath purePath = "";

NFile NFile::open(const FilePath& fileName, FSEntity::Mode mode)
{
	return FileSystem::instance().createAndOpenFile( fileName, mode );
}

NFile::NFile()
{
  #ifdef _DEBUG
    setDebugName("NFile");
  #endif

	_entity = 0;
}

NFile::NFile( FSEntityPtr file )
{
  #ifdef _DEBUG
    setDebugName("NFile");
  #endif
	_entity = file;
}

NFile::~NFile()
{
}

bool NFile::isEof() const
{
  return _entity.isValid() ? _entity->isEof() : false;
}

//! returns how much was read
int NFile::read(void* buffer, unsigned int sizeToRead)
{
  return _entity.isValid() ? _entity->read( buffer, sizeToRead ) : 0;
}

//! changes position in file, returns true if successful
//! if relativeMovement==true, the pos is changed relative to current pos,
//! otherwise from begin of file
bool NFile::seek(long finalPos, bool relativeMovement)
{
  return _entity.isValid() ? _entity->seek( finalPos, relativeMovement ) : 0;
}


//! returns size of file
long NFile::getSize() const
{
	return _entity.isValid() ? _entity->getSize() : 0;
}


//! returns where in the file we are.
long NFile::getPos() const
{
	return _entity.isValid() ? _entity->getPos() : 0;
}

ByteArray NFile::readLine()
{
  return _entity.isValid() ? _entity->readLine() : ByteArray();
}

ByteArray NFile::read( unsigned int sizeToRead)
{
  return _entity.isValid() ? _entity->read( sizeToRead ) : ByteArray();
}

//! returns name of file
const FilePath& NFile::getFileName() const
{
	return _entity.isValid() ? _entity->getFileName() : purePath;
}

//! returns how much was read
int NFile::write(const void* buffer, unsigned int sizeToWrite)
{
  return _entity.isValid() ? _entity->write( buffer, sizeToWrite ) : 0;
}

int NFile::write( const ByteArray& bArray )
{
  return _entity.isValid() ? _entity->write( bArray ) : 0;
}

bool NFile::isOpen() const
{
	return _entity.isValid() ? _entity->isOpen() : false;
}

void NFile::flush()
{
  if( _entity.isValid() )
  {
	  _entity->flush();
  }
}

NFile& NFile::operator=( const NFile& other )
{
  _entity = other._entity;

  return *this;
}

} //end namespace io