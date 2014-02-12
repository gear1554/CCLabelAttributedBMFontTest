//
//  CCLabelAttributedBMFont.cpp
//
//  Created by gear1554 on 2014/01/25.
//
//

#include "CCLabelAttributedBMFont.h"

#define NEWLINE_CHAR    "\n"
#define NULL_CHAR       ""

CCLabelAttributedBMFont::CCLabelAttributedBMFont() {
	dispIndex = 0;
	dispSpeed = 1;
	dispCycle = 5;
	isAllCharDisplayed = false;
	keyWords = CCDictionary::create();
	keyWords->retain();

	scheduleUpdate();
}

CCLabelAttributedBMFont::~CCLabelAttributedBMFont() {
	CCLOG("CCLabelAttributedBMFont ~CCLabelAttributedBMFont()");
	CC_SAFE_RELEASE_NULL(keyWords);
}

CCLabelAttributedBMFont *CCLabelAttributedBMFont::create(const char *str, const char *fntFile, float width, CCTextAlignment alignment) {
	return CCLabelAttributedBMFont::create(str, fntFile, width, alignment, CCPointZero);
}

CCLabelAttributedBMFont *CCLabelAttributedBMFont::create(const char *str, const char *fntFile, float width) {
	return CCLabelAttributedBMFont::create(str, fntFile, width, kCCTextAlignmentLeft, CCPointZero);
}

