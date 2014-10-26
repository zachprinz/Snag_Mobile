//
//  LevelMenuItem.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/3/14.
//
//

#include "LevelMenuItem.h"
#include "LevelSelect.h"
#include "MainMenu.h"
#include "Level.h"

int LevelMenuItem::count;

LevelMenuItem::LevelMenuItem(int position, ui::ScrollView* layer, float center, float inlayHeight){
    float a4x3_NumberPanelSize = 88;
    float a3x2_NumberPanelSize = 41;
    float a16x9_NumberPanelSize = 78;
    float numberPanelSize;
    if(MainMenu::aspectRatio == 0)
        numberPanelSize = a3x2_NumberPanelSize * MainMenu::ar_scale;
    if(MainMenu::aspectRatio == 1)
        numberPanelSize = a4x3_NumberPanelSize * MainMenu::ar_scale;
    if(MainMenu::aspectRatio == 2)
        numberPanelSize = a16x9_NumberPanelSize * MainMenu::ar_scale;
    
    float x = 0;
    float y = 0;
    count++;
    Vector<MenuItem*> menuItems;
    std::string levelImageName = "Level_Left.png";
    if((position+1) % 2 == 0)
        levelImageName = "Level_Right.png";
    background = MainMenu::CreateButton("levelselect", levelImageName);
    background->setGlobalZOrder(0);
    background->setScaleX(background->getScaleX() * 0.98);
    background->setPosition(Vec2(center,(inlayHeight) - floor(((int)position)/2)*background->getBoundingBox().size.height*1.1));
    background->setAnchorPoint(Vec2((position+1) % 2 - (0.0625*(0.5-((position+1)%2))),1.0));
    menuItems.pushBack(background);
    
    float freeWidth = background->getBoundingBox().size.width - numberPanelSize;
    
    float ribbonX = background->getBoundingBox().getMinX() + (numberPanelSize / 2.0);
    if((position+1)%2 == 0)
        ribbonX = background->getBoundingBox().getMaxX() - (numberPanelSize / 2.0);
    this->number = MainMenu::CreateLabel(std::to_string(position+1), 0);
    this->number->setPosition(Vec2(ribbonX, background->getBoundingBox().getMaxY() - (numberPanelSize / 2.0)));
    this->number->setAnchorPoint(Vec2(0.5,0.5));
    this->number->setGlobalZOrder(0);
    
    float nameX = background->getBoundingBox().getMinX() + numberPanelSize + ((background->getBoundingBox().size.width - numberPanelSize)/2.0);
    if((position+1)%2 == 0)
        nameX = (background->getBoundingBox().getMaxX() - numberPanelSize - ((background->getBoundingBox().size.width - numberPanelSize)/2.0));
    this->name = MainMenu::CreateLabel("New Level", 1);
    this->name->setPosition(Vec2(nameX,  background->getBoundingBox().getMinY() + (background->getBoundingBox().size.height - numberPanelSize) + ((background->getBoundingBox().size.height - numberPanelSize)/2.0)));
    this->name->setAnchorPoint(Vec2(0.5,0.5));
    this->name->setGlobalZOrder(0);
    
    clock = MainMenu::CreateButton("levelselect", "Clock.png");
    clock->setAnchorPoint(Vec2(0.5,0.5));
    float clockX = background->getBoundingBox().getMaxX() - numberPanelSize;
    if((position+1)%2 == 0)
        clockX = background->getBoundingBox().getMinX() + numberPanelSize;
    float clockY = background->getBoundingBox().getMinY() + ((background->getBoundingBox().size.height - numberPanelSize)/2.0);
    this->name->setAnchorPoint(Vec2(0.5,0.5));
    clock->setPosition(clockX, clockY);
    menuItems.pushBack(clock);
    
    time = MainMenu::CreateLabel("00:00", 1);
    time->setPosition(clockX, clockY);
    time->setAnchorPoint(Vec2(0.5,0.5));
    
    
    MenuItemImage* previousStar = MainMenu::CreateButton("levelselect", "Star_Empty.png");
    previousStar->setScale(previousStar->getScale() * 0.9);

    previousStar->setAnchorPoint(Vec2(0.5,0.5));
    if((position+1)%2 == 0){
        previousStar->setPosition(number->getBoundingBox().getMinX(), clock->getBoundingBox().getMidY());
        stars.push_back(previousStar);
        for(int x = 0; x < 4; x++){
            auto star = MainMenu::CreateButton("levelselect", "Star_Empty.png");
            star->setScale(star->getScale() * 0.9);

            star->setPosition(previousStar->getBoundingBox().getMinX(), previousStar->getBoundingBox().getMidY());
            star->setAnchorPoint(Vec2(1,0.5));
            stars.push_back(star);
            previousStar = star;
        }
    } else {
        previousStar->setPosition(number->getBoundingBox().getMaxX(), clock->getBoundingBox().getMidY());
        stars.push_back(previousStar);
        for(int x = 0; x < 4; x++){
            auto star = MainMenu::CreateButton("levelselect", "Star_Empty.png");
            star->setScale(star->getScale() * 0.9);

            star->setPosition(previousStar->getBoundingBox().getMaxX(), previousStar->getBoundingBox().getMidY());
            star->setAnchorPoint(Vec2(0,0.5));
            stars.push_back(star);
            previousStar = star;
        }
    }
    for(int x = 0; x < stars.size(); x++){
        menuItems.pushBack(stars[x]);
    }
    time->setVisible(false);
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,0));
    menu->setPosition(0,0);
    
    layer->addChild(menu,-11);
    layer->addChild(name,-11);
    layer->addChild(time, -11);
    layer->addChild(number,-11);
}

