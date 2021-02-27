#include "GameLevels.h"
#include "GameScene.h"
#include "audio/include/AudioEngine.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

Scene* GameLevels::createScene()
{
    Scene * scene = Scene::create();
    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 255));
    auto layer = GameLevels::create();
    scene->addChild(layerColor, 1);
    scene->addChild(layer, 2);
    return scene;
}

int GameLevels::currentScore = 0;

bool GameLevels::init()
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
    background->setTag(2);
    this->addChild(background);

    Label * speedSelectLabel = Label::createWithTTF("speed", "fonts/TrulyMadlyDpad-a72o.ttf", 30);
    speedSelectLabel->setPosition(Vec2(center_screen_x, center_screen_y + visibleSize.y * 0.5 - speedSelectLabel->getBoundingBox().size.height));
    speedSelectLabel->setTextColor(Color4B(0, 189, 241, 255));
    this->addChild(speedSelectLabel);

    Node* preNode = speedSelectLabel;
    for (int i = 1;i <= 6;i++) {
        buttons[i - 1] = createCheckbox(std::to_string(i));
        buttons[i - 1]->setPosition(preNode->getPosition() - Vec2(0, preNode->getBoundingBox().size.height * 1.2));
        buttons[i - 1]->addClickEventListener(CC_CALLBACK_0(GameLevels::levelSelect, this, i));
        buttons[i - 1]->setPosition(Vec2(origin.x + visibleSize.x / 4, buttons[i - 1]->getPosition().y));
        i++;
        buttons[i - 1] = createCheckbox(std::to_string(i));
        buttons[i - 1]->setPosition(preNode->getPosition() - Vec2(0, preNode->getBoundingBox().size.height * 1.2));
        buttons[i - 1]->addClickEventListener(CC_CALLBACK_0(GameLevels::levelSelect, this, i));
        buttons[i - 1]->setPosition(Vec2(origin.x + visibleSize.x * 2.0 / 4.0, buttons[i - 1]->getPosition().y));

        i++;
        buttons[i - 1] = createCheckbox(std::to_string(i));
        buttons[i - 1]->setPosition(preNode->getPosition() - Vec2(0, preNode->getBoundingBox().size.height * 1.2));
        buttons[i - 1]->addClickEventListener(CC_CALLBACK_0(GameLevels::levelSelect, this, i));
        buttons[i - 1]->setPosition(Vec2(origin.x + visibleSize.x * 3.0 / 4.0, buttons[i - 1]->getPosition().y));

        preNode = buttons[i - 1];
    }

    Label* numberOfballLabel = Label::createWithTTF("balls", "fonts/TrulyMadlyDpad-a72o.ttf", 30);
    numberOfballLabel->setPosition(Vec2(center_screen_x,  preNode->getPosition().y - preNode->getBoundingBox().size.height * 2));
    numberOfballLabel->setTextColor(Color4B(0, 189, 241, 255));
    this->addChild(numberOfballLabel);

    preNode = numberOfballLabel;
    for (int i = 1 ;i <= 3;i++) {
        buttons_ball[i - 1] = createCheckbox(std::to_string(button_ball_size[i-1]));
        buttons_ball[i - 1]->setPosition(preNode->getPosition() - Vec2(0, preNode->getBoundingBox().size.height * 1.2));
        buttons_ball[i - 1]->addClickEventListener(CC_CALLBACK_0(GameLevels::ballSelect, this, button_ball_size[i - 1]));
        buttons_ball[i - 1]->setPosition(Vec2(origin.x + visibleSize.x / 4, buttons_ball[i - 1]->getPosition().y));
        i++;
        buttons_ball[i - 1] = createCheckbox(std::to_string(button_ball_size[i - 1]));
        buttons_ball[i - 1]->setPosition(preNode->getPosition() - Vec2(0, preNode->getBoundingBox().size.height * 1.2));
        buttons_ball[i - 1]->addClickEventListener(CC_CALLBACK_0(GameLevels::ballSelect, this, button_ball_size[i - 1]));
        buttons_ball[i - 1]->setPosition(Vec2(origin.x + visibleSize.x * 2.0 / 4.0, buttons_ball[i - 1]->getPosition().y));
        i++;
        buttons_ball[i - 1] = createCheckbox(std::to_string(button_ball_size[i - 1]));
        buttons_ball[i - 1]->setPosition(preNode->getPosition() - Vec2(0, preNode->getBoundingBox().size.height * 1.2));
        buttons_ball[i - 1]->addClickEventListener(CC_CALLBACK_0(GameLevels::ballSelect, this, button_ball_size[i - 1]));
        buttons_ball[i - 1]->setPosition(Vec2(origin.x + visibleSize.x * 3.0 / 4.0, buttons_ball[i - 1]->getPosition().y));

        preNode = buttons_ball[i - 1];
    }

    auto fs = FileUtils::getInstance();
    auto settingPath = fs->getWritablePath() + "/settings.txt";
    if (fs->isFileExist(settingPath)) {
        std::string key = fs->getStringFromFile(settingPath);
        level = atoi(key.c_str()) / 10;
        ball = atoi(key.c_str()) % 10;
        ballSelected = buttons_ball[ball == 2 ? 0 : ball == 3 ? 1 : 2];
        levelSelected = buttons[level - 1];
    }
    else {
        levelSelected = buttons[level - 1];
        ballSelected = buttons_ball[0];
    }


    levelSelected->setEnabled(false);
    ballSelected->setEnabled(false);

    auto buttonback = createButton("back");
    buttonback->setPosition(buttons_ball[1]->getPosition() - Vec2(0, buttonback->getBoundingBox().size.height * 2.5));
    buttonback->addClickEventListener(CC_CALLBACK_0(GameLevels::goBack, this));
    float centerY = (speedSelectLabel->getPosition().y + buttonback->getPosition().y) / 2;
    float shiftY = centerY - center_screen_y;

    for (auto child : getChildren()) {
        if (child->getTag() == 2) continue;
        child->setPosition(child->getPosition() - Vec2(0, shiftY));
    }

    return true;
}

