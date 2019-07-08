/*
 * Utils.h
 *
 *  Created on: 24 lut 2015
 *      Author: piotr
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <sstream>
#include "cocos2d.h"
#include "AppDelegate.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "SimpleAudioEngine.h"
#else
#include "AudioEngine.h"
#endif
#include <spine/spine-cocos2dx.h>
#include <stdio.h>
#include "cocostudio/CocoStudio.h"
#include "ui/UIButton.h"
#include "NativeHelper.h"
#include "AppDelegate.h"


USING_NS_CC;
using namespace std;
using namespace spine;

//----

//extern int gameInLevelNumber; // for BlocksScene
extern int blocksNodeNumber; // for BlocksScene
extern int blocksLevel; // for BlocksScene
extern int waterTime;
extern int basketTime;
extern int bigDogTime;

extern int basketPointsTime;
//extern int blocksPointsTime;
//----

extern bool save;
extern int backgroundMusicSoundId;
extern string letterNameMini;
extern int sequenceGame1;
extern int letterNumber;
extern bool letterMini;
std::string to_str(int t);
std::string to_str(float t);
std::string to_str(long t);
std::string to_str(double t);
std::string int_to_str(int i);
int from_str_to_int(std::string t);
long from_str_to_long(std::string t);
double from_str_to_double(std::string t);
float from_str_to_float(std::string t);
bool isSpriteTouched(Vec2 pos, Node* child, float extraWidth = 0.0f, float extraHeight = 0.0f);
void lookAt(Node* node, cocos2d::Point from, cocos2d::Point to, float radius);
ParticleSystemQuad* createEmitter(string filename, int totalParticles = 100);
void playEmitterAt(cocos2d::Point pos, Vector<ParticleSystemQuad*> *memitters, bool burstAtStart, float emissionRate = 15, int index = -1);
void playEmitterOnce(cocos2d::Point pos, Vector<ParticleSystemQuad* > *memitters, int amount, float time, int index = -1);
void stopEmitters(Vector<ParticleSystemQuad*> *memitters);

void uPlayBackgroundMusic(string filenameWithoutExtension);
void uStopBackgroundMusic();
unsigned int uPlayEffect(string filenameWithoutExtension, bool loop);
unsigned int uPlayEffect(string filenameWithoutExtension);
void uPreloadEffect(string filenameWithoutExtension);
void uUnloadEffect(string filenameWithoutExtension);
void uPauseEffect(unsigned int soundId);
void uStopEffect(unsigned int soundId);
void uResumeEffect(unsigned int soundId);
bool uIsBackgroundMusicPlaying();
void translateNode(Node* node, Vec2 vec);

void replaceSceneForward(Scene* scene, Vec2 p = Vec2::ZERO);
void replaceSceneBackward(Scene* scene, Vec2 p = Vec2::ZERO);
spine::SkeletonAnimation* initSpineModel(Node* parent, std::string skeletonBinaryFile, std::string atlasFile, float scale, std::string defaultAnim);

void fillWithFrames(Vector<SpriteFrame* > *v, string name, int from, int to, int back, int repeat = 1);
void addFrame(Vector<SpriteFrame* > *v, string name, int index);

template<typename T> void split(const std::string &s, char delim, T result);
std::vector<std::string> split(const std::string &s, char delim);

string getRandomItem(vector<string> items);

void setMultipleMixes(SkeletonAnimation* anim, vector<string> names1, vector<string> names2, bool both = true, float time = 0.5f);
void setAllMixes(SkeletonAnimation* anim, float time = 0.5f);

void bringToFront(Node* node);


#endif /* UTILS_H_ */
