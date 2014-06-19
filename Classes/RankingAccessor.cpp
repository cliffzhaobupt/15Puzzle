//
//  RankingAccessor.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#include "RankingAccessor.h"

USING_NS_CC;
using namespace rapidjson;

RankingAccessor::RankingAccessor(const char * const fileName) {
    unsigned long jsonFileSize;
    const char * jsonText = (const char *) CCFileUtils::sharedFileUtils()->getFileData(fileName, "r+", & jsonFileSize);
    _rankings.Parse<kParseDefaultFlags>(jsonText);
    _fullJsonFilePath = CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName).c_str();
    if (_rankings.IsArray()) {
        _rankingCount = _rankings.Size();
        _validRanking = true;
    } else {
        _rankingCount = 0;
        _validRanking = false;
    }
}

RankingAccessor * RankingAccessor::createRankingAccessor(const char *const fileName) {
    while (true) {
        RankingAccessor * raCreated = new RankingAccessor(fileName);
        if (raCreated->isValid()) {
            return raCreated;
        }
        delete raCreated;
    }
}


RankingAccessor::~RankingAccessor() {

}

bool RankingAccessor::isValid() const {
    return _validRanking;
}

int RankingAccessor::getRankingCount() const {
    return _rankingCount;
}

const char * RankingAccessor::getUserAtIndex(unsigned int i) const {
    return _rankings[i]["user"].GetString();
}

int RankingAccessor::getTimeAtIndex(unsigned int i) const {
    return _rankings[i]["time"].GetInt();
}

void RankingAccessor::insertRankingItem(const char *name, int time) {
    Document newRankings;
    
    Document::AllocatorType & allocator = newRankings.GetAllocator();
    
    Value root(kArrayType);
    bool checkWhetherInsert = true;
    for (unsigned int i = 0, size = getRankingCount() ; i < size ; ++ i) {
        if (checkWhetherInsert) {
            if (time > getTimeAtIndex(i)) {
                root.PushBack(_rankings[i], allocator);
            } else {
                Value newItem(kObjectType);
                newItem.AddMember("user", name, allocator);
                newItem.AddMember("time", time, allocator);
                root.PushBack(newItem, allocator);
                root.PushBack(_rankings[i], allocator);
                checkWhetherInsert = false;
            }
        } else {
            root.PushBack(_rankings[i], allocator);
        }
    }
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    root.Accept(writer);
    const char * newRankingsJson = buffer.GetString();
    FILE * file = fopen(_fullJsonFilePath, "w+");
    fputs(newRankingsJson, file);
    fclose(file);
    
    ++ _rankingCount;
    while (true) {
        _rankings.Parse<kParseDefaultFlags>(newRankingsJson);
        if (_rankings.IsArray()) {
            break;
        }
    }

}