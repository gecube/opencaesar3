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

#ifndef __OPENCAESAR3_ENUMS_HELPER_H_INCLUDED__
#define __OPENCAESAR3_ENUMS_HELPER_H_INCLUDED__

#include "oc3_requirements.hpp"
#include <map>
#include <string>

template< class T >
class EnumsHelper
{
public:
  typedef std::pair< T, std::string > TypeEquale;
  typedef std::map< T, std::string > Equales;

  T findType( const std::string& name ) const
  {
    for( typename Equales::const_iterator it=_equales.begin(); it != _equales.end(); it++ )
    {
      if( name == it->second )
      {
        return it->first;
      }
    }

    return getInvalid();
  }

  std::string findName( T type ) const
  {
    typename Equales::const_iterator it = _equales.find( type );
    return it != _equales.end() ? it->second : "";
  }

  void append( T key, const std::string name )
  {
    _equales[ key ] = name;
  }

  virtual T getInvalid() const { return _invalid; }

  virtual ~EnumsHelper() {}
  EnumsHelper( T invalid ) : _invalid( invalid ) {}

protected:
  Equales _equales;
  T _invalid;
};

#endif //__OPENCAESAR3_ACADEMY_H_INCLUDED__
