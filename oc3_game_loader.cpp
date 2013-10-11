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

#include "oc3_game_loader.hpp"

#include "oc3_game_loader_map.hpp"
#include "oc3_game_loader_sav.hpp"
#include "oc3_game_loader_oc3save.hpp"
#include "oc3_game_loader_oc3mission.hpp"
#include "oc3_positioni.hpp"
#include "oc3_tilemap.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_tile.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_foreach.hpp"
#include "oc3_game.hpp"
#include "oc3_city.hpp"

#include <vector>

typedef SmartPtr< GameAbstractLoader > GameAbstractLoaderPtr;

class GameLoader::Impl
{
public:
  typedef std::vector< GameAbstractLoaderPtr > Loaders;
  typedef Loaders::iterator LoaderIterator;
  Loaders loaders;

  void initLoaders();
  void initEntryExitTile( const TilePos& tlPos, Tilemap& tileMap, const unsigned int picIdStart, bool exit );
  void initWaterTileAnimation( Tilemap& tmap );
  void finalize( Game& game );
};

GameLoader::GameLoader() : _d( new Impl )
{
  _d->initLoaders();
}

GameLoader::~GameLoader()
{

}

void GameLoader::Impl::initEntryExitTile( const TilePos& tlPos, Tilemap& tileMap, const unsigned int picIdStart, bool exit )
{
  unsigned int idOffset = 0;
  TilePos tlOffset;
  if( tlPos.getI() == 0 || tlPos.getI() == tileMap.getSize() - 1 )
  {
    tlOffset = TilePos( 0, 1 );
    idOffset = (tlPos.getI() == 0 ? 1 : 3 );

  }
  else if( tlPos.getJ() == 0 || tlPos.getJ() == tileMap.getSize() - 1 )
  {
    tlOffset = TilePos( 1, 0 );
    idOffset = (tlPos.getJ() == 0 ? 2 : 0 );
  }

  Tile& signTile = tileMap.at( tlPos + tlOffset );

  StringHelper::debug( 0xff, "(%d, %d)", tlPos.getI(),    tlPos.getJ()    );
  StringHelper::debug( 0xff, "(%d, %d)", tlOffset.getI(), tlOffset.getJ() );

  signTile.setPicture( ResourceGroup::land3a, picIdStart + idOffset );
  signTile.setFlag( Tile::tlRock, true );
}

void GameLoader::Impl::initWaterTileAnimation( Tilemap& tmap )
{
  TilemapArea area = tmap.getArea( TilePos( 0, 0 ), Size( tmap.getSize() ) );

  Animation water;
  water.setFrameDelay( 12 );
  water.load( ResourceGroup::land1a, 121, 7 );
  water.load( ResourceGroup::land1a, 127, 7, true );
  foreach( Tile* tile, area )
  {
    int rId = tile->getOriginalImgId() - 364;
    if( rId >= 0 && rId < 8 )
    {
      water.setCurrentIndex( rId );
      tile->setAnimation( water );
    }
  }
}

void GameLoader::Impl::finalize( Game& game )
{
  Tilemap& tileMap = game.getCity()->getTilemap();

  // exit and entry can't point to one tile or .... can!
  const BorderInfo& border = game.getCity()->getBorderInfo();

  initEntryExitTile( border.roadEntry, tileMap, 89, false );
  initEntryExitTile( border.roadExit,  tileMap, 85, true  );
  initWaterTileAnimation( tileMap );
}

void GameLoader::Impl::initLoaders()
{
  loaders.push_back( GameAbstractLoaderPtr( new GameLoaderC3Map() ) );
  loaders.push_back( GameAbstractLoaderPtr( new GameLoaderC3Sav() ) );
  loaders.push_back( GameAbstractLoaderPtr( new GameLoaderOc3() ) );
  loaders.push_back( GameAbstractLoaderPtr( new GameLoaderMission() ) );
}

bool GameLoader::load( const io::FilePath& filename, Game& game )
{
  // try to load file based on file extension
  Impl::LoaderIterator it = _d->loaders.begin();
  for( ; it != _d->loaders.end(); ++it)
  {
    if( (*it)->isLoadableFileExtension( filename.toString() ) /*||
        (*it)->isLoadableFileFormat(file) */ )
    {
      bool loadok = (*it)->load( filename.toString(), game );
      
      if( loadok )
      {
        _d->finalize( game );
      }

      return loadok;
    }
  }

  return false; // failed to load
}
