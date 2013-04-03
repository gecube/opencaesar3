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



#include <gui_tilemap.hpp>

#include <gfx_engine.hpp>
#include <exception.hpp>
#include <algorithm>
#include <iostream>
#include <gui_info_box.hpp>
#include <screen_game.hpp>
#include <oc3_positioni.h>
#include <oc3_pictureconverter.h>

typedef std::list<Tile*> Tiles;

class GuiTilemap::Impl
{
public:
    Tiles postTiles;  // these tiles have draw over "normal" tilemap tiles!
    Picture buildInstncePicture;
    Point lastCursorPos;
};


GuiTilemap::GuiTilemap() : _d( new Impl )
{
   _city = NULL;
   _mapArea = NULL;
   _isPreview = false;
}

GuiTilemap::~GuiTilemap() {}


void GuiTilemap::setPreview(const bool isPreview)
{
   _isPreview = isPreview;
}

void GuiTilemap::init(City &city, TilemapArea &mapArea, ScreenGame *screen)
{
   _city = &city;
   _tilemap = &_city->getTilemap();
   _mapArea = &mapArea;
   _screenGame = screen;
   _buildInstance = NULL;
   _removeTool = false;
}

void GuiTilemap::drawTileEx( const Tile& tile, const int depth )
{
    if( tile.is_flat() )
    {
        return;  // tile has already been drawn!
    }

    Tile* master = tile.get_master_tile();

    if( master==NULL )
    {
        // single-tile
		drawTile( tile );
    }
    else
    {
        // multi-tile: draw the master tile.
        int masterZ = master->getJ() - master->getI();
        if( masterZ == depth )
        {
            // it is time to draw the master tile
            if (std::find(_multiTiles.begin(), _multiTiles.end(), master) == _multiTiles.end())
            {
                // master has not been drawn yet
                _multiTiles.push_back(master);  // don't draw that multi-tile again
				drawTile( *master );
            }
        }
    }
}

void GuiTilemap::drawTile( const Tile& tile )
{
	int i = tile.getI();
	int j = tile.getJ();

	int dx = _tilemap_xoffset;
	int dy = _tilemap_yoffset;

	Picture& pic = tile.get_picture();
	GfxEngine &engine = GfxEngine::instance();
	engine.drawPicture(pic, 30*(i+j)+dx, 15*(i-j)+dy);

	// building foregrounds and animations
	LandOverlay *overlay = tile.get_terrain().getOverlay();
	if( overlay != NULL )
	{
		std::vector<Picture*>& fgPictures = overlay->getForegroundPictures();
		for (std::vector<Picture*>::iterator itPic = fgPictures.begin(); itPic != fgPictures.end(); ++itPic)
		{
			// for each foreground picture
			if (*itPic == NULL)
			{
				// skip void picture
				continue;
			}
			Picture &fgpic = **itPic;
			engine.drawPicture(fgpic, 30*(i+j)+dx, 15*(i-j)+dy);
		}
	}
}


