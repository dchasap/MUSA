
#ifndef BW_LAT_MEM_CTRL_H_
#define BW_LAT_MEM_CTRL_H_

#include <fstream>
#include <cmath> // for floor function on implement round

#include "timing_event.h"
#include "zsim.h"
#include "memory_hierarchy.h"
#include "g_std/g_string.h"
#include "pad.h"
#include "stats.h"

using namespace std;


class BwLatMemCtrl : public MemObject {
	private:
		
		const uint32_t domain;
		uint32_t latency;

		double leadOffLatency, maxBandwidth, maxLatency;
		double MaxTheoreticalBW;
		double lastEstimatedBandwidth;
		double lastEstimatedLatency;

		// factor of latency over max latency when the bandwidth is maxed out.
		const double overflowFactor = 1.2;
		// fisrt dimention: read percentafe
		// second dimention data for each read percentage
		// second dimention has two value 
		// [0]: bandwidth (accesses/cycles) 
		// [1]: latency (cycles)
		vector<vector<vector<double>>> curves_data;
		vector<double> maxBandwidthPerRdRatio;
		vector<uint32_t> maxLatencyPerRdRatio;

		string curveAddress;
		uint32_t currentWindowAccessCount;
		uint32_t currentWindowAccessCount_wr;
		uint32_t currentWindowAccessCount_rd;
		
		uint32_t curveWindowSize;
		
		uint64_t currentWindowStartCycle;


		PAD();

        Counter profReads;
        Counter profWrites;
        Counter profTotalRdLat;
        Counter profTotalWrLat;


        g_string name; //barely used
        lock_t updateLock;
        PAD();

        void updateLatency(uint64_t currentWindowEndCyclen);
        uint32_t searchForLatencyOnCurve(double bandwidth, double readPercentage);


	public:
		BwLatMemCtrl(string curveAddress, uint32_t curveWindowSize, double MaxTheoreticalBW, uint32_t _domain, const g_string &_name);
		void initStats(AggregateStat *parentStat);
		uint64_t access(MemReq &req);
		const char *getName() { return name.c_str(); }
		
};




#endif  // BW_LAT_MEM_CTRL_H_
