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


#include "oc3_menurgihtpanel.hpp"
#include "oc3_sdl_facade.hpp"
#include "oc3_picture.hpp"
#include "oc3_gfx_engine.hpp"

class MenuRigthPanel::Impl
{
public:
    Picture* picture; 
};

MenuRigthPanel::MenuRigthPanel( Widget* parent ) : Widget( parent, -1, Rect( 0, 0, 100, 100 ) ), _d( new Impl )
{
}

void MenuRigthPanel::draw( GfxEngine& engine )
{
    engine.drawPicture( *_d->picture, getScreenLeft(), getScreenTop() );
}

MenuRigthPanel* MenuRigthPanel::create( Widget* parent, const Rect& rectangle, const Picture& tilePic )
{
    MenuRigthPanel* ret = new MenuRigthPanel( parent );

    ret->setGeometry( rectangle );
    SdlFacade &sdlFacade = SdlFacade::instance();
    
    ret->_d->picture = &sdlFacade.createPicture( rectangle.getWidth(), rectangle.getHeight() );
    SDL_SetAlpha( ret->_d->picture->get_surface(), 0, 0 );  // remove surface alpha

    int y = 0;
    while( y <  ret->_d->picture->get_height() )
    {
        sdlFacade.drawPicture( tilePic, *ret->_d->picture, 0, y);
        y += tilePic.get_height();
    }

    return ret;
}