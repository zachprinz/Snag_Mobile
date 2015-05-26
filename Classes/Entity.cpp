#include "Entity.h"
#include "MainMenu.h"
#include "Game.h"
#include "PlayScreen.h"
#include "Utils.h"
#include <String>

USING_NS_CC;

Entity* Entity::createWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0), "wall.png", true);
    ent->setTag(WALL);
    return ent;
}
Entity* Entity::createSpikeWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0), "spikewall.png", true);
    ent->setTag(SPIKE_WALL);
    return ent;
}
Entity* Entity::createGoal(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0), "goal.png", true);
    ent->setTag(GOAL);
    ent->createEmitter();
    ParticleSystemQuad* emitter = ent->getEmitter();
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
    emitter->setEmissionRate(0.001 * size.x * size.y);
    ent->setEmitter(emitter);
    return ent;
}
Entity* Entity::createHook(Vec2 pos){
    Entity* ent = new Entity(pos, Vec2(0,0), Vec2(0,0), "game_hook.png", false);
    ent->setTag(HOOK);
    float width = ent->getSize().x;
    ParticleSystemQuad* emitter = ent->getEmitter();
    emitter->setLife(1.f);
    emitter->setLifeVar(0.2f);
    emitter->setEmitterMode(ParticleSystem::Mode::RADIUS);
    emitter->setStartRadius(width * 0.85);
    emitter->setStartRadiusVar(0);
    emitter->setEndRadius(width * 1.2);
    emitter->setEndRadiusVar(0);
    emitter->setRotatePerSecond(0);
    emitter->setRotatePerSecondVar(360);
    emitter->setStartColor(Color4F(Color4B(255, 0, 0, 255)));
    emitter->setEndColor(Color4F(Color4B(255,0,0,50)));
    emitter->setEndColorVar(Color4F(Color4B(0,0,0,50)));
    emitter->setStartSize(20.0f);
    emitter->setStartSizeVar(2.0f);
    emitter->setEndSize(10);
    emitter->setEndSizeVar(2);
    emitter->setEmissionRate(0.035 * width * width); //Square so (W*H == W*W)
    ent->setEmitter(emitter);
    return ent;
}
Entity* Entity::createSpawner(Vec2 pos, Vec2 vel){
    Entity* ent = new Entity(pos, Vec2(0,0), vel, "spawner.png", false);
    ent->setTag(SPAWNER);
    float width = ent->getSize().x;
    ParticleSystemQuad* emitter = ent->getEmitter();
    emitter->setAngle(getAngle(Vec2(0,0),Vec2(vel.x * -1, vel.y)));
    emitter->setAngleVar(0);
    emitter->setLife(0.5f);
    emitter->setLifeVar(0.1f);
    emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);
    emitter->setGravity(Vec2(-1*vel.x, -1*vel.y));
    emitter->setRadialAccel(0);
    emitter->setRadialAccelVar(0);
    emitter->setSpeed(0.6*0.01*(vel.x*vel.x + vel.y*vel.y));
    emitter->setSpeedVar(0.6*0.005*(vel.x*vel.x + vel.y*vel.y));
    emitter->setStartColor(Color4F(Color4B(255, 255, 0, 255)));
    emitter->setEndColor(Color4F(Color4B(255,255,0,255)));
    emitter->setEndColorVar(Color4F(Color4B(0,0,0,0)));
    emitter->setStartSize(25.0f);
    emitter->setStartSizeVar(5.0f);
    emitter->setEndSize(0);
    emitter->setEndSizeVar(5);
    emitter->setEmissionRate(0.02 * width * width); //Square so (W*H == W*W)
    emitter->setPosVar(Vec2(0.25 * width, 0.25 * width));
    ent->setEmitter(emitter);
    return ent;
}

int Entity::count = 0;

Entity::Entity(Vec2 position, Vec2 size, Vec2 launchVelocity, std::string texture, bool collision){
    ID = count++;
    
    this->size = size;
    this->launchVelocity = launchVelocity;
    
    sprite = Sprite::create(texture);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->retain();
    sprite->setGlobalZOrder(0);
    sprite->setPosition(position);

    if(this->size.x != 0 && this->size.y != 0){
        float baseScale_x = this->size.x / sprite->getBoundingBox().size.width;
        float baseScale_y = this->size.y / sprite->getBoundingBox().size.height;
        sprite->setScale(baseScale_x, baseScale_y);
    }
    
    setUpPhysicsSprite(collision);
    emitter = NULL;
}

void Entity::setUpPhysicsSprite(bool collision){
    body = PhysicsBody::createBox(Size(sprite->getBoundingBox().size), PhysicsMaterial(0.1,1,0));
    body->setMass(PHYSICS_INFINITY);
    body->setDynamic(false);
    body->setContactTestBitmask(false);
    body->setCategoryBitmask(false);
    if(collision){
        body->setContactTestBitmask(true);
        body->setCategoryBitmask(true);
    }
    sprite->setPhysicsBody(body);
}

void Entity::add(){
    sprite->setGlobalZOrder(0);
    PlayScreen::Instance->addSprite(sprite);
    sprite->release();
    if(getEmitter() != NULL)
        PlayScreen::Instance->addEmitter(emitter);
}

void Entity::remove(){
    sprite->retain();
    sprite->removeFromParent();
    if(getEmitter() != NULL)
        PlayScreen::Instance->removeEmitter(getEmitter());
}

void Entity::createEmitter(){
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
}

void Entity::setEmitter(ParticleSystemQuad* emit){
    emitter = emit;
    emitter->setEndColor(Color4F(Color4B(0, 0, 0, 0)));
    emitter->setStartColorVar(Color4F(Color4B(0,0,0,0)));
    emitter->setPosition(Vec2(sprite->getBoundingBox().getMidX(), sprite->getBoundingBox().getMidY()));
    emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
}

ParticleSystemQuad* Entity::getEmitter(){
    return emitter;
}

Point Entity::getPosition(){
    return sprite->getPosition();
}
void Entity::setPosition(Vec2 pos){
    sprite->setPosition(pos);
}
Vec2 Entity::getLaunchVelocity(){
    return launchVelocity;
}
void Entity::setLaunchVelocity(Vec2 vl){
    launchVelocity = vl;
    if(getEmitter() != NULL)
        emitter->setAngle(getAngle(Vec2(0,0),Vec2(launchVelocity.x * -1, launchVelocity.y)));
}
Vec2 Entity::getSize(){
    return size;
}
void Entity::setSize(Vec2 size){
    this->size = size;
}
int Entity::getID(){
    return ID;
}
Rect Entity::getBoundingBox(){
    return sprite->getBoundingBox();
}
void Entity::setTag(int type){
    int tag = (type & TYPE_MASK) | ((getID() << 3));
    this->tag = tag;
    body->setTag(tag);
    sprite->setTag(tag);
}
int Entity::getTag(){ return tag; }
PhysicsBody* Entity::getPhysicsBody(){
    return body;
}