#ifndef __GAMEMENU_SCENE_H__
#define __GAMEMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


class GameMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameMenu);
    static int currentScore;
    static int highestScore;
private:
    Vec2 visibleSize;
    int direction = 1;
    float scaleX;
    float center_screen_x;
    float center_screen_y;
    ui::Button* createButton(std::string name);
    void newGame();
    void openLevel();
};

#endif __GAMEMENU_SCENE_H__
