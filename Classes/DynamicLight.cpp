#include <DynamicLight.h>
#include <shaderhelper.h>
#include <color.h>
#include "Game.h"

using namespace cocos2d;
using namespace avalon::utils::color;

namespace  {
    
//    const GLchar* vertexShader =
//#include "shaders/Vert.vsh"//cocos2d::ccPositionTextureColor_noMVP_vert;
    const GLchar* vertexShader =
#include "shaders/pass.vsh"
    
    const GLchar* shadowMapFragmentShader =
#include "shaders/shadowMap.fsh"
    
    const GLchar* shadowRenderFragmentShader =
#include "shaders/shadowRender.fsh"
    
    } // namespace
    
    namespace avalon {
        namespace graphics {
            
            DynamicLight::~DynamicLight()
            {
                CC_SAFE_RELEASE(shadowRenderShader);
                CC_SAFE_RELEASE(shadowMapShader);
                CC_SAFE_RELEASE(occlusionMap);
                CC_SAFE_RELEASE(shadowMap1D);
                CC_SAFE_RELEASE(finalShadowMap);
                CC_SAFE_RELEASE(shadowCasters);
                CC_SAFE_RELEASE(bakedShadowMap);
            }
            
            bool DynamicLight::init()
            {
                if (!Node::init()) {
                    return false;
                }
                printf("\nLoading Shadow Map Shader");
                shadowMapShader = avalon::graphics::loadShader(vertexShader, shadowMapFragmentShader);
                printf("\n\tFinished\nLoading Shadow Render Shader\n");
                shadowRenderShader = avalon::graphics::loadShader(vertexShader, shadowRenderFragmentShader);
                printf("\tFinished\n");
                lightSize = 256;
                initOcclusionMap();
                initShadowMap1D();
                initFinalShadowMap();
                initBakedShadowMap();
                bakedMapIsValid = false;
                
                return true;
            }
            
            void DynamicLight::initOcclusionMap()
            {
                CC_SAFE_RELEASE(occlusionMap);
                
                occlusionMap = RenderTexture::create(lightSize, lightSize);
                occlusionMap->setKeepMatrix(true);
                occlusionMap->retain();
            }
            
            void DynamicLight::setOcclusionMap(RenderTexture* text){
                CC_SAFE_RELEASE(occlusionMap);
                occlusionMap = text;
                occlusionMap->retain();
            }
            
            RenderTexture* DynamicLight::getOcclusionMap(){
                return occlusionMap;
            }
            
            void DynamicLight::initShadowMap1D()
            {
                CC_SAFE_RELEASE(shadowMap1D);
                
                // seems like 16 pixel is the minimum height of a texture (on ios)
                shadowMap1D = RenderTexture::create(lightSize, 16);
                shadowMap1D->retain();
                //shadowMap1D->getSprite()->setFlippedY(true);
            }
            
            void DynamicLight::initFinalShadowMap()
            {
                CC_SAFE_RELEASE(finalShadowMap);
                
                finalSize = lightSize * upScale;
                
                finalShadowMap = RenderTexture::create(finalSize, finalSize);
                finalShadowMap->retain();
            }
            
            void DynamicLight::initBakedShadowMap()
            {
                CC_SAFE_RELEASE(bakedShadowMap);
                
                finalSize = lightSize * upScale;
                
                bakedShadowMap = RenderTexture::create(finalSize, finalSize);
                bakedShadowMap->retain();
            }
            
            void DynamicLight::setShadowCasters(Node& casters)
            {
                CC_SAFE_RELEASE(shadowCasters);
                
                bakedMapIsValid = false;
                
                shadowCasters = &casters;
                shadowCasters->retain();
            }
            
            void DynamicLight::updateShadowMap(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
            {
                createOcclusionMap();
                createShadowMap(renderer, transform, transformUpdated);
            }
            
            void DynamicLight::setPosition(const Point& position)
            {
                if (position.x == getPosition().x && position.y == getPosition().y) {
                    return;
                }
                
                Node::setPosition(position);
                
                ++updateCount;
                if (updateCount > updateFrequency) {
                    updateCount = 0;
                    bakedMapIsValid = false;
                }
            }
            
            void DynamicLight::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags)
            {
                if (!bakedMapIsValid) {
                    bakedMapIsValid = true;
                    
                    updateUniforms();
                    updateShadowMap(renderer, transform, flags);
                    
                    finalShadowMap->getSprite()->setColor({color.r, color.g, color.b});
                    finalShadowMap->getSprite()->setGLProgram(shadowRenderShader);
                    finalShadowMap->getSprite()->setAnchorPoint({0, 0});
                    finalShadowMap->getSprite()->setPosition({0, 0});
                    finalShadowMap->setScaleY(-1);
                    
                    bakedShadowMap->beginWithClear(0.0, 0.0, 0, 0);
                    finalShadowMap->setAnchorPoint({0.5, 0.5});
                    finalShadowMap->setPosition({drawPosition.x * -1, drawPosition.y});
                    finalShadowMap->visit(renderer, transform, flags);
                    bakedShadowMap->end();
                    bakedShadowMap->setPosition({0, 0});
                    
                    if (additive) {
                        bakedShadowMap->getSprite()->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
                    }
                }
                bakedShadowMap->visit(renderer, transform, flags);
                
                if (false) {
                    debugDraw(renderer, transform, flags);
                }
            }
            
            void DynamicLight::debugDraw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
            {
                auto glView = Director::getInstance()->getOpenGLView();
                auto width = glView->getDesignResolutionSize().width;
                auto height = glView->getDesignResolutionSize().height;
                
                auto occlusionX = width - lightSize / 2 - getPositionX();
                auto occlusionY = height - lightSize / 2 - getPositionY();
                
                auto shadowX = width - lightSize / 2 - getPositionX();
                auto shadowY = height - lightSize - 15 - getPositionY();
                
                occlusionMap->getSprite()->setColor(Color3B::RED);
                occlusionMap->setAnchorPoint({0, 0});
                occlusionMap->setPosition({occlusionX, occlusionY});
                occlusionMap->visit(renderer, transform, transformUpdated);
                occlusionMap->getSprite()->setColor(Color3B::WHITE);
                
                shadowMap1D->setAnchorPoint({0, 0});
                shadowMap1D->setPosition({shadowX, shadowY});
                shadowMap1D->visit(renderer, transform, transformUpdated);
            }
            
            void DynamicLight::updateUniforms()
            {
                shadowMapShader->use();
                GLint res = shadowMapShader->getUniformLocationForName("resolution");
                shadowMapShader->setUniformLocationWith2f(res, lightSize, lightSize);
                
                GLint glUpscale = shadowMapShader->getUniformLocationForName("upScale");
                shadowMapShader->setUniformLocationWith1f(glUpscale, upScale);
                
                GLint glAccuracy = shadowMapShader->getUniformLocationForName("accuracy");
                shadowMapShader->setUniformLocationWith1f(glAccuracy, accuracy);
                
                shadowRenderShader->use();
                GLint glResolution2 = shadowRenderShader->getUniformLocationForName("resolution");
                GLint glSoftShadows = shadowRenderShader->getUniformLocationForName("softShadows");
                
                shadowRenderShader->setUniformLocationWith2f(glResolution2, lightSize, lightSize);
                shadowRenderShader->setUniformLocationWith1f(glSoftShadows, softShadows ? 1.0f : 0.0f);
                
                glUniform1i(glGetUniformLocation(shadowRenderShader->getProgram(), "u_texture2"), 1);
                
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, shadowMap1D->getSprite()->getTexture()->getName());
                glActiveTexture(GL_TEXTURE0);
            }
            
            void DynamicLight::createOcclusionMap()
            {
                Game::Instance->CreateOcclusionMap(occlusionMap);
            }
            
            void DynamicLight::createShadowMap(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated)
            {
                // Build a 1D shadow map from occlude FBO
                occlusionMap->getSprite()->setGLProgram(shadowMapShader);
                shadowMap1D->beginWithClear(255, 0, 0, 255);
                occlusionMap->setAnchorPoint({0.5, 0.5});
                occlusionMap->setPosition({static_cast<float>(lightSize / 2.0), static_cast<float>(lightSize / 2.0)});
                occlusionMap->visit(renderer, transform, transformUpdated);
                shadowMap1D->end();
            }
            
            void DynamicLight::setSoftShadows(bool shadows)
            {
                if (softShadows != shadows) {
                    softShadows = shadows;
                    bakedMapIsValid = false;
                }
            }
            
            void DynamicLight::setLightSize(int lightSize)
            {
                if (this->lightSize != lightSize) {
                    this->lightSize = lightSize;
                    bakedMapIsValid = false;
                }
            }
            
            void DynamicLight::setUpScale(float upScale)
            {
                if (this->upScale != upScale) {
                    this->upScale = upScale;
                    bakedMapIsValid = false;
                }
            }
            
            void DynamicLight::setAccuracy(float accuracy)
            {
                if (this->accuracy != accuracy) {
                    this->accuracy = accuracy;
                    bakedMapIsValid = false;
                }
            }
            
            void DynamicLight::setAdditive(bool additive)
            {
                if (this->additive != additive) {
                    this->additive = additive;
                    bakedMapIsValid = false;
                }
            }
            
            void DynamicLight::setColor(const Color4B& color)
            {
                if (this->color != color) {
                    this->color = color;
                    bakedMapIsValid = false;
                }
            }
            
        } // namespace graphics
    } // namespace avalon