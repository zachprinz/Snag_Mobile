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
    
    std::map<std::string, SEL_MenuHandler> callbacks;
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("popup", ref, callbacks, &menuItems, (cocos2d::Layer*)ref);
    elements["Shade"]->setGlobalZOrder(6);
    elements["Panel"]->setGlobalZOrder(7);

    acceptButton = MainMenu::CreateButton("popup", "Accept.png", ref, callback);
    acceptButton->setPosition(elements["Panel"]->getBoundingBox().getMidX() + (MainMenu::screenSize.x * 0.03), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    acceptButton->setAnchorPoint(Vec2(0,0));
    acceptButton->setGlobalZOrder(8);
    menuItems.pushBack(acceptButton);
    
    declineButton = MainMenu::CreateButton("popup", "Decline.png", ref, callback2);
    declineButton->setPosition(elements["Panel"]->getBoundingBox().getMidX() - (MainMenu::screenSize.x * 0.03), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    declineButton->setAnchorPoint(Vec2(1,0));
    declineButton->setGlobalZOrder(8);
    menuItems.pushBack(declineButton);
    
    labels.push_back(MainMenu::CreateLabel("Accept", Vec2(0,0), Vec2(0,0)));
    labels[0]->setPosition(acceptButton->getBoundingBox().getMidX(), acceptButton->getBoundingBox().getMidY());
    labels[0]->setGlobalZOrder(8);
    labels[0]->setAnchorPoint(Vec2(0.5,0.5));
    
    labels.push_back(MainMenu::CreateLabel("Decline", Vec2(0,0), Vec2(0,0)));
    labels[1]->setPosition(declineButton->getBoundingBox().getMidX(), declineButton->getBoundingBox().getMidY());
    labels[1]->setGlobalZOrder(8);
    labels[1]->setAnchorPoint(Vec2(0.5,0.5));
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setGlobalZOrder(8);
    name->setAnchorPoint(Vec2(0.5,1));
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMidY());
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    this->text->setGlobalZOrder(8);
    nameBox = nullptr;
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}
PopUp::PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler callback2, bool taylor_is_such_a_good_person){
    std::map<std::string, SEL_MenuHandler> callbacks;
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("popup", ref, callbacks, &menuItems, (cocos2d::Layer*)ref);
    elements["Shade"]->setGlobalZOrder(6);
    elements["Panel"]->setGlobalZOrder(7);
    
    acceptButton = MainMenu::CreateButton("popup", "Accept.png", ref, callback);
    acceptButton->setPosition(elements["Panel"]->getBoundingBox().getMidX() + (MainMenu::screenSize.x * 0.03), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    acceptButton->setAnchorPoint(Vec2(0,0));
    menuItems.pushBack(acceptButton);
    
    declineButton = MainMenu::CreateButton("popup", "Decline.png", ref, callback2);
    declineButton->setPosition(elements["Panel"]->getBoundingBox().getMidX() - (MainMenu::screenSize.x * 0.03), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.04));
    declineButton->setAnchorPoint(Vec2(1,0));
    menuItems.pushBack(declineButton);
    
    labels.push_back(MainMenu::CreateLabel("Accept", Vec2(0,0), Vec2(0,0)));
    labels[0]->setPosition(acceptButton->getBoundingBox().getMidX(), acceptButton->getBoundingBox().getMidY());
    labels[0]->setGlobalZOrder(8);
    labels[0]->setAnchorPoint(Vec2(0.5,0.5));
    
    labels.push_back(MainMenu::CreateLabel("Decline", Vec2(0,0), Vec2(0,0)));
    labels[1]->setPosition(declineButton->getBoundingBox().getMidX(), declineButton->getBoundingBox().getMidY());
    labels[1]->setGlobalZOrder(8);
    labels[1]->setAnchorPoint(Vec2(0.5,0.5));
    
    Size editBoxSize = Size(elements["Panel"]->getBoundingBox().size.width * 0.7, elements["Panel"]->getBoundingBox().size.height * 0.22);
    Scale9Sprite* nameBoxBG = Scale9Sprite::create("Slice_9_Inlay.png");
    nameBoxBG->setCapInsets(Rect(18,32,89,70));
    nameBoxBG->setContentSize(editBoxSize);
    nameBox = EditBox::create(editBoxSize, nameBoxBG);
    nameBox->setPosition(Point(MainMenu::screenSize.x / 2.0, elements["Panel"]->getBoundingBox().getMidY() - (MainMenu::screenSize.y * 0.07)));
    nameBox->setFontSize(75);
    nameBox->setMaxLength(18);
    nameBox->setPlaceHolder("level name");
    nameBox->setFontColor(Color3B::BLACK);
    nameBox->setPlaceholderFontColor(Color3B::BLACK);
    nameBox->setFontName("Marker Felt.ttf");
    nameBox->setPlaceholderFontName("Marker Felt.ttf");
    nameBox->setGlobalZOrder(7);
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMidY() + (MainMenu::screenSize.y * 0.1));
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setAnchorPoint(Vec2(0.5,1));
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}

