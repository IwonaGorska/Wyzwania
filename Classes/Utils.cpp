#include "Utils.h"
#include <errno.h>
#include <ctime>

//------

int blocksNodeNumber = 1;
int blocksLevel = 1;
int waterTime = 1;
int basketTime = 1;
bool save = false;
int bigDogTime = 1;

int basketPointsTime = 1;
//int blocksPointsTime = 1;

//bool isLoop = true;
//-------


int backgroundMusicSoundId = -1;

std::string to_str(int t) {
    std::stringstream ss;
    ss << t;
    std::string str = ss.str();
    return str;
}

std::string to_str(float t) {
    std::stringstream ss;
    ss << t;
    std::string str = ss.str();
    return str;
}

std::string to_str(long t) {
    std::stringstream ss;
    ss << t;
    std::string str = ss.str();
    return str;
}

std::string to_str(double t) {
    std::stringstream ss;
    ss << t;
    std::string str = ss.str();
    return str;
}

int from_str_to_int(std::string t){
    const char* buffer = t.c_str();
    int i = atoi(buffer);
    return i;
}

long from_str_to_long(std::string t){
    const char* buffer = t.c_str();
    long i = atol(buffer);
    return i;
}

double from_str_to_double(std::string t){
    const char* buffer = t.c_str();
    double i = atof(buffer);
    return i;
}

float from_str_to_float(std::string t){
    const char* buffer = t.c_str();
    float i = atof(buffer);
    return i;
}

bool isSpriteTouched(Vec2 pos, Node* child, float extraWidth, float extraHeight){
    Size childSize = child->getContentSize();
    
    float sx = child->getScaleX();
    float sy = child->getScaleY();
    if(sx == 0.0f) sx = 0.00001f;
    if(sy == 0.0f) sy = 0.00001f;
    
    if(sx < 0) sx *= -1.0f;
    if(sy < 0) sy *= -1.0f;
    Size extraSize = Size(extraWidth * (1.0f / sx), extraHeight * (1.0f / sy));
    child->setContentSize(child->getContentSize() + extraSize);
    
    Vec2 p = child->convertToNodeSpace(pos);
    
    bool result = Rect(0, 0, child->getContentSize().width, child->getContentSize().height).containsPoint(p);
    
    child->setContentSize(childSize);
    
    return result;
    
    //    return Rect(0, 0, child->getContentSize().width + extraWidth, child->getContentSize().height + extraHeight).containsPoint(child->convertToNodeSpace(pos));
}

void lookAt(Node* node, cocos2d::Point from, cocos2d::Point to, float radius){
    float deltaX = to.x - from.x;
    float deltaY = to.y - from.y;
    float angle = atan2f(deltaY, deltaX);
    Vec2 pos = Vec2(cosf(angle) * radius + from.x, sinf(angle) * radius + from.y);
    node->setPosition(pos);
}

void playEmitterAt(cocos2d::Point pos, Vector<ParticleSystemQuad* > *memitters, bool burstAtStart, float emissionRate, int index){
    for(int i = 0; i < memitters->size(); i++){
        if((i == index && index != -1) || index == -1){
            auto emitter = memitters->at(i);
            
            emitter->setPosition(pos);
            emitter->setEmissionRate(burstAtStart ? 20 : emissionRate);
            
            emitter->stopAllActions();
            if (burstAtStart){
                emitter->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([emitter, emissionRate]{if (emitter->getEmissionRate() > 0) emitter->setEmissionRate(emissionRate); }), NULL));
            }
        }
    }
}

void playEmitterOnce(cocos2d::Point pos, Vector<ParticleSystemQuad* > *memitters, int amount, float time, int index){
    for(int i = 0; i < memitters->size(); i++){
        if((i == index && index != -1) || index == -1){
            auto emitter = memitters->at(i);
            
            emitter->setPosition(pos);
            emitter->setEmissionRate(amount);
            
            emitter->stopAllActions();
            emitter->runAction(Sequence::create(DelayTime::create(time), CallFunc::create([emitter]{emitter->setEmissionRate(0); }), NULL));
        }
    }
}

void stopEmitters(Vector<ParticleSystemQuad*> *memitters){
    for(int i = 0; i < memitters->size(); i++){
        auto emitter = memitters->at(i);
        emitter->stopAllActions();
        emitter->setEmissionRate(0);
    }
}

ParticleSystemQuad* createEmitter(string filename, int totalParticles){
    auto emitter = ParticleSystemQuad::createWithTotalParticles(totalParticles);
    auto director = Director::getInstance();
    director->getTextureCache()->addImage(filename);
    emitter->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(filename.c_str()));
    emitter->setDuration(-1);
    emitter->setAngle(0);
    emitter->setAngleVar(180);
    emitter->setSpeed(150);
    emitter->setSpeedVar(50);
    emitter->setPosVar(Point(0, 0));
    emitter->setPosition(Point(0, 0));
    emitter->setLife(4);
    emitter->setLifeVar(0);
    emitter->setStartSpin(0);
    emitter->setStartSpinVar(0);
    emitter->setEndSpin(1440);
    emitter->setEndSpinVar(0);
    emitter->setStartColor(Color4F(1, 1, 1, 1));
    emitter->setStartColorVar(Color4F(0, 0, 0, 0));
    emitter->setEndColor(Color4F(1, 1, 1, 1));
    emitter->setEndColorVar(Color4F(0, 0, 0, 0));
    emitter->setStartSize(30);
    emitter->setStartSizeVar(0);
    emitter->setEndSize(0);
    emitter->setEndSizeVar(0);
    emitter->setEmissionRate(0);
    
    return emitter;
}

