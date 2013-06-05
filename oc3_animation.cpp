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

#include "oc3_animation.hpp"
#include "oc3_positioni.hpp"
#include "oc3_pic_loader.hpp"

void Animation::start(bool loop)
{
    _animIndex = 0;
    _loop = loop;
    _lastTimeUpdate = 0;
}

PicturesArray& Animation::getPictures()
{
    return _pictures;
}

const PicturesArray& Animation::getPictures() const
{
    return _pictures;
}

void Animation::setOffset( const Point& offset )
{
    for( PicturesArray::iterator it = _pictures.begin(); it != _pictures.end(); ++it)
    {
        (*it)->setOffset(offset.getX(), offset.getY() );
    }
}

void Animation::update( unsigned int time )
{  
  if( _animIndex < 0 )
    return;

  if( _frameDelay > 0 )
  {
    if( time - _lastTimeUpdate < _frameDelay )
      return;
  }

  _animIndex += 1;
  _lastTimeUpdate = time;

  if( _animIndex >= (int)_pictures.size() ) 
  {
    _animIndex = _loop ? 0 : -1;
  }
}

Picture* Animation::getCurrentPicture()
{
    return (_pictures.size() > 0 && _animIndex >= 0) ? _pictures[_animIndex] : 0;
}

int Animation::getCurrentIndex() const
{
    return _animIndex;
}

Animation::Animation()
{
    _frameDelay = 0;
    start( true );
}

void Animation::setFrameDelay( const unsigned int delay )
{
    _frameDelay = delay;
}

void Animation::setLoop( bool loop )
{
    _loop = loop;
}

void Animation::load( const std::string &prefix, const int start, const int number, 
                      bool reverse /*= false*/, const int step /*= 1*/ )
{
    PicLoader& loader = PicLoader::instance();

    int revMul = reverse ? -1 : 1;
    for( int i = 0; i < number; ++i)
    {
        Picture &pic = loader.getPicture(prefix, start + revMul*i*step);
        _pictures.push_back( &pic );
    }
}

void Animation::clear()
{
    _pictures.clear();
}

bool Animation::isRunning() const
{
    return _animIndex >= 0;
}

bool Animation::isStopped() const
{
    return _animIndex == -1;
}

void Animation::stop()
{
    _animIndex = -1;
}

Animation& Animation::operator=( const Animation& other )
{
  _pictures = other._pictures;
  _animIndex = other._animIndex;  // index of the current frame
  _frameDelay = other._frameDelay;
  _lastTimeUpdate = _lastTimeUpdate;
  _loop = other._loop;

  return *this;
}

int Animation::getPicturesCount() const
{
  return _pictures.size();
}

bool Animation::isValid() const
{
  return _pictures.size() > 0;
}