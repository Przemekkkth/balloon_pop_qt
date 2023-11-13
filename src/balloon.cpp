#include "balloon.h"
#include "utils.h"
#include "gamescene.h"
#include "pixmapmanager.h"
#include <QGraphicsRectItem>
#include <QRandomGenerator>

Balloon::Balloon(int pos_x, int pos_y, b2World* world)
{
    m_pixmapItem = new QGraphicsPixmapItem();
    m_pixmapItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Balloon));
    m_pixmapItem->setPos(pos_x, pos_y);


    PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Balloon).width();
    m_bodyDef.position.Set(CONVERTER::pixelsToMeters(pos_x), CONVERTER::pixelsToMeters(pos_y));
    m_bodyDef.type = b2_dynamicBody;

    b2CircleShape b2Circle;
    float radius = CONVERTER::pixelsToMeters(m_pixmapItem->pixmap().width()/2.0f-2.0f);
    b2Circle.m_radius = radius;

    m_fixtureDef.density = .5;
    m_fixtureDef.friction = 0.8;
    m_fixtureDef.restitution = .25;
    m_fixtureDef.shape = &b2Circle;

    m_body = world->CreateBody(&m_bodyDef);
    m_body->CreateFixture(&m_fixtureDef);

    intptr_t pointer = reinterpret_cast<intptr_t>(m_pixmapItem);
    m_body->GetUserData().pointer = pointer;
}

Balloon::Balloon(b2World *world)
{
    float pos_x = QRandomGenerator::global()->bounded(0, SCREEN::PHYSICAL_SIZE.width() - 52);
    float pos_y = SCREEN::PHYSICAL_SIZE.height() + QRandomGenerator::global()->bounded(0, 200);

    m_pixmapItem = new QGraphicsPixmapItem();
    m_pixmapItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Balloon));
    m_pixmapItem->setPos(pos_x, pos_y);

    PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Balloon).width();
    m_bodyDef.position.Set(CONVERTER::pixelsToMeters(pos_x), CONVERTER::pixelsToMeters(pos_y));
    m_bodyDef.type = b2_dynamicBody;

    b2CircleShape b2Circle;
    float radius = CONVERTER::pixelsToMeters(m_pixmapItem->pixmap().width())/2.0;
    b2Circle.m_radius = radius;

    m_fixtureDef.density = .5;
    m_fixtureDef.friction = 0.8;
    m_fixtureDef.restitution = .25;
    m_fixtureDef.shape = &b2Circle;

    m_body = world->CreateBody(&m_bodyDef);
    m_body->CreateFixture(&m_fixtureDef);

    intptr_t pointer = reinterpret_cast<intptr_t>(m_pixmapItem);
    m_body->GetUserData().pointer = pointer;
}

Balloon::~Balloon()
{
    delete m_pixmapItem;
}

void Balloon::draw(GameScene &scene)
{
    QGraphicsPixmapItem* rI = reinterpret_cast<QGraphicsPixmapItem*>(m_body->GetUserData().pointer);
    if(rI)
    {
        QGraphicsPixmapItem* newItem = new QGraphicsPixmapItem();
        float x = CONVERTER::metersToPixels(m_body->GetPosition().x);
        float y = CONVERTER::metersToPixels(m_body->GetPosition().y);
        newItem->setPixmap(rI->pixmap());
        newItem->setPos(x - rI->pixmap().width()/2.0f, y - rI->pixmap().height()/2.0f);
        m_point.setX(newItem->x());
        m_point.setY(newItem->y());
        scene.addItem(newItem);
    }
}

QPointF Balloon::pos() const
{
    return m_point;
}
