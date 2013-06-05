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
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com

#ifndef __OPENCAESAR3_SERIALIZABLE_H_INCLUDED__
#define __OPENCAESAR3_SERIALIZABLE_H_INCLUDED__

class VariantMap;

class Serializable
{
public:
  static const char* damageLevel;
  static const char* fireLevel;
  // read/write self on stream
  virtual void save( VariantMap& options ) const = 0;
  virtual void load( const VariantMap& options ) = 0;
};


#endif //__OPENCAESAR3_SERIALIZABLE_H_INCLUDED__
