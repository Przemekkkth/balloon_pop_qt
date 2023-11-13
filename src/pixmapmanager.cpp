#include "pixmapmanager.h"

PixmapManager* PixmapManager::ptr = nullptr;

PixmapManager *PixmapManager::Instance()
{
    if(!ptr)
    {
        ptr = new PixmapManager();
    }
    return ptr;
}

QPixmap& PixmapManager::getPixmap(TextureID id)
{
    return m_textures.get(id);
}

PixmapManager::PixmapManager()
{
    m_textures.load(TextureID::Balloon,    ":/res/sprite/balloon.png");
    m_textures.load(TextureID::Background, ":/res/sprite/bg.png");
    m_textures.load(TextureID::Crosshair,  ":/res/sprite/crosshair.png");
}
