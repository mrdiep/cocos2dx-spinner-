#include "GameMenu.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
#include "GameLevels.h"
USING_NS_CC;

Scene* GameMenu::createScene()
{
    Scene * scene = Scene::create();
    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 255));
    auto layer = GameMenu::create();
    scene->addChild(layerColor, 1);
    scene->addChild(layer, 2);
    return scene;
}

int GameMenu::currentScore = 0;
int GameMenu::highestScore = 0;

bool GameMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

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

    Label * scoreLabel = Label::createWithTTF(std::to_string(GameMenu::currentScore), "fonts/TrulyMadlyDpad-a72o.ttf", 85);
    scoreLabel->setPosition(Vec2(center_screen_x, center_screen_y + visibleSize.y * 0.2));
    scoreLabel->setTextColor(Color4B(0, 189, 241, 255));
    this->addChild(scoreLabel);

    std::string subLabel = GameMenu::currentScore >= GameMenu::highestScore ? "new best" : "best " + std::to_string(GameMenu::highestScore);

    Label* bestLabel = Label::createWithTTF(subLabel, "fonts/TrulyMadlyDpad-a72o.ttf", 30);
    bestLabel->setPosition(scoreLabel->getPosition() - Vec2(0, scoreLabel->getBoundingBox().size.height / 2 + bestLabel->getBoundingBox().size.height * 0.6));
    bestLabel->setTextColor(Color4B(0, 189, 241, 255));
    this->addChild(bestLabel);

    ui::Button* newgame = createButton("new game");
    newgame->setPosition(Vec2(center_screen_x, center_screen_y));
    newgame->addClickEventListener(CC_CALLBACK_0(GameMenu::newGame, this));
    Vec2 margin = Vec2(0, newgame->getBoundingBox().size.height * 1.2);

    ui::Button* levels = createButton("levels");
    levels->setPosition(newgame->getPosition() - margin);
    levels->addClickEventListener(CC_CALLBACK_0(GameMenu::openLevel, this));

    ui::Button* shareFriend = createButton("share friend");
    shareFriend->setPosition(levels->getPosition() - margin);
    shareFriend->addClickEventListener(CC_CALLBACK_0(GameMenu::newGame, this));

    ui::Button* rating = createButton("rating");
    rating->setPosition(shareFriend->getPosition() - margin);
    rating->addClickEventListener(CC_CALLBACK_0(GameMenu::newGame, this));

    ui::Button* quitgame = createButton("quit game");
    quitgame->setPosition(rating->getPosition() - margin);
    quitgame->addClickEventListener(CC_CALLBACK_0(GameMenu::newGame, this));

    return true;
}
ui::Button* GameMenu::createButton(std::string title) {
    ui::Button* button = ui::Button::create("button-background.png", "button-background-pressed.png", "button-background.png");
    Label* buttonLabel = Label::createWithTTF(title, "fonts/TrulyMadlyDpad-a72o.ttf", 22);
    buttonLabel->setPosition(button->getBoundingBox().size / 2);
    buttonLabel->setTextColor(Color4B(0, 189, 241, 255));
    button->addChild(buttonLabel);

    button->setScale(scaleX);
    this->addChild(button);

    return button;
}
void GameMenu::newGame() {
    Director::getInstance()->replaceScene(GameScene::createScene());
}

void GameMenu::openLevel() {
    Director::getInstance()->pushScene(GameLevels::createScene());
}

