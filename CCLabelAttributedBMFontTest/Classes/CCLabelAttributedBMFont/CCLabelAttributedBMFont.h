//
//  CCLabelAttributedBMFont.h
//
//  Created by gear1554 on 2014/01/25.
//
//

#ifndef __CCLabelAttributedBMFont__
#define __CCLabelAttributedBMFont__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

// カスタムビットマップフォントラベルクラス
class CCLabelAttributedBMFont : public CCLabelBMFont{
private:
    
    CCLabelAttributedBMFont();
    ~CCLabelAttributedBMFont();
    
    CCDictionary *keyWords;
    
    /**
        キーワードの存在する位置を検出する
    */
    void searchKeywordsIndex();
    
    /**
        ラベル上に該当するキーワードがあればキーワードの文字色、文字サイズを変更する
    */
    void updateKeyWordsVisualInfo();
    
    /**
        ラベル上の文字色、文字サイズを初期化する
    */
    void initKeyWordsVisualInfo();
    
public:
    
    // 文字送りが完了している位置
    CC_SYNTHESIZE(unsigned int, dispIndex, DispIndex);
    
    // 何フレーム周期で文字送りをするか
    CC_SYNTHESIZE(unsigned int, dispCycle, DispCycle);
    
    // 文字送りする際は何文字ずつ行うか
    CC_SYNTHESIZE(unsigned int, dispSpeed, DispSpeed)
    
    // 文字送りが完了状態かどうか
    CC_SYNTHESIZE(bool, isAllCharDisplayed, IsAllCharDisplayed);
    
    /**
        文字色、文字サイズを局所的に変更したいキーワードを追加する
        @param keyword   キーワード
        @param fontColor キーワードの色
        @param fontScale キーワードのサイズ
    */
    void addKeyWord(string keyword, ccColor3B fontColor, float fontScale);
    
    /**
        キーワードを削除する
        @param 削除したいキーワード
    */
    void removeKeyWord(string keyword);
    
    /**
        文字送り中の現在表示できているの文字列を取得する
        @return 現在表示途中の文字列
    */
    string getVisbleString();
    
    /**
        キーワードからラベル内のSpriteを配列で取得する
        @param キーワード
        @return 文字のSpriteが入った配列
    */
    CCArray* getCharSpriteFromKeyword(string keyword);
    
    /**
     文字送り中の文字を全て表示させる
     */
    void dispAllCharacters();

    /**
        ラベルの更新
    */
    virtual void updateLabel();
    
    virtual void update(float dt);
    
    CREATE_FUNC(CCLabelAttributedBMFont);
    
    static CCLabelAttributedBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset);
	static CCLabelAttributedBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment);
	static CCLabelAttributedBMFont * create(const char *str, const char *fntFile, float width);
	static CCLabelAttributedBMFont * create(const char *str, const char *fntFile);
};


// キーワードの位置情報クラス
class KeywordIndex : public CCObject{
private:
     ~KeywordIndex(){
         CC_SAFE_RELEASE_NULL( keyWordSprites );
     };
    
public:
    CC_SYNTHESIZE( unsigned int, startIndex, StartIndex );
    CC_SYNTHESIZE( unsigned int, endIndex, EndIndex );
    CC_SYNTHESIZE(CCArray*, keyWordSprites, KeyWordSprites); // キーワードへの参照情報
    bool init(){
        startIndex = 0;
        endIndex = 0;
        keyWordSprites = CCArray::create();
        keyWordSprites->retain();
        return true;
    };
    
    CREATE_FUNC(KeywordIndex);
    
};


// キーワードに紐付く文字色、文字サイズ情報クラス
class KeywordData : public CCObject{
private:
    ~KeywordData(){
        CC_SAFE_RELEASE_NULL( keyWordIndexes );
    };
    
public:
    
    // 文字色
    CC_SYNTHESIZE(ccColor3B, fontColor, FontColor);
    
    // 文字サイズ
    CC_SYNTHESIZE(float, fontScale, FontScale);
    
    // キーワード文字列位置情報
    CC_SYNTHESIZE(CCArray*, keyWordIndexes, KeyWordIndexes);
    
    bool init(){
        fontColor = ccWHITE;
        fontScale = 1.0f;
        
        keyWordIndexes = CCArray::create();
        keyWordIndexes->retain();
        
        return true;
    };
    
    CREATE_FUNC(KeywordData);
};

#endif /* defined(__CCLabelAttributedBMFont__) */
