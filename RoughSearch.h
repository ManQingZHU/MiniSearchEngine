//
// Created by 朱曼青 on 2017/3/21.
//

#ifndef SEARCHENGINE_FINAL_ROUGHSEARCH_H
#define SEARCHENGINE_FINAL_ROUGHSEARCH_H

#include "InvFileInd.h"
#include "Stemming.h"

void RoughSearch(string dirname, std::vector<std::string>& Query, int MAXRESULTNUM, double THRESHOLD);

#endif //SEARCHENGINE_FINAL_ROUGHSEARCH_H