LevelMenuItem::LevelMenuItem(int position, Layer* layer, float center, float height){
    float x = 0;
    float y = 0;
    count++;
    float a4x3_NumberPanelSize = 88;
    float a3x2_NumberPanelSize = 41;
    float a16x9_NumberPanelSize = 78;
    float numberPanelSize;
    if(MainMenu::aspectRatio == 0)
        numberPanelSize = a3x2_NumberPanelSize * MainMenu::ar_scale;
    if(MainMenu::aspectRatio == 1)
        numberPanelSize = a4x3_NumberPanelSize * MainMenu::ar_scale;
    if(MainMenu::aspectRatio == 2)
        numberPanelSize = a16x9_NumberPanelSize * MainMenu::ar_scale;
    
    Vector<MenuItem*> menuItems;
    std::string levelImageName = "Level_Left.png";
    if((position+1) % 2 == 0)
        levelImageName = "Level_Right.png";
    background = MainMenu::CreateButton("levelselect", levelImageName);
    background->setGlobalZOrder(0);
    background->setScaleX(background->getScaleX() * 0.98);
    background->setPosition(Vec2(center,height));
    background->setAnchorPoint(Vec2((position+1) % 2 - (0.055*(0.5-((position+1)%2))),0.5));
    menuItems.pushBack(background);
    
    float ribbonX = background->getBoundingBox().getMinX() + (numberPanelSize / 2.0);
    if((position+1)%2 == 0)
        ribbonX = background->getBoundingBox().getMaxX() - (numberPanelSize / 2.0);
    ribbon = MainMenu::CreateButton("levelselect", "Featured_Icon.png");
    ribbon->setPosition(Vec2(ribbonX, background->getBoundingBox().getMaxY() - (numberPanelSize / 2.0)));
    ribbon->setAnchorPoint(Vec2(0.5,0.5));
    menuItems.pushBack(ribbon);
    
    this->number = MainMenu::CreateLabel("1) ", 0);
    this->number->setPosition(Vec2(background->getPositionX() + (24*MainMenu::screenScale.x), background->getBoundingBox().getMidY()));
    this->number->setAnchorPoint(Vec2(0,0.5));
    this->number->setGlobalZOrder(0);
    
    float nameX = background->getBoundingBox().getMinX() + numberPanelSize + ((background->getBoundingBox().size.width - numberPanelSize)/2.0);
    if((position+1)%2 == 0)
        nameX = (background->getBoundingBox().getMaxX() - numberPanelSize - ((background->getBoundingBox().size.width - numberPanelSize)/2.0));
    this->name = MainMenu::CreateLabel("New Level", 1);
    this->name->setPosition(Vec2(nameX,  background->getBoundingBox().getMinY() + (background->getBoundingBox().size.height - numberPanelSize) + ((background->getBoundingBox().size.height - numberPanelSize)/2.0)));
    this->name->setAnchorPoint(Vec2(0.5,0.5));
    this->name->setGlobalZOrder(0);
    float xScale = name->getScaleX()* 0.75;
    float yScale = name->getScaleY()* 0.75;
    xScale = ((float)floor((xScale) * 100))/100.f;
    yScale = ((float)floor((yScale) * 100))/100.f;
    printf("\nName Scale: (%f, %f)\n", name->getScaleX(), name->getScaleY());

    
    clock = MainMenu::CreateButton("levelselect", "Clock.png");
    clock->setAnchorPoint(Vec2(0.5,0.5));
    float clockX = background->getBoundingBox().getMaxX() - numberPanelSize;
    if((position+1)%2 == 0)
        clockX = background->getBoundingBox().getMinX() + numberPanelSize;
    float clockY = background->getBoundingBox().getMinY() + ((background->getBoundingBox().size.height - numberPanelSize)/2.0);
    this->name->setAnchorPoint(Vec2(0.5,0.5));
    clock->setPosition(clockX, clockY);
    menuItems.pushBack(clock);
    
    time = MainMenu::CreateLabel("00:00", 1);
    time->setPosition(clockX, clockY);
    time->setAnchorPoint(Vec2(0.5,0.5));
    
    MenuItemImage* previousStar = MainMenu::CreateButton("levelselect", "Star_Empty.png");
    previousStar->setScale(previousStar->getScale() * 0.9);

    previousStar->setAnchorPoint(Vec2(0.5,0.5));
    if((position+1)%2 == 0){
        previousStar->setPosition(ribbon->getBoundingBox().getMinX(), clock->getBoundingBox().getMidY());
        stars.push_back(previousStar);
        for(int x = 0; x < 4; x++){
            auto star = MainMenu::CreateButton("levelselect", "Star_Empty.png");
            star->setScale(star->getScale() * 0.9);
            star->setPosition(previousStar->getBoundingBox().getMinX(), previousStar->getBoundingBox().getMidY());
            star->setAnchorPoint(Vec2(1,0.5));
            stars.push_back(star);
            previousStar = star;
        }
    } else {
        previousStar->setPosition(ribbon->getBoundingBox().getMaxX(), clock->getBoundingBox().getMidY());
        stars.push_back(previousStar);
        for(int x = 0; x < 4; x++){
            auto star = MainMenu::CreateButton("levelselect", "Star_Empty.png");
            star->setScale(star->getScale() * 0.9);

            star->setPosition(previousStar->getBoundingBox().getMaxX(), previousStar->getBoundingBox().getMidY());
            star->setAnchorPoint(Vec2(0,0.5));
            stars.push_back(star);
            previousStar = star;
        }
    }
    for(int x = 0; x < stars.size(); x++){
        menuItems.pushBack(stars[x]);
    }
    time->setVisible(false);

    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,0));
    menu->setPosition(0,0);
    layer->addChild(menu,1);
    layer->addChild(name,1);
    layer->addChild(time,1);

}
void LevelMenuItem::SetEnabled(bool is){
    menu->setEnabled(is);
    menu->setVisible(is);
    name->setVisible(is);
    number->setVisible(is);
    time->setVisible(false);//is);
}
void LevelMenuItem::SetOrder(int){

}
void LevelMenuItem::SetFavorited(bool i){
    if(i){
        //int count = std::atoi(favorites->getString().c_str());
        //favorites->setString(std::to_string(count));
        //favorited->setNormalImage(Sprite::create("FavoriteOn.png"));
    }
    else{
        //int count = std::atoi(favorites->getString().c_str());
        //favorites->setString(std::to_string(count));
        //favorited->setNormalImage(Sprite::create("FavoriteOff.png"));
    }
}
void LevelMenuItem::SetLevel(Level* lvl, int page){
    level = lvl;
    myName = lvl->GetName();
    myPath = lvl->GetPath();
    name->setString(myName);
    SetFavorited(lvl->GetIsFavorited());
    printf("\nSet up a custom leve with %d favorites that %d favorited.", lvl->GetFavorites(), lvl->GetIsFavorited());
}
void LevelMenuItem::SetTag(int x){
    //favorited->setTag(x);
    background->setTag(x);
    if(myName != "")
        name->setString(std::to_string(x) + ") " + myName);
}