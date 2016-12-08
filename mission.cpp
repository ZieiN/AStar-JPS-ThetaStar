#include "mission.h"

Mission::Mission()
{
    logger = NULL;
    search = NULL;
    fileName = NULL;
}

Mission::Mission(const char* FileName)
{
    fileName = FileName;
    logger = NULL;
    search = NULL;
}

Mission::~Mission()
{
    if (logger) delete logger;
    if (search) delete search;
}

bool Mission::getMap()
{
    return map.getMap(fileName);
}

bool Mission::getConfig()
{
    return config.getConfig(fileName);
}

bool Mission::createLog()
{
    logger = new XmlLogger();
    logger->loglevel = config.SearchParams[CN_SP_LL];
    return logger -> getLog (fileName, config.LogParams);
}

void Mission::createEnvironmentOptions()
{
    options = EnvironmentOptions(config.SearchParams[CN_SP_MT], config.SearchParams[CN_SP_AS], config.SearchParams[CN_SP_LC],
                                 config.SearchParams[CN_SP_DC], config.SearchParams[CN_SP_AD], config.SearchParams[CN_SP_RP]);
}

void Mission::createSearch()
{
    if (config.SearchParams[CN_SP_ST] == CN_SP_ST_BFS)
    {
        search = new BFS();
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_DIJK)
    {
        search = new Dijkstra();
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_ASTAR)
    {
        search = new Astar(config.SearchParams[CN_SP_HW], config.SearchParams[CN_SP_BT], config.SearchParams[CN_SP_SL]);
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_JP_SEARCH)
    {
        search=new JP_Search(config.SearchParams[CN_SP_HW], config.SearchParams[CN_SP_BT], config.SearchParams[CN_SP_SL]);
    }
    else if (config.SearchParams[CN_SP_ST]== CN_SP_ST_TH)
    {
        search = new Theta (config.SearchParams[CN_SP_HW], config.SearchParams[CN_SP_BT], config.SearchParams[CN_SP_SL]);
    }

}

void Mission::startSearch()
{
    sr=search->startSearch(logger, map, options);
}

void Mission::printSearchResultsToConsole()
{

        std::cout << "Path ";
        if (!sr.pathfound)
            std::cout << "NOT ";
        std::cout << "found!" << std::endl;

        std::cout << "numberofsteps=" << sr.numberofsteps << std::endl;
        std::cout << "nodescreated=" << sr.nodescreated << std::endl;

        if (sr.pathfound)
            std::cout << "pathlength=" << sr.pathlength << std::endl;
        std::cout << "time=" << sr.time << std::endl;

}

void Mission::saveSearchResultsToLog()
{
    logger->writeToLogSummary(sr.numberofsteps, sr.nodescreated, sr.pathlength, sr.time);

    if (sr.pathfound)
    {
        logger->writeToLogPath(*sr.lppath);
        logger->writeToLogHPpath(*sr.hppath);
        logger->writeToLogMap(map,*sr.lppath);
    }
    else
        logger->writeToLogNotFound();
    logger->saveLog();
}

const char* Mission::getAlgorithmName ()
{
    if (config.SearchParams[CN_SP_ST] == CN_SP_ST_ASTAR)
    {
        return CNS_SP_ST_ASTAR;
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_DIJK)
    {
        return CNS_SP_ST_DIJK;
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_BFS)
    {
        return CNS_SP_ST_BFS;
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_JP_SEARCH)
    {
        return CNS_SP_ST_JP_SEARCH;
    }
    else if (config.SearchParams[CN_SP_ST] == CN_SP_ST_TH)
    {
        return CNS_SP_ST_TH;
    }
    else
    {
        return "";
    }
}