void uPlayBackgroundMusic(string filenameWithoutExtension){
    string extension = SOUNDS_EXT;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    string tfilename = filenameWithoutExtension + ".caf";
    if(FileUtils::getInstance()->isFileExist(tfilename)){
        extension = "caf";
    }
#endif
    string filename = filenameWithoutExtension + "." + extension;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(backgroundMusicSoundId != -1){
        uStopBackgroundMusic();
    }
    backgroundMusicSoundId = cocos2d::experimental::AudioEngine::play2d(filename, true);
    cocos2d::experimental::AudioEngine::setLoop(backgroundMusicSoundId, true);
    CCLOG("uPlayBackgroundMusic: %d", backgroundMusicSoundId);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), true);
#endif
}

void uStopBackgroundMusic() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (backgroundMusicSoundId != -1) {
        cocos2d::experimental::AudioEngine::stop(backgroundMusicSoundId);
        backgroundMusicSoundId = -1;
    }
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
#endif
}

void uPauseEffect(unsigned int soundId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::experimental::AudioEngine::pause(soundId);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseEffect(soundId);
#endif
}

void uStopEffect(unsigned int soundId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::experimental::AudioEngine::stop(soundId);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(soundId);
#endif
}

void uResumeEffect(unsigned int soundId){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::experimental::AudioEngine::resume(soundId);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeEffect(soundId);
#endif
}

unsigned int uPlayEffect(string filenameWithoutExtension, bool loop){
    string extension = SOUNDS_EXT;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    string tfilename = filenameWithoutExtension + ".caf";
    if(FileUtils::getInstance()->isFileExist(tfilename)){
        extension = "caf";
    }
#endif
    string filename = filenameWithoutExtension + "." + extension;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    int r = cocos2d::experimental::AudioEngine::play2d(filename, loop);
    if(r < 0){
        CCLOG("uPlayEffect(%s, %s) returned %d", filename.c_str(), (loop ? "true" : "false"), r);
        return 0;
    }
    return r;
#else
    return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(filename.c_str(), loop);
#endif
}

unsigned int uPlayEffect(string filenameWithoutExtension){
    return uPlayEffect(filenameWithoutExtension, false);
}

void uPreloadEffect(string filenameWithoutExtension){
    string filename = filenameWithoutExtension + "." + SOUNDS_EXT;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::experimental::AudioEngine::preload(filename);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(filename.c_str());
#endif
}

void uUnloadEffect(string filenameWithoutExtension){
    string filename = filenameWithoutExtension + "." + SOUNDS_EXT;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::experimental::AudioEngine::uncache(filename);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(filename.c_str());
#endif
}

bool uIsBackgroundMusicPlaying(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(backgroundMusicSoundId == -1) return false;
    auto state = cocos2d::experimental::AudioEngine::getState(backgroundMusicSoundId);
    return state == cocos2d::experimental::AudioEngine::AudioState::PLAYING;
#else
    return CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
#endif
}

void translateNode(Node* node, Vec2 vec){
    node->setPosition(node->getPosition() + vec);
}

void replaceSceneForward(Scene* scene, Vec2 p){
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

void replaceSceneBackward(Scene* scene, Vec2 p){
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}

SkeletonAnimation* initSpineModel(Node* parent, std::string skeletonBinaryFile, std::string atlasFile, float scale, std::string defaultAnim){
    auto model = SkeletonAnimation::createWithBinaryFile(skeletonBinaryFile, atlasFile, scale);
    if(defaultAnim != ""){
        model->setAnimation(0, defaultAnim, true);
    }
    model->update(0.0f);
    model->setTag(999);
    if(parent != nullptr){
        model->setPosition(Vec2(parent->getContentSize().width * parent->getAnchorPoint().x, parent->getContentSize().height * parent->getAnchorPoint().y));
        parent->addChild(model);
    }
    return model;
}

void fillWithFrames(Vector<SpriteFrame* > *v, string name, int from, int to, int back, int repeat){
    if(from == to) return;
    int d = from < to ? 1 : -1;
    for(int r = 0; r < repeat; r++){
        
        int i = from;
        while(i != to){
            addFrame(v, name, i);
            i += d;
        }
        if(to == back){
            addFrame(v, name, to);
            continue;
        }
        while(i != back){
            addFrame(v, name, i);
            i -= d;
        }
        addFrame(v, name, back);
    }
}

void addFrame(Vector<SpriteFrame* > *v, string name, int index){
    char str[100] = {0};
    sprintf(str, name.c_str(), index);
    auto sprite = Sprite::create(str);
    //    CCLOG("%s", str);
    //    CCLOG("%d", index);
    v->pushBack(sprite->getSpriteFrame());
}

template<typename T>
void split(const std::string &s, char delim, T result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

string getRandomItem(vector<string> items){
    if(items.size() == 0) return "";
    int i = random(0, (int) items.size() - 1);
    return items.at(i);
}

void setMultipleMixes(SkeletonAnimation* anim, vector<string> names1, vector<string> names2, bool both, float time){
    for(int j = 0; j < names2.size(); j++){
        for(int k = 0; k < names1.size(); k++){
            string n1 = names1.at(k);
            string n2 = names2.at(j);
            anim->setMix(n1, n2, time);
            if(both) anim->setMix(n2, n1, time);
        }
    }
}

void setAllMixes(SkeletonAnimation* anim, float time){
    vector<string> animations;
    for(int i = 0; i < anim->getState()->data->skeletonData->animationsCount; i++){
        auto a = anim->getState()->data->skeletonData->animations[i];
        string animation = a->name;
        animations.push_back(animation);
    }
    setMultipleMixes(anim, animations, animations, true, time);
}

void bringToFront(Node* node){
    auto parent = node->getParent();
    node->retain();
    node->removeFromParentAndCleanup(false);
    parent->addChild(node);
    node->release();
}

