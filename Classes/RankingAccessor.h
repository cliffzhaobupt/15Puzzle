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

 
USING_NS_CC;
using namespace rapidjson;

class RankingAccessor {
    //rapidjson::Document class
    //to hold the whole JSON content
    Document _rankings;
    //the absolute path of the json file
    const char * _fullJsonFilePath;
    //count of ranking items
    int _rankingCount;
    //whether the ranking file is parsed successfully
    bool _validRanking;
    
public:
    RankingAccessor(const char * const fileName);
    bool isValid() const;
    
    int getRankingCount() const;
    const char * getUserAtIndex(unsigned int i) const;
    int getTimeAtIndex(unsigned int i) const;
    
    void insertRankingItem(const char * name, int time);
    
    static RankingAccessor * createRankingAccessor(const char * const fileName);
    
};

#endif /* defined(___5Puzzle__RankingAccessor__) */