void GameLevels::goBack() {
    Director::getInstance()->popScene();
}

void GameLevels::levelSelect(int level)
{
    GameLevels::level = level;
    levelSelected->setEnabled(true);
    levelSelected = buttons[level - 1];
    levelSelected->setEnabled(false);

    auto fs = FileUtils::getInstance();
    auto settingPath = fs->getWritablePath() + "/settings.txt";
    fs->writeStringToFile(std::to_string(level * 10 + GameLevels::ball), settingPath);
}

void GameLevels::ballSelect(int ball)
{
    GameLevels::ball = ball;
    ballSelected->setEnabled(true);
    ballSelected = buttons_ball[ball == 2 ? 0 : ball == 3 ? 1 :2 ];
    ballSelected->setEnabled(false);

    auto fs = FileUtils::getInstance();
    auto settingPath = fs->getWritablePath() + "/settings.txt";
    fs->writeStringToFile(std::to_string(GameLevels::level * 10 + ball), settingPath);
}

ui::Button* GameLevels::createCheckbox(std::string title) {
    ui::Button* button = ui::Button::create("check-box-normal.png", "check-box-checked.png", "check-box-checked.png");
    Label* buttonLabel = Label::createWithTTF(title, "fonts/TrulyMadlyDpad-a72o.ttf", 22);
    buttonLabel->setPosition(button->getBoundingBox().size / 2);
    buttonLabel->setTextColor(Color4B(0, 189, 241, 255));
    button->addChild(buttonLabel);

    button->setScale(scaleX);
    this->addChild(button);

    return button;
}

ui::Button* GameLevels::createButton(std::string title) {
    ui::Button* button = ui::Button::create("button-background.png", "button-background-pressed.png", "button-background.png");
    Label* buttonLabel = Label::createWithTTF(title, "fonts/TrulyMadlyDpad-a72o.ttf", 22);
    buttonLabel->setPosition(button->getBoundingBox().size / 2);
    buttonLabel->setTextColor(Color4B(0, 189, 241, 255));
    button->addChild(buttonLabel);

    button->setScale(scaleX);
    this->addChild(button);

    return button;
}
void GameLevels::newGame() {
    Director::getInstance()->replaceScene(GameScene::createScene());
}
