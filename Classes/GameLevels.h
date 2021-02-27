#ifndef __GAME_LEVEL_SCENE_H__
#define __GAME_LEVEL_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


class GameLevels : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameLevels);
    static int currentScore;
private:
    ui::Button* buttons[6];
    ui::Button* buttons_ball[3];
    int button_ball_size[3] = { 2,3,5 };
    Vec2 visibleSize;
    int direction = 1;
    ui::Button* levelSelected;
    ui::Button* ballSelected;
    int level = 4;
    int ball = 2;
    float scaleX;
    float center_screen_x;
    float center_screen_y;
    void newGame();
    ui::Button* createCheckbox(std::string title);
    ui::Button* createButton(std::string title);
    void levelSelect(int level);
    void ballSelect(int ball);

    void goBack();
};

#endif __GAME_LEVEL_SCENE_H__
