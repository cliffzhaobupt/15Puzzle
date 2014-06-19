//
//  RankingAccessor.cpp
//  15Puzzle
//
//  Created by Cliff Zhao on 14-6-18.
//
//

#include "RankingAccessor.h"

//constructor
//parse json in the file
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

//static member function
RankingAccessor * RankingAccessor::createRankingAccessor(const char *const fileName) {
    while (true) {
        RankingAccessor * raCreated = new RankingAccessor(fileName);
        if (raCreated->isValid()) {
            return raCreated;
        }
        delete raCreated;
    }
}

//whether parsed successfully
bool RankingAccessor::isValid() const {
    return _validRanking;
}

//how many ranking items in the ranking
int RankingAccessor::getRankingCount() const {
    return _rankingCount;
}

//get user name of one item in the ranking
const char * RankingAccessor::getUserAtIndex(unsigned int i) const {
    return _rankings[i]["user"].GetString();
}

//get time of one item in the ranking
int RankingAccessor::getTimeAtIndex(unsigned int i) const {
    return _rankings[i]["time"].GetInt();
}

//insert one item into the ranking
void RankingAccessor::insertRankingItem(const char *name, int time) {
    Document newRankings;
    newRankings.SetArray();
    
    Document::AllocatorType & allocator = newRankings.GetAllocator();
    
    bool checkWhetherInsert = true;
    for (unsigned int i = 0, size = getRankingCount() ; i < size ; ++ i) {
        if (checkWhetherInsert) {
            if (time > getTimeAtIndex(i)) {
                newRankings.PushBack(_rankings[i], allocator);
            } else {
                Value newItem;
                newItem.SetObject();
                newItem.AddMember("user", name, allocator);
                newItem.AddMember("time", time, allocator);
                newRankings.PushBack(newItem, allocator);
                newRankings.PushBack(_rankings[i], allocator);
                checkWhetherInsert = false;
            }
        } else {
            newRankings.PushBack(_rankings[i], allocator);
        }
    }
    
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    newRankings.Accept(writer);
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