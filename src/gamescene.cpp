#include "gamescene.h"
#include "utils.h"
#include "pixmapmanager.h"
#include "fontmanager.h"
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QDir>
#include <QPainter>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_world(b2Vec2(0.0f,-3.0f)), m_balloonsMax(100), m_counter(20.0f), m_isRunning(true),
    m_level(0)
{
    onUserCreate();
}

GameScene::~GameScene()
{

}

void GameScene::regenerateBalloons()
{
    foreach(Balloon* balloon, m_balloons)
    {
        m_world.DestroyBody(balloon->body());
        m_balloons.removeOne(balloon);
    }
    for(int i = 0; i < m_balloonsMax; ++i)
    {
        m_balloons.push_back(new Balloon(&m_world));
    }
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_loopTime += m_deltaTime;
    while(m_loopTime > m_loopSpeed)
    {
        m_loopTime -= m_loopSpeed;

        if(m_isRunning){
            handlePlayerInput();

            if(m_mouse->m_released){
                bool isShot = false;
                foreach(Balloon* balloon, m_balloons)
                {
                    if(m_mouse->m_x > balloon->pos().x() && m_mouse->m_x < balloon->pos().x() + 52 &&
                        m_mouse->m_y > balloon->pos().y() && m_mouse->m_y < balloon->pos().y() + 52){
                        m_world.DestroyBody(balloon->body());
                        m_balloons.removeOne(balloon);
                        m_poppingSFX->play();
                        isShot = true;
                    }
                }
                if(!isShot)
                {
                    if(!m_gunshotSFX->isPlaying())
                    {
                        m_gunshotSFX->play();
                    }
                }
                if(!m_balloons.size())
                {
                    m_isRunning = false;
                    m_result = RESULT::Win;
                }
            }

            m_counter -= (m_loopSpeed/1000.0f);
            if(m_counter < 0.0f)
            {
                m_isRunning = false;
                m_result = RESULT::Lost;
            }
            updatePhysics();

            clear();
            QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
            pItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Background));
            addItem(pItem);
            m_leftWall->draw(*this);
            m_rightWall->draw(*this);
            m_topWall->draw(*this);
            foreach (Balloon* balloon, m_balloons) {
                balloon->draw(*this);
            }
            drawGUI();
            drawResult();
            drawCrosshair();
        }
        else{
            if(m_mouse->m_released){
                if(m_result == RESULT::Win)
                {
                    m_counter = 20.0f;
                    m_level++;
                    m_counter += m_level;
                    m_balloonsMax += 5;
                    regenerateBalloons();
                }
                else
                {
                    m_counter = 20.0f;
                    regenerateBalloons();
                }

                m_result = RESULT::None;
                m_isRunning = true;
            }
        }
        resetStatus();
    }
}

void GameScene::onUserCreate()
{
    setBackgroundBrush(COLOR_STYLE::BACKGROUND);
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i] = new KeyStatus();
    }
    m_mouse = new MouseStatus();
    setSceneRect(0,0, SCREEN::PHYSICAL_SIZE.width(), SCREEN::PHYSICAL_SIZE.height());
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/FPS));
    m_elapsedTimer.start();
    m_leftWall = new Wall(-10,0,10,SCREEN::PHYSICAL_SIZE.height()+100, &m_world);
    m_topWall  = new Wall(0,-10,SCREEN::PHYSICAL_SIZE.width(),10, &m_world);
    m_rightWall = new Wall(SCREEN::PHYSICAL_SIZE.width(), 0, 10, SCREEN::PHYSICAL_SIZE.height()+100, &m_world);

    for(int i = 0; i < m_balloonsMax; ++i)
    {
        m_balloons.push_back(new Balloon(&m_world));
    }

    m_gunshotSFX = new QSoundEffect();
    m_gunshotSFX->setSource(QUrl("qrc:/res/snd/pistol.wav"));
    m_gunshotSFX->setVolume(0.25f);
    m_poppingSFX = new QSoundEffect();
    m_poppingSFX->setSource(QUrl("qrc:/res/snd/pop1.wav"));
    m_poppingSFX->setVolume(0.75f);
}

void GameScene::renderScene()
{
    static int index = 0;
    QString fileName = QDir::currentPath() + QDir::separator() + "screen" + QString::number(index++) + ".png";
    QRect rect = sceneRect().toAlignedRect();
    QImage image(rect.size(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    render(&painter);
    image.save(fileName);
    qDebug() << "saved " << fileName;
}

void GameScene::handlePlayerInput()
{
    if(m_keys[KEYBOARD::KeysMapper[Qt::Key_Z]]->m_released)
    {
        renderScene();//uncoment if want to make screenshots
    }
    if(m_keys[KEYBOARD::KeysMapper[Qt::Key_R]]->m_released)
    {
        regenerateBalloons();
    }
    else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_P]]->m_released)
    {
        m_isRunning = !m_isRunning;
    }
    else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_S]]->m_released)
    {
    }
    else if(m_keys[KEYBOARD::KeysMapper[Qt::Key_W]]->m_released)
    {
    }
}