void GuiTilemap::drawTilemap()
{
   GfxEngine &engine = GfxEngine::instance();

   _multiTiles.clear();  // we will draw everything again
   Tilemap &tilemap = *_tilemap;
   TilemapArea &mapArea = *_mapArea;

   // center the map on the screen
   int dx = engine.getScreenWidth()/2 - 30*(mapArea.getCenterX()+1) + 1;
   int dy = engine.getScreenHeight()/2 + 15*(mapArea.getCenterZ()-tilemap.getSize()+1) - 30;
   _tilemap_xoffset = dx;  // this is the current offset
   _tilemap_yoffset = dy;  // this is the current offset

   int lastZ = -1000;  // dummy value

   const std::vector<std::pair<int, int> >& tiles = mapArea.getTiles();
   std::vector<std::pair<int, int> >::const_iterator itTiles;

   // FIRST PART: draw all flat land (walkable/boatable)
   for (itTiles = tiles.begin(); itTiles != tiles.end(); ++itTiles)
   {
      int i = (*itTiles).first;
      int j = (*itTiles).second;

      Tile& tile = getTileIJ(i, j);
      Tile* master = tile.get_master_tile();

      if( tile.is_flat() )
      {
         if( master==NULL )
         {
            // single-tile
			 drawTile( tile );
         }
         else
         {
            // multi-tile: draw the master tile.
            if (std::find(_multiTiles.begin(), _multiTiles.end(), master) == _multiTiles.end())
            {
               // master has not been drawn yet
               _multiTiles.push_back(master);
			   drawTile( *master );
            }
         }
      }
   }

   // SECOND PART: draw all sprites, impassable land and buildings
   for (itTiles = tiles.begin(); itTiles != tiles.end(); ++itTiles)
   {
      int i = (*itTiles).first;
      int j = (*itTiles).second;

      int z = j - i;
      if (z != lastZ)
      {
         // TODO: pre-sort all animations
         lastZ = z;

         std::vector<Picture*> pictureList;

         std::list<Walker*> walkerList = _city->getWalkerList();
         for (std::list<Walker*>::iterator itWalker =  walkerList.begin();
               itWalker != walkerList.end(); ++itWalker)
         {
            // for each walker
            Walker &anim = **itWalker;
            int zAnim = anim.getJ() - anim.getI();
            if (zAnim > z && zAnim <= z+1)
            {
               pictureList.clear();
               anim.getPictureList(pictureList);
               for (std::vector<Picture*>::iterator picIt = pictureList.begin(); picIt != pictureList.end(); ++picIt)
               {
                  if (*picIt == NULL)
                  {
                     continue;
                  }
                  Picture &pic = **picIt;
                  engine.drawPicture(pic, 2*(anim.getII()+anim.getJJ())+dx, anim.getII()-anim.getJJ()+dy);
               }
            }
         }
      }

      Tile& tile = getTileIJ(i, j);
	  drawTileEx( tile, z );
   }

   //Third part: drawing build/remove tools
    {
        for( Tiles::iterator itPostTile = _d->postTiles.begin(); itPostTile != _d->postTiles.end(); ++itPostTile )
        {
            int z = (*itPostTile)->getJ() - (*itPostTile)->getI();
			drawTileEx( **itPostTile, z );
        }       
    }
}

Tile* GuiTilemap::getTileXY(const int x, const int y)
{
   int dx = x-_tilemap_xoffset;  // x relative to the left most pixel of the tilemap
   int dy = y-_tilemap_yoffset;  // y relative to the left most pixel of the tilemap
   int i = (dx+2*dy)/60;
   int j = (dx-2*dy)/60;
   // std::cout << "ij ("<<i<<","<<j<<")"<<std::endl;

   Tilemap &tilemap = *_tilemap;
   if (i>=0 && j>=0 && i<tilemap.getSize() && j<tilemap.getSize())
   {
      // valid coordinate
      return &getTileIJ(i, j);
   }
   else
   {
      // the pixel is outside the tilemap => no tile here
      return NULL;
   }
}


TilemapArea &GuiTilemap::getMapArea()
{
   return *_mapArea;
}

void GuiTilemap::updatePreviewTiles()
{
    Tile* tile = getTileXY( _d->lastCursorPos.x, _d->lastCursorPos.y );  // tile under the cursor (or NULL)
    discardPreview();

    if( tile )
    {
        int i = tile->getI();
        int j = tile->getJ();

        checkPreviewBuild(i, j);
        checkPreviewRemove(i, j);
    }
};


void GuiTilemap::handleEvent(SDL_Event &event)
{
   if (event.type == SDL_MOUSEMOTION)
   {
       _d->lastCursorPos = Point( event.motion.x, event.motion.y );  
       updatePreviewTiles();
   }

   if (event.type == SDL_MOUSEBUTTONDOWN)
   {
      int button = event.button.button;
      int x = event.button.x;
      int y = event.button.y;

      Tile* tile = getTileXY(x, y);  // tile under the cursor (or NULL)
      if (tile == NULL)
      {
         // std::cout << "No Tile pressed at ("<<x<<","<<y<<")" << std::endl;
      }
      else
      {
         int i = tile->getI();
         int j = tile->getJ();
         std::cout << "Tile ("<<i<<","<<j<<") pressed at ("<<x<<","<<y<<") with button:" << button << std::endl;

         if (button == 1)
         {
            // left button
            if (_removeTool)
            {
               _city->clearLand(i, j);
            }
            else if (_buildInstance != NULL)
            {
               Construction& overlay = *_buildInstance;
               if (overlay.canBuild(i, j))
               {
                  _city->build(overlay, i, j);
               }
            }
            else
            {
               getMapArea().setCenterIJ(i, j);
            }
         }
         else if (button == 3)
         {
            if (_removeTool)
            {
               _removeTool = false;
               discardPreview();
            }
            else if (_buildInstance != NULL)
            {
               _buildInstance = NULL;
               discardPreview();
            }
            // std::cout << "request info for tile" << std::endl;
            if (tile->get_terrain().getOverlay() == NULL)
            {
               // tile has no overlay
               // tile_type = typeid(*tile->get_terrain());
               // std::cout << "no overlay!" << std::endl;
            }
            else
            {
               // tile has an overlay
               LandOverlay &overlay = *tile->get_terrain().getOverlay();
               GuiInfoBox* infoBox = overlay.makeInfoBox();

               if (infoBox != NULL)
               {
                  infoBox->setPosition((GfxEngine::instance().getScreenWidth()-infoBox->getWidth())/2, 10);
                  _screenGame->setInfoBox(infoBox);
               }
            }

         }
      }

   }
}


