//
//  PopUp.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 8/11/14.
//
//

#include "PopUp.h"

PopUp* PopUp::Instance;

PopUp::PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler callback2){
    Instance = this;
    Vector<MenuItem*> menuItems;
    
    shade = MainMenu::CreateButton("shade.png", Vec2(0,0), Vec2(0,0));
    shade->setScale(1,1);
    shade->setScale(MainMenu::screenSize.x / shade->getBoundingBox().size.width, MainMenu::screenSize.y / shade->getBoundingBox().size.height);
    shade->setPosition(0,0);
    shade->setAnchorPoint(Vec2(0,0));
    shade->setGlobalZOrder(4);
    menuItems.pushBack(shade);
    
    background = MainMenu::CreateButton("PUBackground.png", Vec2(0,0), Vec2(0,0));
    background->setGlobalZOrder(5);
    background->setPosition(Vec2(MainMenu::screenSize.x / 2.0, MainMenu::screenSize.y / 2.0));
    background->setAnchorPoint(Vec2(0.5,0.5));
    menuItems.pushBack(background);
    
    acceptButton = MainMenu::CreateButton("PUAccept.png", ref, callback, Vec2(0,0), Vec2(0,0));
    acceptButton->setPosition(background->getBoundingBox().getMidX() + (MainMenu::screenSize.x * 0.03), background->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    acceptButton->setAnchorPoint(Vec2(0,0));
    menuItems.pushBack(acceptButton);
    
    declineButton = MainMenu::CreateButton("PUDecline.png", ref, callback2, Vec2(0,0), Vec2(0,0));
    declineButton->setPosition(background->getBoundingBox().getMidX() - (MainMenu::screenSize.x * 0.03), background->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    declineButton->setAnchorPoint(Vec2(1,0));
    menuItems.pushBack(declineButton);
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setAnchorPoint(Vec2(0.5,1));
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMidY());
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    nameBox = nullptr;
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}
PopUp::PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler callback2, bool taylor_is_such_a_good_person){
    Instance = this;
    Vector<MenuItem*> menuItems;
    
    shade = MainMenu::CreateButton("shade.png", Vec2(0,0), Vec2(0,0));
    shade->setScale(1,1);
    shade->setScale(MainMenu::screenSize.x / shade->getBoundingBox().size.width, MainMenu::screenSize.y / shade->getBoundingBox().size.height);
    shade->setPosition(0,0);
    shade->setAnchorPoint(Vec2(0,0));
    shade->setGlobalZOrder(4);
    menuItems.pushBack(shade);
    
    background = MainMenu::CreateButton("PUBackground.png", Vec2(0,0), Vec2(0,0));
    background->setGlobalZOrder(5);
    background->setPosition(Vec2(MainMenu::screenSize.x / 2.0, MainMenu::screenSize.y / 2.0));
    background->setAnchorPoint(Vec2(0.5,0.5));
    menuItems.pushBack(background);
    
    acceptButton = MainMenu::CreateButton("PUAccept.png", ref, callback, Vec2(0,0), Vec2(0,0));
    acceptButton->setPosition(background->getBoundingBox().getMidX() + (MainMenu::screenSize.x * 0.03), background->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    acceptButton->setAnchorPoint(Vec2(0,0));
    menuItems.pushBack(acceptButton);
    
    declineButton = MainMenu::CreateButton("PUDecline.png", ref, callback2, Vec2(0,0), Vec2(0,0));
    declineButton->setPosition(background->getBoundingBox().getMidX() - (MainMenu::screenSize.x * 0.03), background->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    declineButton->setAnchorPoint(Vec2(1,0));
    menuItems.pushBack(declineButton);
    
    Size editBoxSize = Size(750*MainMenu::screenScale.x, 100 * MainMenu::screenScale.y);
    Scale9Sprite* nameBoxBG = Scale9Sprite::create("line.png");
    nameBoxBG->setContentSize(editBoxSize);
    nameBox = EditBox::create(Size(350,50), nameBoxBG);
    nameBox->setPosition(Point(MainMenu::screenSize.x / 2.0, background->getBoundingBox().getMidY() - (MainMenu::screenSize.y * 0.07)));
    nameBox->setFontSize(30);
    nameBox->setMaxLength(18);
    nameBox->setPlaceHolder("level name");
    nameBox->setFontColor(Color3B::WHITE);
    nameBox->setGlobalZOrder(7);
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMidY() + (MainMenu::screenSize.y * 0.1));
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setAnchorPoint(Vec2(0.5,1));
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}

PopUp::PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback){
    Instance = this;
    Vector<MenuItem*> menuItems;
    
    shade = MainMenu::CreateButton("shade.png", Vec2(0,0), Vec2(0,0));
    shade->setScale(1,1);
    shade->setScale(MainMenu::screenSize.x / shade->getBoundingBox().size.width, MainMenu::screenSize.y / shade->getBoundingBox().size.height);
    shade->setPosition(0,0);
    shade->setAnchorPoint(Vec2(0,0));
    shade->setGlobalZOrder(4);
    menuItems.pushBack(shade);
    
    background = MainMenu::CreateButton("PUBackground.png", Vec2(0,0), Vec2(0,0));
    background->setGlobalZOrder(5);
    background->setPosition(Vec2(MainMenu::screenSize.x / 2.0, MainMenu::screenSize.y / 2.0));
    background->setAnchorPoint(Vec2(0.5,0.5));
    menuItems.pushBack(background);
    
    acceptButton = MainMenu::CreateButton("PUAccept.png", ref, callback, Vec2(0,0), Vec2(0,0));
    acceptButton->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.1));
    acceptButton->setAnchorPoint(Vec2(0.5,0));
    menuItems.pushBack(acceptButton);
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMidY() - (0.05 * MainMenu::screenSize.y));
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(background->getBoundingBox().getMidX(), background->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setAnchorPoint(Vec2(0.5,1));
    nameBox = nullptr;
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}
void PopUp::Show(){
    //menu->setPosition(menu->getPosition().x, menu->getPosition().y + 1000);
    menu->setAnchorPoint(Vec2(0,0));
    name->setVisible(true);
    text->setVisible(true);
    menu->setVisible(true);
    menu->setEnabled(true);
    if(nameBox != nullptr){
        nameBox->setVisible(true);
        nameBox->setEnabled(true);
    }
}
void PopUp::Close(){
    menu->setAnchorPoint(Vec2(0,1));
    name->setVisible(false);
    text->setVisible(false);
    menu->setVisible(false);
    menu->setEnabled(false);
    if(nameBox != nullptr){
        nameBox->setVisible(false);
        nameBox->setEnabled(false);
    }
}
void PopUp::Add(Layer* layer){
    layer->addChild(menu,1);
    layer->addChild(name,1);
    layer->addChild(text,1);
    if(nameBox != nullptr){
        layer->addChild(nameBox,1);
    }
}
std::string PopUp::GetText(){
    return nameBox->getText();
}