void GameScene::resetStatus()
{
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_released = false;
    }
    for(int i = 0; i < 256; ++i)
    {
        m_keys[i]->m_pressed = false;
    }
    m_mouse->m_released = false;
}

void GameScene::updatePhysics()
{
    m_world.Step(1000.0f/100000, 8, 3);
}

void GameScene::drawGUI()
{
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::GUI);
    font.setPixelSize(45);
    font.setBold(true);

    QGraphicsSimpleTextItem* counterTextItem = new QGraphicsSimpleTextItem();
    counterTextItem->setText("Hurry Up - " + QString::number(int(m_counter)) + " sec");
    counterTextItem->setFont(font);
    counterTextItem->setPos(15, 600);
    counterTextItem->setPen(QPen(Qt::green, 2));
    counterTextItem->setBrush(QBrush(Qt::black));

    addItem(counterTextItem);

    QGraphicsSimpleTextItem* resultTextItem = new QGraphicsSimpleTextItem();
    resultTextItem->setText(QString::number(m_balloons.size()).left(4) + " balloons");
    resultTextItem->setFont(font);
    resultTextItem->setPos(SCREEN::PHYSICAL_SIZE.width() - resultTextItem->boundingRect().width() - 15, 600);
    resultTextItem->setPen(QPen(Qt::green, 2));
    resultTextItem->setBrush(QBrush(Qt::black));
    addItem(resultTextItem);
}

void GameScene::drawResult()
{
    QFont font = FontManager::Instance()->getFont(FontManager::FontID::GUI);
    font.setPixelSize(35);
    if(m_result == RESULT::Win)
    {
        QGraphicsRectItem* rItem = new QGraphicsRectItem();
        //rItem;960, 640
        rItem->setRect(-860/2, -560/2, 860, 560);
        rItem->setPen(QPen(Qt::red, 5));
        rItem->setBrush(QBrush(Qt::black));
        rItem->setPos(480, 320);

        QGraphicsSimpleTextItem* tItem = new QGraphicsSimpleTextItem(rItem);
        tItem->setPen(QPen(Qt::white));
        tItem->setBrush(QBrush(Qt::white));
        tItem->setFont(font);
        tItem->setText("You Win !");
        tItem->setPos(-tItem->boundingRect().width()/2, -tItem->boundingRect().height()/2);

        QGraphicsSimpleTextItem* t1Item = new QGraphicsSimpleTextItem(rItem);
        t1Item->setPen(QPen(Qt::white));
        t1Item->setBrush(QBrush(Qt::white));
        t1Item->setFont(font);
        t1Item->setText("Click to go next level");
        t1Item->setPos(-t1Item->boundingRect().width()/2, -t1Item->boundingRect().height()/2 + tItem->boundingRect().height());

        addItem(rItem);
    }
    else if(m_result == RESULT::Lost)
    {
        QGraphicsRectItem* rItem = new QGraphicsRectItem();
        //rItem;960, 640
        rItem->setRect(-860/2, -560/2, 860, 560);
        rItem->setPen(QPen(Qt::red, 5));
        rItem->setBrush(QBrush(Qt::black));
        rItem->setPos(480, 320);
        QGraphicsSimpleTextItem* tItem = new QGraphicsSimpleTextItem(rItem);
        tItem->setPen(QPen(Qt::white));
        tItem->setBrush(QBrush(Qt::white));
        tItem->setFont(font);
        tItem->setText("You Lost !");
        tItem->setPos(-tItem->boundingRect().width()/2, -tItem->boundingRect().height()/2);

        QGraphicsSimpleTextItem* t1Item = new QGraphicsSimpleTextItem(rItem);
        t1Item->setPen(QPen(Qt::white));
        t1Item->setBrush(QBrush(Qt::white));
        t1Item->setFont(font);
        t1Item->setText("Click to restart");
        t1Item->setPos(-t1Item->boundingRect().width()/2, -t1Item->boundingRect().height()/2 + tItem->boundingRect().height());

        addItem(rItem);
    }
}

void GameScene::drawCrosshair()
{
    QGraphicsPixmapItem* pItem = new QGraphicsPixmapItem();
    pItem->setPixmap(PixmapManager::Instance()->getPixmap(PixmapManager::TextureID::Crosshair));
    pItem->setPos(m_mouse->m_x - 16, m_mouse->m_y - 16);
    addItem(pItem);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
        }
        else
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = true;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held    = false;
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if(KEYBOARD::KeysMapper.contains(event->key()))
    {
        if(!event->isAutoRepeat())
        {
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_held = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_pressed = false;
            m_keys[KEYBOARD::KeysMapper[event->key()]]->m_released = true;
        }

    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = true;
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    QGraphicsScene::mouseMoveEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_mouse->m_x = event->scenePos().x();
    m_mouse->m_y = event->scenePos().y();
    m_mouse->m_pressed = false;
    m_mouse->m_released = true;
    QGraphicsScene::mouseReleaseEvent(event);
}
