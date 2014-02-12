#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CCLabelAttributedBMFont.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene *HelloWorld::scene() {
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();

	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}


	string str = "吾輩は猫である。名前はまだ無い。\nどこで生れたかとんと見当けんとうがつかぬ。\n何でも薄暗いじめじめした所で\nニャーニャー泣いていた事だけは\n記憶している。";

	// create方法はCCLabelBMFontと同様でOK
	CCLabelAttributedBMFont *attributeLabel = CCLabelAttributedBMFont::create(str.c_str(), "font.fnt");
	attributeLabel->setAnchorPoint(ccp(0.0f, 1.0f));
	attributeLabel->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height - 30));

	// キーワードと対応する色、サイズを設定
	attributeLabel->addKeyWord("猫", ccRED, 1.5f);
	attributeLabel->addKeyWord("ニャーニャー", ccORANGE, 0.8f);

	// 5フレームに1文字送る設定
	attributeLabel->setDispCycle(5);
	attributeLabel->setDispSpeed(1);
	addChild(attributeLabel);


	string strEn = "I am a cat.\nAs yet I have no name.\nI've no idea where I was born.\nI only remember that I cried 'meow meow' in a dark and damp.";

	// create方法はCCLabelBMFontと同様でOK
	CCLabelAttributedBMFont *attributeLabelEn = CCLabelAttributedBMFont::create(strEn.c_str(), "font.fnt");
	attributeLabelEn->setAnchorPoint(ccp(0.0f, 1.0f));
	attributeLabelEn->setPosition(ccp(0, CCDirector::sharedDirector()->getWinSize().height / 2));

	// キーワードと対応する色、サイズを設定
	attributeLabelEn->addKeyWord("cat", ccRED, 1.4f);
	attributeLabelEn->addKeyWord("'meow meow'", ccORANGE, 0.8f);

	// 1フレームに2文字送る設定
	attributeLabelEn->setDispCycle(1);
	attributeLabelEn->setDispSpeed(2);
	addChild(attributeLabelEn);


	return true;
}

void HelloWorld::menuCloseCallback(CCObject *pSender) {
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
