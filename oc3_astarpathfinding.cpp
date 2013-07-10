#include "oc3_astarpathfinding.hpp"
#include "oc3_tilemap.hpp"
#include "oc3_positioni.hpp"
#include "oc3_astarpoint.hpp"
#include "oc3_path_finding.hpp"
#include "oc3_stringhelper.hpp"

using namespace std;

class Pathfinder::Impl
{
public:
};

const AStarPoint Pathfinder::invalidPoint = AStarPoint( 0 ); 

Pathfinder::Pathfinder()
{
}

void Pathfinder::update( const Tilemap& tilemap )
{
  _tilemap = const_cast< Tilemap* >( &tilemap );

  PtrTilesList tiles = _tilemap->getFilledRectangle( TilePos( 0, 0 ), Size( _tilemap->getSize() ) );

  for( PtrTilesList::iterator i=tiles.begin(); i != tiles.end(); i++ )
  {
    if ( !pointExists( (*i)->getIJ() ) )
    {
      grid[ (*i)->getI() ][ (*i)->getJ() ] = new AStarPoint( *i );
    }
  }
}

bool Pathfinder::getPath( const Tile& start, const Tile& stop, PathWay& oPathWay,
                          int flags, const Size& arrivedArea )
{
  return getPath( start.getIJ(), stop.getIJ(), oPathWay, flags, arrivedArea );
}

bool Pathfinder::getPath( const TilePos& start, const TilePos& stop,  
                          PathWay& oPathWay, int flags,
                          const Size& arrivedArea )
{
  if( (flags & checkStart) && !pointIsWalkable( start ) )
      return false;

  return aStar( start, stop, arrivedArea, oPathWay, flags );
}

std::list<AStarPoint*> Pathfinder::getTraversingPoints( const TilePos& start, const TilePos& stop )
{
  std::list<AStarPoint*> points;

  if( start == stop )
    return points;

  points.push_back( getPoint( start ) );

  TilePos housePos = stop;               
  while( points.back()->getPos() != housePos )
  {
    TilePos ij = points.back()->getPos();
    TilePos move( math::clamp( housePos.getI() - ij.getI(), -1, 1 ), math::clamp( housePos.getJ() - ij.getJ(), -1, 1 ) );
    points.push_back( getPoint( ij + move ) );
  }

  return points;
}

unsigned int Pathfinder::getMaxLoopCount() const
{
  return 600;
}

