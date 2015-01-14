//
//  Entity.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "Entity.h"
#include "MainMenu.h"
#include "Game.h"

USING_NS_CC;

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5

Entity* Entity::createWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0),WALL);
    return ent;
}
Entity* Entity::createSpikeWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0),SPIKE_WALL);
    return ent;
}
Entity* Entity::createGoal(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0),GOAL);
    return ent;
}
Entity* Entity::createHook(Vec2 pos){
    Entity* ent = new Entity(pos, Vec2(0,0), Vec2(0,0), HOOK);
    return ent;
}
Entity* Entity::createSpawner(Vec2 pos, Vec2 vel){
    Entity* ent = new Entity(pos, Vec2(0,0), vel, SPAWNER);
    return ent;
}
int Entity::count = 0;

Entity::Entity(Vec2 pos, Vec2 size, Vec2 vel, int type){
    ID = count++;
    std::string textures[6] = {"wall.png", "spikewall.png", "game_hook.png", "spawner.png", "goal.png", "user.png"};
    float baseScales[6] = {1,1,1,1,1,1};
    this->baseScale = Vec2(baseScales[type],baseScales[type]);
    this->type = type;
    this->size = Vec2(size.x,size.y);
    this->position = Vec2(pos.x, pos.y);
    sprite = Sprite::create(textures[type]);
    this->originalSize = Vec2(sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->retain();
    sprite->setGlobalZOrder(0);
    if(this->size.x != 0 && this->size.y != 0){
        baseScale.x = this->size.x / sprite->getBoundingBox().size.width;
        baseScale.y = this->size.y / sprite->getBoundingBox().size.height;
    }
    this->launchVelocity = vel;
    sprite->setScale(baseScale.x, baseScale.y);
    if(type != USER)
        this->SetUpPhysicsSprite(textures[type], baseScale);
    sprite->setPosition(position);
    if(type == HOOK){
        line = Sprite::create("game_line.png");
        line->retain();
        line->setAnchorPoint(Vec2(0.0,0.5));
        line->setVisible(false);
        line->setPosition(this->position.x,this->position.y);
        hasParticleEffects = true;
    }
    if(type == HOOK || type == GOAL || type == SPAWNER){
        SetUpParticles();
    }
}
float GetAngle(Vec2 a, Vec2 b){
    float o = a.x - b.x;
    float adj = a.y - b.y;
    if(a.x == b.x){
        if(a.y > b.y)
            return 90;
        else
            return 270;
    }
    if(a.y == b.y){
        if(a.x > b.x)
            return 0;
        else
            return 180;
    }
    float angle = atan(o/adj) * 57.29;
    if(adj < 0){ //Left two
        if(adj > 0) // bottom left
            angle = (90-angle) + 180;
        if(adj < 0) // top left
            angle = 90 + angle;
    }
    else{ // Right two
        if(adj > 0) // bottom right
            angle += 270;
        if(adj < 0) // top right
            angle = (90 - angle);
    }
    return angle;
}
void Entity::SetUpParticles(){
    emitter = ParticleSystemQuad::create();
    for(int x = 0; x < 10; x++)
    emitter->addParticle();
    emitter->retain();
    emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("Images/particle.png"));
    emitter->setBlendAdditive(false);
    emitter->setDuration(ParticleSystem::DURATION_INFINITY);
    emitter->setAngle(0);
    emitter->setAngleVar(360);
    emitter->setPosVar(Vec2(0.5*sprite->getBoundingBox().size.width, 0.5*sprite->getBoundingBox().size.height));
    if(type == HOOK){
        emitter->setLife(1.f);
        emitter->setLifeVar(0.2f);
        emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);
        emitter->setStartRadius(sprite->getBoundingBox().size.width * .85);
        emitter->setStartRadiusVar(sprite->getBoundingBox().size.width * 0.0);
        emitter->setEndRadius(sprite->getBoundingBox().size.width * 1.2);
        emitter->setEndRadiusVar(sprite->getBoundingBox().size.width * 0.0);
        emitter->setRotatePerSecond(0);
        emitter->setRotatePerSecondVar(360);
        emitter->setStartColor(Color4F(Color4B(255, 0, 0, 255)));
        emitter->setEndColor(Color4F(Color4B(255,0,0,50)));
        emitter->setEndColorVar(Color4F(Color4B(0,0,0,50)));
        emitter->setStartSize(20.0f);
        emitter->setStartSizeVar(2.0f);
        emitter->setEndSize(10);
        emitter->setEndSizeVar(2);
        emitter->setEmissionRate(0.035 * sprite->getBoundingBox().size.width * sprite->getBoundingBox().size.height);
    }
    if(type == GOAL){
        emitter->setLife(0.8f);
        emitter->setLifeVar(0.1f);
        emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
        emitter->setGravity(Vec2(0,0));
        emitter->setRadialAccel(0);
        emitter->setRadialAccelVar(0);
        emitter->setSpeed(30);
        emitter->setSpeedVar(0);
        emitter->setStartColor(Color4F(Color4B(0, 255, 0, 255)));
        emitter->setEndColor(Color4F(Color4B(0,255,0,11)));
        emitter->setEndColorVar(Color4F(Color4B(0,0,0,50)));
        emitter->setStartSize(25.0f);
        emitter->setStartSizeVar(5.0f);
        emitter->setEndSize(10);
        emitter->setEndSizeVar(5);
        emitter->setEmissionRate(0.001 * sprite->getBoundingBox().size.width * sprite->getBoundingBox().size.height);
    }
    if(type == SPAWNER){
        emitter->setAngle(GetAngle(Vec2(0,0),Vec2(launchVelocity.x * -1, launchVelocity.y)));
        emitter->setAngleVar(0);
        emitter->setLife(0.5f);
        emitter->setLifeVar(0.1f);
        emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
        emitter->setGravity(Vec2(-1*launchVelocity.x, -1*launchVelocity.y));
        emitter->setRadialAccel(0);
        emitter->setRadialAccelVar(0);
        emitter->setSpeed(0.6*0.01*(launchVelocity.x*launchVelocity.x + launchVelocity.y*launchVelocity.y));
        emitter->setSpeedVar(0.6*0.005*(launchVelocity.x*launchVelocity.x + launchVelocity.y*launchVelocity.y));
        emitter->setStartColor(Color4F(Color4B(255, 255, 0, 255)));
        emitter->setEndColor(Color4F(Color4B(255,255,0,255)));
        emitter->setEndColorVar(Color4F(Color4B(0,0,0,0)));
        emitter->setStartSize(25.0f);
        emitter->setStartSizeVar(5.0f);
        emitter->setEndSize(0);
        emitter->setEndSizeVar(5);
        emitter->setEmissionRate(0.02 * sprite->getBoundingBox().size.width * sprite->getBoundingBox().size.height);
        emitter->setPosVar(Vec2(0.25*sprite->getBoundingBox().size.width, 0.25*sprite->getBoundingBox().size.height));
    }
    emitter->setEndColor(Color4F(Color4B(0, 0, 0, 0)));
    emitter->setStartColorVar(Color4F(Color4B(0,0,0,0)));
    emitter->setPosition(Vec2(sprite->getBoundingBox().getMidX(), sprite->getBoundingBox().getMidY()));
    emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
}
void Entity::SetLine(float size, float angle){
    if(type == HOOK){
        line->setRotation(0);
        line->setScale((size)/100,5);
        line->setRotation(angle);
        line->setVisible(true);
    }
}
void Entity::SetLineOff(){
    if(type == HOOK){
        line->setVisible(false);
    }
}
void Entity::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    auto physMat = PhysicsMaterial(0.1,1,0);
    body = PhysicsBody::createBox(Size(sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height),physMat);
    body->setMass(PHYSICS_INFINITY);
    body->setDynamic(false);
    body->setTag(type);
    sprite->setPhysicsBody(body);
    sprite->setTag(type);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    if(type == SPAWNER || type == HOOK){
        body->setContactTestBitmask(false);
        body->setCategoryBitmask(false);
    } else {
        body->setContactTestBitmask(true);
        body->setCategoryBitmask(true);
    }
}
void Entity::update(Vec2 userPosition, float boardScale){
    //emitter->setPosVar(Vec2(sprite->getBoundingBox().size.width * (1/boardScale), (1/boardScale) * sprite->getBoundingBox().size.height));
}
void Entity::CalculateScale(Vec2 userPosition, float boardScale){

}
void Entity::Add(Game* game){
    sprite->setGlobalZOrder(0);
    game->layers[type]->addChild(sprite,1);
    sprite->release();
    if(type == GOAL || type == SPAWNER || type == HOOK){
        game->particleBatchNode->addChild(emitter);
    }
    if(type == HOOK){
        game->layers[type]->addChild(line,1);
        line->release();
    }
    if(type == USER){
        //game->layers[HOOK]->addChild(((User*)this)->line,1);
        //line->release();
    }
}
void Entity::Remove(Game* game){
    sprite->retain();
    this->sprite->removeFromParent();
    if(type == HOOK){
        line->retain();
        line->removeFromParent();
    }
    if(type == GOAL || type == SPAWNER || type == HOOK){
        game->particleBatchNode->removeChild(emitter, false);
    }
    if(type == USER){
        //game->layers[HOOK]->addChild(((User*)this)->line,1);
    }
    //game->removeChild(sprite);
}
Sprite* Entity::GetSprite(){
    return sprite;
}
Point Entity::GetPosition(){
    return position;
}
void Entity::SetPosition(Vec2 pos){
    position = pos;
}
Vec2 Entity::GetLaunchVelocity(){
    return launchVelocity;
}
void Entity::SetLaunchVelocity(Vec2 vl){
    launchVelocity = vl;
    emitter->setAngle(GetAngle(Vec2(0,0),Vec2(launchVelocity.x * -1, launchVelocity.y)));
}
int Entity::GetType(){
    return type;
}
Vec2 Entity::GetSize(){
    return size;
}
void Entity::SetSize(Vec2 size){
    this->size = size;
}
Vec2 Entity::GetOriginalSize(){
    return originalSize;
}
float Entity::GetSum(){
    return (position.x + position.y + launchVelocity.x + launchVelocity.y + size.x + size.y);
}
