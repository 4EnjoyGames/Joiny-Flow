#ifndef STATISTICS_H
#define STATISTICS_H
class Statistics
{
public:
    enum StatisticsEvent
    {
        OnMoreLevelsClick = 100,
        OnDevelopersClick = 200,
        UseHint = 300,
        OnSkipLevel = 400,
        OnLevelComplete = 500
    };

    static void statisticEvent(const StatisticsEvent id);
    static void detailStatisticEvent(const StatisticsEvent id,
                                  const unsigned int level_id,
                                  const unsigned int collection_id,
                                  const unsigned int stamps_num);
};
#endif // STATISTICS_H