bool Pathfinder::aStar( const TilePos& startPos, const TilePos& stopPos, 
                        const Size& arrivedArea, PathWay& oPathWay,
                        int flags )
{
  oPathWay.init( *_tilemap, _tilemap->at( startPos ) );

  // Define points to work with
  AStarPoint* start = getPoint( startPos );
  AStarPoint* end = getPoint( stopPos );
  AStarPoint* current;
  AStarPoint* child;

  // Define the open and the close list
  list<AStarPoint*> openList;
  list<AStarPoint*> closedList;
  list<AStarPoint*>::iterator i;

  int tSize = _tilemap->getSize();
  map<AStarPoint*,AStarPoint::WalkableType> saveArrivedArea;

  TilePos arrivedAreaStart( math::clamp( stopPos.getI()-arrivedArea.getWidth(), 0, tSize ),
                            math::clamp( stopPos.getJ()-arrivedArea.getHeight(), 0, tSize) );

  TilePos arrivedAreaStop(  math::clamp( stopPos.getI()+arrivedArea.getWidth(), 0, tSize ),
                            math::clamp( stopPos.getJ()+arrivedArea.getHeight(), 0, tSize) );
  
  for( int i=arrivedAreaStart.getI(); i <= arrivedAreaStop.getI(); i++ )
  {
    for( int j=arrivedAreaStart.getJ(); j <= arrivedAreaStop.getJ(); j++ )
    {
      AStarPoint* ap = getPoint( TilePos( i, j) );
      if( ap )
      {
        saveArrivedArea[ ap ] = ap->priorWalkable;
        ap->priorWalkable = AStarPoint::alwaysWalkable;
      }
    }
  }

  unsigned int n = 0;

  // Add the start point to the openList
  openList.push_back(start);
  start->opened = true;

  while( n == 0 || (current != end && n < getMaxLoopCount() ))
  {
    // Look for the smallest F value in the openList and make it the current point
    for (i = openList.begin(); i != openList.end(); ++ i)
    {
      if (i == openList.begin() || (*i)->getFScore() <= current->getFScore())
      {
        current = (*i);
      }
    }

    // Stop if we reached the end
    if( current == end ) 
    {
      break;
    }

    // Remove the current point from the openList
    openList.remove(current);
    current->opened = false;

    // Add the current point to the closedList
    closedList.push_back(current);
    current->closed = true;

    // Get all current's adjacent walkable points
    for (int x = -1; x < 2; x ++)
    {
      for (int y = -1; y < 2; y ++)
      {
        // If it's current point then pass
        if (x == 0 && y == 0)
        {
          continue;
        }

        // Get this point
        child = getPoint( current->getPos() + TilePos( x, y ) );

        _OC3_DEBUG_BREAK_IF( !child );
        if( !child )
        {
          continue;
        }
        // If it's closed or not walkable then pass
        if( child->closed || !child->isWalkable() )
        {
          continue;
        }

        // If we are at a corner
        if (x != 0 && y != 0)
        {
          // if the next horizontal point is not walkable or in the closed list then pass
          //AStarPoint* tmpPoint = getPoint( current->pos + TilePos( 0, y ) );
          TilePos tmp = current->getPos() + TilePos( 0, y );
          if( !pointIsWalkable( tmp ) || getPoint( tmp )->closed)
          {
            continue;
          }

          tmp = current->getPos() + TilePos( x, 0 );
          // if the next vertical point is not walkable or in the closed list then pass
          if( !pointIsWalkable( tmp ) || getPoint( tmp )->closed)
          {
            continue;
          }
        }

        // If it's already in the openList
        if (child->opened)
        {
          // If it has a wroste g score than the one that pass through the current point
          // then its path is improved when it's parent is the current point
          if (child->getGScore() > child->getGScore(current))
          {
            // Change its parent and g score
            child->setParent(current);
            child->computeScores(end);
          }
        }
        else
        {
          // Add it to the openList with current point as parent
          openList.push_back(child);
          child->opened = true;

          // Compute it's g, h and f score
          child->setParent(current);
          child->computeScores(end);
        }
      }
    }

    n++;
  }

  // Reset
  for (i = openList.begin(); i != openList.end(); ++ i)
  {
    (*i)->opened = false;
  }
  for (i = closedList.begin(); i != closedList.end(); ++ i)
  {
    (*i)->closed = false;
  }

  for( map<AStarPoint*,AStarPoint::WalkableType>::iterator it=saveArrivedArea.begin(); it != saveArrivedArea.end(); it++ )
  {
    it->first->priorWalkable = it->second;
  }

  if( n == getMaxLoopCount() )
  {
    return false;
  }
  // Resolve the path starting from the end point
  list<AStarPoint*> lPath;
  while( current->hasParent() && current != start )
  {
    lPath.push_front( current );
    current = current->getParent();
    n++;
  }

  for( list<AStarPoint*>::iterator lpIt=lPath.begin(); lpIt != lPath.end(); lpIt++ )
  {
    oPathWay.setNextTile( _tilemap->at( (*lpIt)->getPos() ) );
  }

  return oPathWay.getLength() > 0;
}

AStarPoint* Pathfinder::getPoint( const TilePos& pos )
{
  if( pointExists( pos ))
  {
    return grid[pos.getI()][pos.getJ()];
  }
  else
  {
    StringHelper::debug( 0xff, "ERROR: failed to gather point (%d,%d) on grid", pos.getI(), pos.getJ() );
    return const_cast< AStarPoint* >( &invalidPoint );
  }
}

bool Pathfinder::pointExists( const TilePos& pos )
{
  return ( pos.getI() < (int)grid.size() && pos.getJ() < (int)grid[pos.getI()].size() );
}

bool Pathfinder::pointIsWalkable( const TilePos& pos )
{
  return (pointExists( pos ) && grid[ pos.getI() ][ pos.getJ() ]->isWalkable() );
}

Pathfinder& Pathfinder::getInstance()
{
  static Pathfinder inst;
  return inst;
}

Pathfinder::~Pathfinder()
{

}
