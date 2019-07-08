//
//  HitDetectHelper.cpp
//
//  Created by Alexander Wong on 1/21/15.
//
//
//PL
#include "HitDetectHelper.h"

USING_NS_CC;

#pragma mark - HitDetectHelper

RenderTexture* HitDetectHelper::_sharedRenderTexture = NULL;
HitDetectHelper::HitDetectHelperNode* HitDetectHelper::_helperNode = NULL;

#pragma mark - Pixel Test

bool HitDetectHelper::hitTest(Node* node, Touch* touch, float extraWidth, float extraHeight){
    if(!hitTestJustBounds(node, touch, extraWidth, extraHeight)){
        return false;
    }
    else{
        if(extraWidth > 0 || extraHeight > 0){
            return true;
        }
    }
    return hitTestJustPixels(node, touch);
}

bool HitDetectHelper::hitTestJustBounds(cocos2d::Node* node, cocos2d::Touch* touch, float extraWidth, float extraHeight){
    return Rect(0, 0, node->getContentSize().width + extraWidth, node->getContentSize().height + extraHeight).containsPoint(node->convertToNodeSpace(touch->getLocation()));
}

//bool HitDetectHelper::hitTestJustPixels(Node* node, cocos2d::Touch* touch)
bool HitDetectHelper::hitTestJustPixels(Node* node, cocos2d::Touch* touch)
{
    Point point = node->convertToNodeSpace(touch->getLocation());
    if (!_sharedRenderTexture) {
        _sharedRenderTexture = RenderTexture::create(1, 1);
        _sharedRenderTexture->retain();
        
        _sharedRenderTexture->setKeepMatrix(true);
        Size winSize = Director::getInstance()->getWinSize();
        Size winSizeInPixels = Director::getInstance()->getWinSizeInPixels();
        _sharedRenderTexture->setVirtualViewport(Director::getInstance()->getVisibleOrigin(), Rect(0.0f, 0.0f, winSize.width, winSize.height), Rect(0.0f, 0.0f, winSizeInPixels.width, winSizeInPixels.height));
        _sharedRenderTexture->setAutoDraw(false);
    }
    
    if (!_helperNode) {
        _helperNode = HitDetectHelperNode::create(Point::ZERO);
        _helperNode->retain();
    }
    else {
        _helperNode->reset();
    }
    
    _sharedRenderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
    
    if(auto sprite = dynamic_cast<Sprite* >(node)){
        if (sprite->getBatchNode()) {
            Sprite* tempSprite = Sprite::createWithTexture(sprite->getTexture());
            tempSprite->setTextureRect(sprite->getTextureRect(), sprite->isTextureRectRotated(), sprite->getContentSize());
            sprite = tempSprite;
        }
    }
    
    // store transforms
    Point position = node->getPosition();
    Point anchorPoint = node->getAnchorPoint();
    float rotationX = node->getRotationSkewX();
    float rotationY = node->getRotationSkewY();
    float skewX = node->getSkewX();
    float skewY = node->getSkewY();
    float scaleX = node->getScaleX();
    float scaleY = node->getScaleY();
    
    // unset transforms
    node->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    node->setPosition(Vec2(-point.x, -point.y) + Director::getInstance()->getVisibleOrigin());
    node->setRotationSkewX(0.0f);
    node->setRotationSkewY(0.0f);
    node->setSkewX(0.0f);
    node->setSkewY(0.0f);
    node->setScaleX(1.0f);
    node->setScaleY(1.0f);
    
    // draw node
    node->visit();
    
    // add command to read pixel
    _helperNode->visit();
    
    // revert transforms
    node->setAnchorPoint(anchorPoint);
    node->setPosition(position);
    node->setRotationSkewX(rotationX);
    node->setRotationSkewY(rotationY);
    node->setSkewX(skewX);
    node->setSkewY(skewY);
    node->setScaleX(scaleX);
    node->setScaleY(scaleY);
    
    _sharedRenderTexture->end();
    
    Director::getInstance()->getRenderer()->render(); // render texture fix
    
    uint8_t p0 = _helperNode->getPixelValue(0);
    uint8_t p1 = _helperNode->getPixelValue(1);
    uint8_t p2 = _helperNode->getPixelValue(2);
    uint8_t p3 = _helperNode->getPixelValue(3);
    
    return p0 || p1 || p2 || p3;
}

#pragma mark - HitDetectHelperNode

#pragma mark - Reset

void HitDetectHelper::HitDetectHelperNode::reset()
{
    _pixelRead = false;
    
    _pixelBuffer[0] = 0;
    _pixelBuffer[1] = 0;
    _pixelBuffer[2] = 0;
    _pixelBuffer[3] = 0;
}

#pragma mark - Pixel Buffer

uint8_t HitDetectHelper::HitDetectHelperNode::getPixelValue(unsigned int index)
{
    CCASSERT(_pixelRead, "Pixel not read yet.");
    CCASSERT(index < 4, "Out of bounds.");
    
    return _pixelBuffer[index];
}

#pragma mark - Draw

void HitDetectHelper::HitDetectHelperNode::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(HitDetectHelper::HitDetectHelperNode::readPixel, this);
    renderer->addCommand(&_customCommand);
}

#pragma mark - Read Pixel

void HitDetectHelper::HitDetectHelperNode::readPixel()
{
    _pixelRead = true;
    
    glReadPixels(_pixelPoint.x, _pixelPoint.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, _pixelBuffer);
}

#pragma mark - Constructors

HitDetectHelper::HitDetectHelperNode* HitDetectHelper::HitDetectHelperNode::create(const Point& pixelPoint)
{
    HitDetectHelper::HitDetectHelperNode* node = new HitDetectHelper::HitDetectHelperNode();
    if (node && node->init(pixelPoint)) {
        node->autorelease();
    }
    else {
        CC_SAFE_DELETE(node);
    }
    return node;
}

HitDetectHelper::HitDetectHelperNode::HitDetectHelperNode()

// instance variables
: _pixelRead(false)
{
}

bool HitDetectHelper::HitDetectHelperNode::init(const Point &pixelPoint)
{
    _pixelPoint = pixelPoint;
    
    reset();
    
    return Node::init();
}
