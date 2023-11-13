#ifndef BALLOON_H
#define BALLOON_H
#include <box2d/box2d.h>
#include <QPointF>

class GameScene;
class b2World;
class QGraphicsPixmapItem;
class Balloon
{
public:
    Balloon(int pos_x, int pos_y, b2World* world);
    Balloon(b2World* world);
    ~Balloon();
    void draw(GameScene& scene);
    b2Body* body() const{return m_body;}
    QPointF pos() const;
private:
    b2BodyDef m_bodyDef;
    b2FixtureDef m_fixtureDef;
    b2Body* m_body;
    QGraphicsPixmapItem* m_pixmapItem;
    QPointF m_point;
};

#endif // BALLOON_H