PopUp::PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback){
    std::map<std::string, SEL_MenuHandler> callbacks;
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("popup", ref, callbacks, &menuItems, (cocos2d::Layer*)ref);
    elements["Shade"]->setGlobalZOrder(6);
    elements["Panel"]->setGlobalZOrder(7);
    
    acceptButton = MainMenu::CreateButton("popup", "Accept.png", ref, callback);
    acceptButton->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.1));
    acceptButton->setAnchorPoint(Vec2(0.5,0));
    acceptButton->setGlobalZOrder(3);
    menuItems.pushBack(acceptButton);
    
    labels.push_back(MainMenu::CreateLabel("Ok", Vec2(0,0), Vec2(0,0)));
    labels[0]->setPosition(acceptButton->getBoundingBox().getMidX(), acceptButton->getBoundingBox().getMidY());
    labels[0]->setGlobalZOrder(8);
    labels[0]->setAnchorPoint(Vec2(0.5,0.5));
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMidY() + (0.05 * MainMenu::screenSize.y));
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setAnchorPoint(Vec2(0.5,1));
    nameBox = nullptr;
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}
PopUp::PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler callback2, SEL_MenuHandler callback3, int x){
    std::map<std::string, SEL_MenuHandler> callbacks;
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("popup", ref, callbacks, &menuItems, (cocos2d::Layer*)ref);
    elements["Shade"]->setGlobalZOrder(6);
    elements["Panel"]->setGlobalZOrder(7);
    
    acceptButton = MainMenu::CreateButton("popup", "Accept.png", ref, callback);
    acceptButton->setPosition(elements["Panel"]->getBoundingBox().getMidX() - (MainMenu::screenSize.x * 0.165), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.25));
    acceptButton->setAnchorPoint(Vec2(0.5,0));
    acceptButton->setGlobalZOrder(8);
    menuItems.pushBack(acceptButton);
    
    declineButton = MainMenu::CreateButton("popup", "Accept.png", ref, callback2);
    declineButton->setPosition(elements["Panel"]->getBoundingBox().getMidX() + (MainMenu::screenSize.x * 0.165), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.25));
    declineButton->setAnchorPoint(Vec2(0.5,0));
    declineButton->setGlobalZOrder(8);
    menuItems.pushBack(declineButton);
    
    thirdButton = MainMenu::CreateButton("popup", "Accept.png", ref, callback3);
    thirdButton->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMinY() + (MainMenu::screenSize.y * 0.08));
    thirdButton->setAnchorPoint(Vec2(0.5,0));
    thirdButton->setGlobalZOrder(8);
    menuItems.pushBack(thirdButton);
    
    labels.push_back(MainMenu::CreateLabel("Level Select", Vec2(0,0), Vec2(0,0)));
    labels[0]->setPosition(acceptButton->getBoundingBox().getMidX(), acceptButton->getBoundingBox().getMidY());
    labels[0]->setGlobalZOrder(8);
    labels[0]->setAnchorPoint(Vec2(0.5,0.5));
    
    labels.push_back(MainMenu::CreateLabel("Highscores", Vec2(0,0), Vec2(0,0)));
    labels[1]->setPosition(declineButton->getBoundingBox().getMidX(), declineButton->getBoundingBox().getMidY());
    labels[1]->setGlobalZOrder(8);
    labels[1]->setAnchorPoint(Vec2(0.5,0.5));
    
    labels.push_back(MainMenu::CreateLabel("Replay", Vec2(0,0), Vec2(0,0)));
    labels[2]->setPosition(thirdButton->getBoundingBox().getMidX(), thirdButton->getBoundingBox().getMidY());
    labels[2]->setGlobalZOrder(8);
    labels[2]->setAnchorPoint(Vec2(0.5,0.5));
    
    name = MainMenu::CreateLabel(title, Vec2(0,0), Vec2(0,0));
    name->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMaxY() - (MainMenu::screenSize.y * 0.01));
    name->setGlobalZOrder(8);
    name->setAnchorPoint(Vec2(0.5,1));
    
    this->text = MainMenu::CreateLabel(text, Vec2(0,0), Vec2(0,0));
    this->text->setPosition(elements["Panel"]->getBoundingBox().getMidX(), elements["Panel"]->getBoundingBox().getMidY() + (MainMenu::screenSize.y * 0.1));
    this->text->setAnchorPoint(Vec2(0.5,0.5));
    this->text->setGlobalZOrder(8);
    nameBox = nullptr;
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,1));
    menu->setPosition(0,0);
    
    Close();
}
void PopUp::Show(){
    //menu->setPosition(menu->getPosition().x, menu->getPosition().y + 1000);
    visible = true;
    menu->setAnchorPoint(Vec2(0,0));
    name->setVisible(true);
    text->setVisible(true);
    menu->setVisible(true);
    menu->setEnabled(true);
    if(nameBox != nullptr){
        nameBox->setVisible(true);
        nameBox->setEnabled(true);
    }
    for(int x = 0; x < labels.size(); x++){
        labels[x]->setVisible(true);
    }
    if(labels.size() == 3){
        thirdButton->setVisible(true);
        thirdButton->setEnabled(true);
    }
}
void PopUp::Close(){
    visible = false;
    menu->setAnchorPoint(Vec2(0,1));
    name->setVisible(false);
    text->setVisible(false);
    menu->setVisible(false);
    menu->setEnabled(false);
    if(nameBox != nullptr){
        nameBox->setVisible(false);
        nameBox->setEnabled(false);
    }
    for(int x = 0; x < labels.size(); x++){
        labels[x]->setVisible(false);
    }
    if(labels.size() == 3){
        thirdButton->setVisible(false);
        thirdButton->setEnabled(false);
    }
}
void PopUp::Add(Layer* layer){
    layer->addChild(menu,1);
    layer->addChild(name,1);
    layer->addChild(text,1);
    if(nameBox != nullptr){
        layer->addChild(nameBox,1);
    }
    for(int x = 0; x < labels.size(); x++){
        layer->addChild(labels[x]);
    }
}
void PopUp::Remove(Layer* layer){
    layer->removeChild(menu,1);
    layer->removeChild(name,1);
    layer->removeChild(text,1);
    if(nameBox != nullptr){
        layer->removeChild(nameBox);
    }
    for(int x = 0; x < labels.size(); x++){
        layer->removeChild(labels[x]);
    }
}
std::string PopUp::GetText(){
    return nameBox->getText();
}