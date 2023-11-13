#include "wall.h"
#include "utils.h"
#include "gamescene.h"
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>

Wall::Wall(int pos_x, int pos_y, int size_x, int size_y, b2World *world)
    : m_body(nullptr)
{
    m_bodyDef.position.Set(CONVERTER::pixelsToMeters(pos_x), CONVERTER::pixelsToMeters(pos_y));
    m_bodyDef.type = b2_staticBody;

    b2PolygonShape b2shape;
    float sx = CONVERTER::pixelsToMeters(size_x)/2.0;
    float sy = CONVERTER::pixelsToMeters(size_y)/2.0;
    b2shape.SetAsBox(sx,sy
                     ,b2Vec2(sx,sy),0);

    m_fixtureDef.density = 1.0;
    m_fixtureDef.friction = 0.8;
    m_fixtureDef.restitution= 0.1;
    m_fixtureDef.shape = &b2shape;

    m_rectItem = new QGraphicsRectItem(-size_x/2.0, -size_y/2.0, size_x, size_y);
    m_rectItem->setTransformOriginPoint(QPointF(0.0f, 0.0f));
    m_rectItem->setPos(QPointF(pos_x+size_x/2.0,pos_y+size_y/2.0));
    m_rectItem->setPen(QPen(QColor(50,50,50)));
    m_rectItem->setBrush(QBrush(QColor(50,50,50)));

    m_body = world->CreateBody(&m_bodyDef);
    m_body->CreateFixture(&m_fixtureDef);

    intptr_t pointer = reinterpret_cast<intptr_t>(m_rectItem);
    m_body->GetUserData().pointer = pointer;
}

void Wall::draw(GameScene &scene)
{
    if(!m_body)
    {
        return;
    }

    QGraphicsRectItem* rI = reinterpret_cast<QGraphicsRectItem*>(m_body->GetUserData().pointer);
    if(rI)
    {
        QGraphicsRectItem* newItem = new QGraphicsRectItem();
        newItem->setRect(rI->rect());
        newItem->setPos(rI->pos());
        newItem->setTransformOriginPoint(rI->transformOriginPoint());
        newItem->setPen(rI->pen());
        newItem->setBrush(rI->brush());
        scene.addItem(newItem);
    }
}
