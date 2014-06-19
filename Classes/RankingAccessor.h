//
//  RankingAccessor.h
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#ifndef ___5Puzzle__RankingAccessor__
#define ___5Puzzle__RankingAccessor__

#include "cocos2d.h"
#include "prettywriter.h"
#include "reader.h"
#include "document.h"
#include "stringbuffer.h"

class RankingAccessor {
//    cocos2d::extension::Json * _rankings;
    rapidjson::Document _rankings;
    const char * _fullJsonFilePath;
    int _rankingCount;
    bool _validRanking;
    
public:
    RankingAccessor(const char * const fileName);
    ~RankingAccessor();
    bool isValid() const;
    
    int getRankingCount() const;
    const char * getUserAtIndex(unsigned int i) const;
    int getTimeAtIndex(unsigned int i) const;
    
    void insertRankingItem(const char * name, int time);
    
    static RankingAccessor * createRankingAccessor(const char * const fileName);
    
};

#endif /* defined(___5Puzzle__RankingAccessor__) */
