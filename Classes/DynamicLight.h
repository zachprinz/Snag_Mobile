#ifndef AVALON_GRAPHICS_DYNAMICLIGHT_H
#define AVALON_GRAPHICS_DYNAMICLIGHT_H

#include "cocos2d.h"

namespace avalon {
    namespace graphics {
        
        class DynamicLight : public cocos2d::Node
        {
        private:
            bool bakedMapIsValid = false;
            bool softShadows = true;
            bool additive = true;
            float upScale = 1.0;
            float finalSize = lightSize * upScale;
            float accuracy = 1.0;
            int lightSize = 256;
            cocos2d::Color4B color = {64, 130, 77, 255};
            cocos2d::Node* shadowCasters = nullptr;
            
            cocos2d::RenderTexture* occlusionMap = nullptr;
            cocos2d::RenderTexture* shadowMap1D = nullptr;
            cocos2d::RenderTexture* finalShadowMap = nullptr;
            cocos2d::RenderTexture* bakedShadowMap = nullptr;
            
            cocos2d::GLProgram* shadowMapShader = nullptr;
            cocos2d::GLProgram* shadowRenderShader = nullptr;
            
            void initOcclusionMap();
            void initShadowMap1D();
            void initFinalShadowMap();
            void initBakedShadowMap();
            void updateUniforms();
            void createOcclusionMap();
            void createShadowMap(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated);
            void updateShadowMap(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated);
            
        public:
            int updateFrequency = 0;
            int updateCount = 0;
            bool debugDrawEnabled = false;
            
            CREATE_FUNC(DynamicLight);
            bool init() override;
            ~DynamicLight();
            
            void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
            void debugDraw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, bool transformUpdated);
            
            void setPosition(const cocos2d::Point& position) override;
            void setSoftShadows(bool shadows);
            void setLightSize(int lightSize);
            void setUpScale(float upScale);
            void setAccuracy(float accuracy);
            void setAdditive(bool additive);
            void setColor(const cocos2d::Color4B& color);
            void setShadowCasters(cocos2d::Node& casters);
        };
        
    } // namespace graphics
} // namespace avalon

#endif /* AVALON_GRAPHICS_DYNAMICLIGHT_H */
