#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QElapsedTimer>
#include <QTimer>
#include <QImage>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QSoundEffect>
#include <box2d/box2d.h>
#include "wall.h"
#include "balloon.h"

struct KeyStatus
{
    bool m_pressed = false;
    bool m_held = false;
    bool m_released = false;
};

struct MouseStatus
{
    float m_x = 0.0f;
    float m_y = 0.0f;
    bool m_released = false;
    bool m_pressed = false;
};

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();
    void regenerateBalloons();
    enum RESULT {None, Win, Lost};
private slots:
    void loop();

private:
    void onUserCreate();
    void renderScene();
    void handlePlayerInput();
    void resetStatus();
    void updatePhysics();
    void drawGUI();
    void drawResult();
    void drawCrosshair();
    KeyStatus* m_keys[256];
    MouseStatus* m_mouse;
    const int FPS = 60;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f, m_loopTime = 0.0f;
    const float m_loopSpeed = int(1000.0f/FPS);

    b2World m_world;
    Wall *m_leftWall, *m_rightWall, *m_topWall;
    QList<Balloon*> m_balloons;
    int m_balloonsMax;
    float m_counter;
    bool m_isRunning;
    RESULT m_result;
    int m_level;
    QSoundEffect *m_gunshotSFX, *m_poppingSFX;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};


#endif // GAMESCENE_H