void GuiTilemap::setBuildInstance(Construction *buildInstance)
{
   // std::cout << "set build instance!" << std::endl;
   _buildInstance = buildInstance;
   _removeTool = false;
}


void GuiTilemap::setRemoveTool()
{
   // std::cout << "set remove tool!" << std::endl;
   _buildInstance = NULL;
   _removeTool = true;
}


void GuiTilemap::discardPreview()
{
   for( Tiles::iterator it=_d->postTiles.begin(); it != _d->postTiles.end(); it++ )
        delete *it;
   _d->postTiles.clear();
}

void GuiTilemap::checkPreviewBuild(const int i, const int j)
{
   // TODO: do only when needed, when (i, j, _buildInstance) has changed
   if (_buildInstance != NULL)
   {
      Construction& overlay = *_buildInstance;

      int size = overlay.getSize();
      if( overlay.canBuild(i, j) )
      {
          PictureConverterPtr converter = PictureConverter::create();
          converter->rgbBalance( _d->buildInstncePicture, overlay.getPicture(), -255, +0, -255 );

          Tile *masterTile=0;
          for (int dj = 0; dj<size; ++dj)
          {
              for (int di = 0; di<size; ++di)
              {
                  Tile* tile = new Tile(_tilemap->at(i+di, j+dj));  // make a copy of tile

                  if (di==0 && dj==0)
                  {
                      // this is the masterTile
                      masterTile = tile;
                  }
                  tile->set_picture( &_d->buildInstncePicture );
                  tile->set_master_tile(masterTile);
                  TerrainTile &terrain = tile->get_terrain();
                  terrain.setOverlay(&overlay);
                  _d->postTiles.push_back( tile );
                  //_priorityTiles.push_back( tile );
              }
          }
      }
      else
      {
          Picture& grnPicture = PicLoader::instance().get_picture( "oc3_land", 1 );
          Picture& redPicture = PicLoader::instance().get_picture( "oc3_land", 2 );
          
          for (int dj = 0; dj<size; ++dj)
          {
              for (int di = 0; di<size; ++di)
              {
                  Tile* tile = new Tile(_tilemap->at(i+di, j+dj));  // make a copy of tile

                  bool isConstructible = tile->get_terrain().isConstructible();
                  tile->set_picture( isConstructible ? &grnPicture : &redPicture );
                  tile->set_master_tile(0);
				  tile->get_terrain().reset();
                  _d->postTiles.push_back( tile );
              }
          }
      }
   }
}

void GuiTilemap::checkPreviewRemove(const int i, const int j)
{
   if (_removeTool)
   {
      Tile& cursorTile = _tilemap->at(i, j);
      TerrainTile& terrain = cursorTile.get_terrain();
      if( terrain.isDestructible() )
      {
         Picture &pic_clear = PicLoader::instance().get_picture( "oc3_land", 2 );

         LandOverlay* overlay = terrain.getOverlay();
         if (overlay == NULL)
         {
            // this is maybe a lonely tree
            Tile* tile = new Tile(_tilemap->at(i, j));  // make a copy of tile
            tile->set_picture(&pic_clear);
            tile->set_master_tile(NULL);  // single tile
            _d->postTiles.push_back( tile );
            //_priorityTiles.push_back(&tile);
         }
         else
         {
            // remove the overlay, and make single tile of cleared land
            int size = overlay->getSize();
            int mi = overlay->getTile().getI(); // master I
            int mj = overlay->getTile().getJ(); // master J

            for (int dj = 0; dj<size; ++dj)
            {
               for (int di = 0; di<size; ++di)
               {
                  Tile* tile = new Tile(_tilemap->at(mi+di, mj+dj));  // make a copy of tile

                  tile->set_picture(&pic_clear);
                  tile->set_master_tile(NULL);  // single tile
                  TerrainTile &terrain = tile->get_terrain();
                  terrain.setOverlay(NULL);
                  //_priorityTiles.push_back(&tile);
               }
            }
         }
      }
   }
}




Tile& GuiTilemap::getTileIJ(const int i, const int j)
{
   return _tilemap->at(i, j);
}