CCLabelAttributedBMFont *CCLabelAttributedBMFont::create(const char *str, const char *fntFile) {
	return CCLabelAttributedBMFont::create(str, fntFile, kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
}

CCLabelAttributedBMFont *CCLabelAttributedBMFont::create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset) {
	CCLabelAttributedBMFont *pRet = new CCLabelAttributedBMFont();
	if (pRet && pRet->initWithString(str, fntFile, width, alignment, imageOffset)) {
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

/**
   引数の文字列のマルチバイトを考慮した文字長を取得する
   @param str 対象文字列
   @return 文字長
 */
unsigned int getMuitiByteStringLength(const char *str) {
	string targetStr = string(str);
	int len = 0;
	for (int i = 0; i < targetStr.length(); ) {
		unsigned char code = str[i];
		((code >= 0x81 && code <= 0x9F) || (code >= 0xe0 && code <= 0xfc)) ? i += 3 : i++;
		len++;
	}
	return len;
}

/**
   文字色、文字サイズを局所的に変更したいキーワードを追加する
   @param keyword   キーワード
   @param fontColor キーワードの色
   @param fontScale キーワードのサイズ
 */
void CCLabelAttributedBMFont::addKeyWord(string keyword, ccColor3B fontColor, float fontScale) {
	// 改行コードの場合は何もしない
	if (keyword == NEWLINE_CHAR) {
		CCLOG("CCLabelAttributedBMFont addKeyWord() keyword is newline character!!!");
		return;
	}

	KeywordData *keyWordData = KeywordData::create();
	keyWordData->retain();
	keyWordData->setFontColor(fontColor);
	keyWordData->setFontScale(fontScale);

	keyWords->setObject(keyWordData, keyword);
	updateLabel();
}

/**
   キーワードを削除する
   @param 削除したいキーワード
 */
void CCLabelAttributedBMFont::removeKeyWord(string keyword) {
	keyWords->removeObjectForKey(keyword);
}

/**
   文字送り中の現在表示できているの文字列を取得する
   @return 現在表示途中の文字列
 */
string CCLabelAttributedBMFont::getVisbleString() {
	string str = string(m_sInitialStringUTF8);
	int len = 0;
	for (int i = 0; i < getChildrenCount(); i++) {
		CCSprite *mChar = (CCSprite *)getChildren()->objectAtIndex(i);
		if (!mChar->isVisible()) {
			len = i;
		}
	}

	string result = "";
	for (int i = 0; i < str.length(); ) {
		unsigned char code = str[i];
		((code >= 0x81 && code <= 0x9F) || (code >= 0xe0 && code <= 0xfc)) ? i += 3 : i++;
		result.append(string(&str[i]));
	}

	return result;
}

/**
   キーワードからラベル内のSpriteを配列で取得する
   @param 文字のSpriteが入った配列
 */
CCArray *CCLabelAttributedBMFont::getCharSpriteFromKeyword(string keyword) {
	KeywordData *keywordData = (KeywordData *)keyWords->objectForKey(keyword.c_str());
	keywordData->getKeyWordIndexes()->objectAtIndex(0);
	CCArray *keywordSprites = CCArray::create();
	for (int i = 0; i < keywordData->getKeyWordIndexes()->count(); i++) {
		KeywordIndex *keywordIndex = (KeywordIndex *)keywordData->getKeyWordIndexes()->objectAtIndex(i);
		keywordSprites->addObjectsFromArray(keywordIndex->getKeyWordSprites());
	}
	return keywordSprites;
}

/**
   文字列の置換
   @param targetStr 置換をかける文字列
   @param fromStr   置換対象文字列
   @param toStr     置換後文字列
   @return          置換完了後文字列
 */
string replace(string &targetStr, string fromStr, string toStr) {
	string::size_type pos = targetStr.find_first_of(fromStr, 0);
	while (string::npos != pos) {
		targetStr.replace(pos, string(fromStr).length(), toStr);
		pos = targetStr.find_first_of(fromStr, pos);
	}
	return targetStr;
}

/**
    ラベル上の文字色、文字サイズを初期化する
 */
void CCLabelAttributedBMFont::initKeyWordsVisualInfo() {
	dispIndex = 0;
	for (int i = 0; i < getChildrenCount(); i++) {
		CCSprite *mChar = (CCSprite *)getChildren()->objectAtIndex(i);
		mChar->setColor(ccWHITE);
		mChar->setScale(1.0f);
		mChar->setVisible(false);
	}
	isAllCharDisplayed = false;
}

/**
   文字送り中の文字を全て表示させる
 */
void CCLabelAttributedBMFont::dispAllCharacters() {
	dispIndex = getChildrenCount();
	for (int i = 0; i < getChildrenCount(); i++) {
		CCSprite *mChar = (CCSprite *)getChildren()->objectAtIndex(i);
		mChar->setVisible(true);
	}
	isAllCharDisplayed = true;
}

/**
    キーワードの存在する位置を検出する
 */
void CCLabelAttributedBMFont::searchKeywordsIndex() {
	// ラベル上に表示している文字列
	string newStr = string(m_sInitialStringUTF8);

	// 改行コードを除去
	newStr = replace(newStr, "\n", "");

	// 文字色、文字サイズを初期化
	initKeyWordsVisualInfo();

	CCObject *obj;
	CCArray *keys = keyWords->allKeys();
	// 登録したキーワード分だけ更新処理を行う
	CCARRAY_FOREACH(keys, obj)
	{
		CCString *key = (CCString *)obj;
		if (key->retainCount() != 1) return;

		string keyStr = string(key->getCString());                          // キーワード
		int keyLengthMultiByte = getMuitiByteStringLength(keyStr.c_str());  // キーワードの長さ(マルチバイト)

		unsigned int searchIndex = 0;                                       // 現在の検出走査位置
		unsigned int keyFindIndex = newStr.find(keyStr, searchIndex);       // キーワードが検出された位置

		while (keyFindIndex != string::npos) {
			unsigned int keyFindIndexMultiByte = 0;                         // キーワードが検出された位置(マルチバイト)

			// キーワードが検出された位置(マルチバイト)を検出
			string clipStr = newStr.substr(0, keyFindIndex);
			keyFindIndexMultiByte = getMuitiByteStringLength(clipStr.c_str());

			KeywordIndex *keywordIndex = KeywordIndex::create();
			keywordIndex->setStartIndex(keyFindIndexMultiByte);
			keywordIndex->setEndIndex(keyFindIndexMultiByte + keyLengthMultiByte);

			KeywordData *keyWordData = (KeywordData *)keyWords->objectForKey(keyStr);
			keyWordData->getKeyWordIndexes()->addObject(keywordIndex);

			// 現在の検出走査位置をキーワード終了位置へ
			searchIndex = (keyFindIndex + keyStr.length());

			// キーワード終了位置以降に存在するキーワードを検出
			keyFindIndex = newStr.find(keyStr, searchIndex);
		}
	}
}

/**
   ラベル上に該当するキーワードがあればキーワードの文字色、文字サイズを変更する
 */
void CCLabelAttributedBMFont::updateKeyWordsVisualInfo() {
	CCObject *keyObj;
	CCArray *keys = keyWords->allKeys();
	// 登録したキーワード分だけ更新処理を行う
	CCARRAY_FOREACH(keys, keyObj)
	{
		CCString *key = (CCString *)keyObj;
		KeywordData *keyWordData = (KeywordData *)keyWords->objectForKey(key->getCString());
		CCObject *keywordIndexObj;
		CCARRAY_FOREACH(keyWordData->getKeyWordIndexes(), keywordIndexObj)
		{
			KeywordIndex *keywordIndex = (KeywordIndex *)keywordIndexObj;
			for (int i = keywordIndex->getStartIndex(); i < keywordIndex->getEndIndex(); i++) {
				CCSprite *charSprite = (CCSprite *)getChildren()->objectAtIndex(i);
				charSprite->setScale(keyWordData->getFontScale());
				charSprite->setColor(keyWordData->getFontColor());
				keywordIndex->getKeyWordSprites()->addObject(charSprite);
			}
		}
	}
}

/**
    ラベルの更新
 */
void CCLabelAttributedBMFont::updateLabel() {
	dispIndex = 0;

	CCLabelBMFont::updateLabel();

	// キーワードの存在する位置を検出する
	searchKeywordsIndex();

	// キーワードの文字色、文字サイズを更新
	updateKeyWordsVisualInfo();
}

/**
    更新、文字送りを行う
    @param dt 前回コールされてからの経過時間
 */
void CCLabelAttributedBMFont::update(float dt) {
	if (CCDirector::sharedDirector()->getTotalFrames() % dispCycle == 0) {
		for (int i = 0; i < dispIndex; i++) {
			if (dispIndex <= getChildren()->count()) {
				CCSprite *sprite = (CCSprite *)getChildren()->objectAtIndex(i);
				sprite->setVisible(true);
			}
		}

		dispIndex += dispSpeed;
		if (dispIndex > getChildrenCount()) {
			dispIndex = getChildrenCount();
			isAllCharDisplayed = true;
		}
	}
}
