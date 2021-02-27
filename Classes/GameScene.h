#ifndef __GAME_MAIN_SCENE_H__
#define __GAME_MAIN_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;


class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    void update(float fDelta);
    CREATE_FUNC(GameScene);
private:
    std::vector<Node*> balls;
    float emitRate = 3.0f;
    float rotate_speed = 1.0;
    float velocity_ball = 0;
    int ballNumber = 2;
    Label* scoreLabel;
    int score = 0;
    LayerColor * rotateLayer;
    Vec2 visibleSize;
    int direction = 1;
    float scaleX, ballRadius;
    float center_screen_x;
    float center_screen_y;
    void spawNewObject(float x);
    bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void newGame();
    void loadSettings();
};

#endif __GAME_MAIN_SCENE_H__
