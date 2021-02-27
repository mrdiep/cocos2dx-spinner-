#include "GameScene.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "GameMenu.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    Scene * scene = Scene::create();
    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 255));
    auto layer = GameScene::create();
    scene->addChild(layerColor, 1);
    scene->addChild(layer, 2);
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    loadSettings();
    auto listener2 = EventListenerTouchOneByOne::create();
    listener2->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);

    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    center_screen_x = origin.x + visibleSize.x / 2;
    center_screen_y = origin.y + visibleSize.y / 2;

    Sprite* background = Sprite::create("background.png");
    scaleX = visibleSize.x / background->getBoundingBox().size.width;
    float scaleY = visibleSize.y / background->getBoundingBox().size.height;
    background->setScaleX(scaleX);
    background->setScaleY(scaleY);
    background->setPosition(center_screen_x, center_screen_y);
    this->addChild(background);

    Sprite* circle = Sprite::create("circle.png");
    circle->setOpacity(15);
    circle->setScale(scaleX);
    circle->setPosition(center_screen_x, center_screen_y);
    this->addChild(circle);

    rotateLayer = LayerColor::create();
    this->addChild(rotateLayer);

    float angleDiff = 2 * 3.14159265359 / ballNumber;
    float angle = 0;
    for (int i = 0; i < ballNumber;i++) {
        Sprite * ball1 = Sprite::create("ball-green.png");
        ball1->setScale(scaleX);
        ballRadius = ball1->getBoundingBox().size.width;
        balls.push_back(ball1);
        float radius = visibleSize.x * 0.3;

        float tx = center_screen_x + sin(angle) * radius;
        float ty = center_screen_y + cos(angle) * radius;
        ball1->setPosition(tx, ty);
        this->addChild(ball1);
        angle += angleDiff;
        ball1->setRotation(angle);
    }

    scoreLabel = Label::createWithTTF("0", "fonts/TrulyMadlyDpad-a72o.ttf", 60);
    scoreLabel->setPosition(Vec2(center_screen_x, center_screen_y + visibleSize.y * 0.4));
    scoreLabel->setTextColor(Color4B(0, 189, 241, 255));
    this->addChild(scoreLabel);

    Label * hintLabel = Label::createWithTTF("Tap screen to change ball direction\r\nHit the green ball to earn score\r\nAvoid the white ball", "fonts/TrulyMadlyDpad-a72o.ttf", 16);
    hintLabel->setPosition(Vec2(center_screen_x, origin.y + hintLabel->getBoundingBox().size.height * 0.6));
    hintLabel->setTextColor(Color4B(0, 189, 241, 255));
    this->addChild(hintLabel);
    hintLabel->runAction(Sequence::create(
        DelayTime::create(10),
        CallFuncN::create([&](Node* node) {
            node->removeFromParentAndCleanup(true);
            }),
        NULL));

    this->schedule(CC_CALLBACK_1(GameScene::spawNewObject, this), emitRate, 2000000000, 1, "spawNew");
    this->scheduleUpdate();
    return true;
}

void GameScene::loadSettings() {
    auto fs = FileUtils::getInstance();
    auto settingPath = fs->getWritablePath() + "/settings.txt";
    int level = 4;
    ballNumber = 2;
    if (fs->isFileExist(settingPath)) {
        std::string key = fs->getStringFromFile(settingPath);
        level = atoi(key.c_str());

        ballNumber = level % 10;
        level = level / 10;
    }

    float floatMaxEmmitRate = 2.0f; // level 1
    float floatMinEmmitRate = 1.0f; // level 7

    emitRate = (7 - level)* (floatMaxEmmitRate - floatMinEmmitRate) / 6 + 1;
    rotate_speed = (level) * (6 - 2) / 6 + 1;
    velocity_ball = (7 -level) * 2 / 6 + 1;
    CCLOG("%d = %f", level, emitRate);

}

void GameScene::spawNewObject(float x)
{
    bool isScoreObject = rand() % 100 < 30;
    Sprite* object = Sprite::create(isScoreObject ? "ball-green.png" : "ball-white.png");
    object->setScale(0);
    object->setTag(isScoreObject ? 1 : 2);
    object->setPosition(center_screen_x, center_screen_y);
    int angle = rand() % 360;
    float radius = visibleSize.x * 0.3 + ballRadius;

    float tx = center_screen_x + sin(angle * 3.1415 / 180.0) * radius;
    float ty = center_screen_y + cos(angle * 3.1415 / 180.0) * radius;
    object->runAction(RepeatForever::create(RotateBy::create(isScoreObject ? 2: 0.7, 360)));
    object->runAction(Sequence::create(
        ScaleTo::create(0.5, scaleX),
        MoveTo::create(velocity_ball, Vec2(tx, ty)),
        ScaleBy::create(0.3, 0),
        CallFuncN::create([&](Node* node) {
            node->removeFromParentAndCleanup(true);
            }),
        NULL));
    this->addChild(object);
}

void GameScene::update(float fDelta)
{
    auto childs = this->getChildren();
    for (auto ball : balls) {
       ball->setRotation(ball->getRotation() + (0.01744444444) * rotate_speed * direction);
        float radius = visibleSize.x * 0.3;

        float tx = center_screen_x + sin(ball->getRotation()) * radius;
        float ty = center_screen_y + cos(ball->getRotation()) * radius;

        ball->setPosition(tx, ty);
        for (auto c : childs) {
            if (c->getTag() == 1) {
                float distance = ball->getPosition().getDistance(c->getPosition());
                if (distance < 1.12 * ballRadius) {
                    ball->setScale(ball->getScale());
                    c->stopAllActions();
                    auto particleSystem = ParticleSystemQuad::create("effect-exlode.plist");
                    particleSystem->setPosition(c->getPosition());
                    this->addChild(particleSystem);

                    c->removeFromParentAndCleanup(true);
                    scoreLabel->setString(std::to_string(++score));
                }
            } else if (c->getTag() == 2) {
                float distance = ball->getPosition().getDistance(c->getPosition());
                if (distance < 1.12 * ballRadius) {
                    c->stopAllActions();
                    ball->removeFromParentAndCleanup(true);

                    auto particleSystem = ParticleSystemQuad::create("effect-exlode.plist");
                    particleSystem->setPosition(ball->getPosition());
                    this->addChild(particleSystem);
                    this->unscheduleUpdate();
                    this->unschedule("spawNew");

                    this->scheduleOnce(CC_CALLBACK_0(GameScene::newGame, this), 3.0f, "newScene");
                    for (auto c : childs) {
                        if (c->getTag() == 1 || c->getTag() == 2) {
                            c->stopAllActions();
                        }
                    }
                }
            }
        }
    }
}

void GameScene::newGame() {
    int hScore = score;

    auto fs = FileUtils::getInstance();
    auto settingPath = fs->getWritablePath() + "/highest-score.txt";
    if (fs->isFileExist(settingPath)) {
        std::string key = fs->getStringFromFile(settingPath);
        hScore = atoi(key.c_str());
    }

    if (score >= hScore) {
        fs->writeStringToFile(std::to_string(score), settingPath);
    }

    GameMenu::currentScore = score;
    GameMenu::highestScore = hScore;
    Director::getInstance()->replaceScene(GameMenu::createScene());
}

bool GameScene::onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    CCLOG("Touch At: %f, %f", touch->getLocation().x, touch->getLocation().y);
    direction = -1 * direction;
    return true;
}
