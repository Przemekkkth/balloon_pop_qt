#ifndef WALL_H
#define WALL_H
#include <box2d/box2d.h>


class GameScene;
class b2World;
class QGraphicsRectItem;
class Wall
{
public:
    Wall(int pos_x, int pos_y, int size_x, int size_y, b2World* world);
    void draw(GameScene& scene);
private:
    b2BodyDef m_bodyDef;
    b2FixtureDef m_fixtureDef;
    b2Body* m_body;
    QGraphicsRectItem* m_rectItem;
};

#endif // WALL_H
