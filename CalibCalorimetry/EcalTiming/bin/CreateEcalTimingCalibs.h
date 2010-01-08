#include <vector>
#include <iostream>
#include <algorithm>

// ****************************************************************
class TimingEvent
{
  public:
    float amplitude;
    float time;
    float chi2;
    float sigmaTime;
    float expectedPrecision;

    TimingEvent() :
      amplitude(-1),
      time(-1),
      chi2(-1),
      sigmaTime(-1),
      expectedPrecision(-1)
    {
    }
    
    TimingEvent(float amp,float t,float sigmaT, bool ee) : 
      amplitude(amp), 
      time(t), 
      chi2(-1),
      sigmaTime(sigmaT)
    {
      if(ee)
        expectedPrecision = 33/(amplitude/2.0);
      else
        expectedPrecision = 33/(amplitude/1.2);
    }

};

class CrystalCalibration
{
  public:
    float mean;
    float sigma;
    float totalChi2;
    std::vector<TimingEvent> timingEvents;
    std::vector<TimingEvent>::iterator maxChi2Itr;

    CrystalCalibration() :
      mean(-1),
      sigma(-1),
      totalChi2(-1),
      useWeightedMean(true)
    {
    }
    
    CrystalCalibration(bool weightMean) :
      mean(-1),
      sigma(-1),
      totalChi2(-1),
      useWeightedMean(weightMean)
    {
    }
      
    CrystalCalibration(float m, float s, float tc, std::vector<TimingEvent> te) :
      mean(m),
      sigma(s),
      totalChi2(tc)
    {
      timingEvents = te;
    }

    CrystalCalibration(float m, float s, float tc, std::vector<TimingEvent> te, bool wm) :
      mean(m),
      sigma(s),
      totalChi2(tc),
      useWeightedMean(wm)
    {
      timingEvents = te;
    }

    bool insertEvent(float amp, float t, float sigmaT, bool ee)
    {
      if(sigmaT > 0) // throw away events with zero or negative errors
      {
        timingEvents.push_back(TimingEvent(amp,t,sigmaT,ee));
        updateChi2();
        return true;
      }
      else
        return false;
    }

    bool insertEvent(TimingEvent te)
    {
      if(te.sigmaTime > 0)
      {
        timingEvents.push_back(te);
        updateChi2();
        return true;
      }
      else
        return false;
    }

    int filterOutliers(float threshold = 0.5)
    {
      int numPointsErased = 0;
      while(timingEvents.size() > 4)
      {
        updateChi2();
        float oldMean = mean;
        // Erase largest chi2 event
        TimingEvent toRemove = *maxChi2Itr;
        timingEvents.erase(maxChi2Itr);
        //Calculate new mean/error
        updateChi2();
        //Compare to old mean and break if |(newMean-oldMean)| < newSigma
        //TODO: study acceptance threshold
        if(fabs(mean-oldMean) < threshold*sigma)
        {
          insertEvent(toRemove);
          break;
        }
        else
        {
          numPointsErased++;
        }
      }
      return numPointsErased;
    }

  private:
    bool useWeightedMean;

    void updateChi2() // update individual, total, maxChi2s
    {
      if(useWeightedMean)
        updateChi2Weighted();
      else
        updateChi2Unweighted();
    }

    void updateChi2Weighted()
    {
      updateMeanWeighted();
      float chi2 = 0;
      maxChi2Itr = timingEvents.begin();
      for(std::vector<TimingEvent>::iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        float singleChi = (itr->time-mean)/itr->sigmaTime;
        itr->chi2 = singleChi*singleChi;
        chi2+=singleChi*singleChi;
        if(itr->chi2 > maxChi2Itr->chi2)
          maxChi2Itr = itr;
      }
      totalChi2 = chi2;
    }

    void updateChi2Unweighted()
    {
      updateMeanUnweighted();
      float chi2 = 0;
      maxChi2Itr = timingEvents.begin();
      for(std::vector<TimingEvent>::iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        float singleChi = (itr->time-mean);
        itr->chi2 = singleChi*singleChi;
        chi2+=singleChi*singleChi;
        if(itr->chi2 > maxChi2Itr->chi2)
          maxChi2Itr = itr;
      }
      totalChi2 = chi2;
    }

    void updateMeanWeighted()
    {
      float meanTmp = 0;
      float sigmaTmp = 0;
      for(std::vector<TimingEvent>::const_iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        float sigmaT2 = itr->sigmaTime;
        sigmaT2*=sigmaT2;
        meanTmp+=(itr->time)/(sigmaT2);
        sigmaTmp+=1/(sigmaT2);
      }
      mean = meanTmp/sigmaTmp;
      sigma = sqrt(1/sigmaTmp);
    }

    void updateMeanUnweighted()
    {
      float meanTmp = 0;
      for(std::vector<TimingEvent>::const_iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        meanTmp+=itr->time;
      }
      mean = meanTmp/timingEvents.size();
      float sigmaTmp = 0;
      for(std::vector<TimingEvent>::const_iterator itr = timingEvents.begin();
          itr != timingEvents.end(); ++itr)
      {
        sigmaTmp+=(itr->time-mean)*(itr->time-mean);
      }
      sigma = sqrt(sigmaTmp/timingEvents.size());
    }
    
};


