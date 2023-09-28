
#include "bw_lat_mem_ctrl.h"

BwLatMemCtrl::BwLatMemCtrl(string _curveAddress, uint32_t _curveWindowSize, double _MaxTheoreticalBW, uint32_t _domain, const g_string &_name) : domain(_domain)
{
	name = _name;
	curveAddress = _curveAddress;
	curveWindowSize = _curveWindowSize;
	// change the bandwidth to access per cycles
	MaxTheoreticalBW =  (_MaxTheoreticalBW  / 64) / 2.1;

	leadOffLatency = 100000;
	maxBandwidth = 0;
	maxLatency = 0;
	// curves_data
	for (uint32_t i=0; i<101; i+=2) {
		string fileAddress;
		ifstream curveFiles;
		double inputBandwidth, inputLatency; 

		// generate the address of the files
		if (i<10)
			fileAddress = curveAddress + "/bwlat_" + '0' + to_string(i) + ".txt";
		else
			fileAddress = curveAddress + "/bwlat_" + to_string(i) + ".txt";
		
		// cout << fileAddress << endl;

		curveFiles.open(fileAddress, std::ifstream::in);

		// create read the curve file into the curves_date
		// also set leadOffLatency, maxBandwidth, maxLatency
		


		double maxBandwidthTemp = 0;
		double maxLatencyTemp = 0;

		vector<vector<double>> curve_data;
		while(curveFiles >> inputBandwidth >> inputLatency) {
			vector<double> dataPoint;
			// change the bandwidth to access per cycles
			// this is hard coded for skylake it should read the config frecuency
			inputBandwidth = (inputBandwidth / 64) / (2.1*1000)  ;
			// decrease LLC latency; this is hard coded for skylake. it should read all parent 
			// cache latency and sum them at the end. for Skylake this value is 37 (L3) + 10 (L2) + 4 (L1) = 51
			inputLatency = inputLatency - 51;


			dataPoint.push_back(inputBandwidth);
			dataPoint.push_back(inputLatency);			
			// cout << dataPoint[0] << " " << dataPoint[1] << endl;

			if (leadOffLatency > inputLatency)
				leadOffLatency = inputLatency;
			if (maxLatency < inputLatency)
				maxLatency = inputLatency;
			if (maxLatencyTemp < inputLatency)
				maxLatencyTemp = inputLatency;
			if (maxBandwidth < inputBandwidth)
				maxBandwidth = inputBandwidth;
			if (maxBandwidthTemp < inputBandwidth)
				maxBandwidthTemp = inputBandwidth;


			curve_data.push_back(dataPoint);
		}
		maxBandwidthPerRdRatio.push_back(maxBandwidthTemp);
		maxLatencyPerRdRatio.push_back(maxLatencyTemp); 
		curves_data.push_back(curve_data);
		curveFiles.close();
	}

	cout << "leadOffLatency: " << (leadOffLatency + 51) / 2.1 << " ns, cycles: " << (leadOffLatency + 51) << endl;
	cout << "maxLatency: " << (maxLatency + 51) / 2.1 << " ns"<< endl;
	cout << "maxBandwidth: " << maxBandwidth * 64  * 2.1 << " GB/s" << endl;
	cout << "MaxTheoreticalBW: " << MaxTheoreticalBW * 64 * 2.1 << " GB/s" << endl;

	// test to see if the data is loaded correctly.
	// for (uint32_t i=0; i< curves_data.size(); i++)
	// {
	// 	cout << "read percentage is: " << i*2 << endl;
	// 	for (uint32_t j = 0; j < curves_data[i].size(); ++j)
	// 	{
	// 		cout << curves_data[i][j][0] << " " << curves_data[i][j][1] << endl;
	// 	}
	// }

	cout << "maxBandwidth: " << endl;
	for (uint32_t j = 0; j < maxBandwidthPerRdRatio.size(); ++j) {
		cout << maxBandwidthPerRdRatio[j] << endl;
	}

	cout << "maxlatency: " << endl;
	for (uint32_t j = 0; j < maxLatencyPerRdRatio.size(); ++j) {
		cout << maxLatencyPerRdRatio[j] << endl;
	}

	// initialization of the counters
	currentWindowAccessCount = 0;
	currentWindowAccessCount_wr = 0;
	currentWindowAccessCount_rd = 0;

	// initialize the state
	lastEstimatedBandwidth = 0;
	lastEstimatedLatency = leadOffLatency;
	latency = (uint32_t) lastEstimatedLatency;


	futex_init(&updateLock);
}

double round(double d) {
	return std::floor(d + 0.5);
}

uint32_t BwLatMemCtrl::searchForLatencyOnCurve(double bandwidth, double readPercentage) {

	// curves_data
	// first select the correct read to write ratio. an even number between 0 and 100
	uint32_t intReadPercentage;
	intReadPercentage = round( (100*readPercentage) * 0.5f ) * 2;
	assert(intReadPercentage >= 0);
	assert(intReadPercentage <= 100);
	assert(intReadPercentage % 2 == 0);

	uint32_t curveDataIndex = intReadPercentage/2;


	uint32_t finalLatency=0;
	double finalBW=0;

	// TODO...
	// sanity check 
	cout << endl << "new" << endl;
	cout << "MaxTheoreticalBW: " << MaxTheoreticalBW  << " acesses/cycles" << endl;
	cout << "maxBandwidth in all the curves : " << maxBandwidth  << " acesses/cycles" << endl;
	cout << "measured bandwidth: " << bandwidth << " max from curve: "<< maxBandwidthPerRdRatio[curveDataIndex] << endl;
	cout << "rd percentage: " << intReadPercentage << endl;


	bool overflow=false;
	// if (MaxTheoreticalBW < bandwidth) {
	if (maxBandwidth < bandwidth) {
		
		// bandwidth =  maxBandwidthPerRdRatio[curveDataIndex];
		// cout << "more than max! bandwidth: " << bandwidth << " max from curve: "<< maxBandwidthPerRdRatio[curveDataIndex] << endl;
		overflow=true;
		// let's converge slowly
		finalBW = 0.5f * maxBandwidthPerRdRatio[curveDataIndex] + 0.5f * lastEstimatedBandwidth;
		// simulate the wave form at least for a very high bandwidth
		// finalLatency = curves_data[curveDataIndex][0][1];
		finalLatency = maxLatencyPerRdRatio[curveDataIndex];
		lastEstimatedBandwidth = finalBW;

		cout << "finalLatency " << finalLatency << endl;
		finalLatency = finalLatency + 0.1*(double)(finalLatency);
		cout << "overflow..." << endl;
		cout << "lastEstimatedBandwidth: " << lastEstimatedBandwidth << endl;
		cout << "finalLatency (updated)" << finalLatency << endl;

		// put more assert
		assert(finalLatency>=leadOffLatency);

		return finalLatency;

		// cout << "more than max! bandwidth: " << bandwidth << " max from curve: "<< maxBandwidthPerRdRatio[curveDataIndex] << endl;
	}


	// use j later for extrapolation
	uint32_t j;
	for (j = 0; j < curves_data[curveDataIndex].size(); ++j)
	{

		// cout << "finalLatency " << finalLatency << " finalBW: " << finalBW << endl;
		// cout << "curves_data[curveDataIndex][j][0]: " << curves_data[curveDataIndex][j][0] << " bandwidth " << bandwidth << endl;
		
		if (finalBW == 0) {
			finalBW = curves_data[curveDataIndex][j][0];
			finalLatency = curves_data[curveDataIndex][j][1];
		}

		if (curves_data[curveDataIndex][j][0] >= bandwidth) {
			finalBW = curves_data[curveDataIndex][j][0];
			finalLatency = curves_data[curveDataIndex][j][1];
		}
		else {
			break;
		}

		// cout << curves_data[curveDataIndex][j][0] << " " << curves_data[curveDataIndex][j][1] << endl;
	}
	if (j!=0) {
		// for easier readability
		double x1 = curves_data[curveDataIndex][j][0];
		double y1 = curves_data[curveDataIndex][j][1];
		double x2 = curves_data[curveDataIndex][j-1][0];
		double y2 = curves_data[curveDataIndex][j-1][1];
		double x = bandwidth;

		finalLatency = y1 + ((x - x1) / (x2 - x1)) * (y2 - y1);
	}
	
	lastEstimatedBandwidth = finalBW;

	cout << "lastEstimatedBandwidth: " << lastEstimatedBandwidth << endl;
	cout << "finalLatency " << finalLatency << endl;

	assert(finalLatency>=leadOffLatency);
	// cout << finalLatency << " " << finalBW << endl;


	return finalLatency;


}

void BwLatMemCtrl::updateLatency(uint64_t currentWindowEndCyclen) {
	// bandwidth unit is accesses per cycles
	
	double bandwidth = ((double)currentWindowAccessCount) / (double)(currentWindowEndCyclen - currentWindowStartCycle);
	double readPercentage = ((double)(currentWindowAccessCount_rd))/ (currentWindowAccessCount_rd+currentWindowAccessCount_wr);


	lastEstimatedLatency = searchForLatencyOnCurve(bandwidth, readPercentage);
	assert(lastEstimatedLatency>=0);
	assert(lastEstimatedLatency<=maxLatency);

	latency = (uint32_t)lastEstimatedLatency;
	// cout << "latency: " << latency << " cycles. readPercentage: " << readPercentage << endl;

	__sync_synchronize();
}

uint64_t BwLatMemCtrl::access(MemReq &req) {
	
	bool isWrite;
	if(req.type == PUTS) {
		// clean write should not go to memory
		*req.state = I;
		return req.cycle + 1;
	}



	// // update the latency here by counting the number of accesses or cycles.
    futex_lock(&updateLock);

    if (currentWindowAccessCount==0) {
    	currentWindowStartCycle = req.cycle;
    }

    currentWindowAccessCount++;
    if (req.type == PUTX) {
    	currentWindowAccessCount_wr++;
    	isWrite = true;
    }
    else {
    	currentWindowAccessCount_rd++;
    	isWrite = false;
    }

    
    if(currentWindowAccessCount==curveWindowSize) {
    	updateLatency(req.cycle);
    	currentWindowAccessCount = 0; // reset the access as we enter a new window
    	currentWindowAccessCount_wr = 0; // reset the access as we enter a new window
    	currentWindowAccessCount_rd = 0; // reset the access as we enter a new window
    }
    futex_unlock(&updateLock);


	switch (req.type) {
        case PUTS:
        case PUTX:
            *req.state = I;
            break;
        case GETS:
            *req.state = req.is(MemReq::NOEXCL)? S : E;
            break;
        case GETX:
            *req.state = M;
            break;

        default: panic("!?");
    }

    uint64_t respCycle = req.cycle + (uint64_t)latency;
    // uint64_t respCycle = req.cycle + (uint64_t)leadOffLatency;
    if (isWrite)
    {
    	profWrites.inc();
    	profTotalWrLat.inc(latency);
    }
    else {
    	profReads.inc();
    	profTotalRdLat.inc(latency);
    }
    assert(respCycle > req.cycle);

    return respCycle;
}

void BwLatMemCtrl::initStats(AggregateStat *parentStat) {
	AggregateStat* memStats = new AggregateStat();
    memStats->init(name.c_str(), "Memory controller stats");
    profReads.init("rd", "Read requests"); memStats->append(&profReads);
    profWrites.init("wr", "Write requests"); memStats->append(&profWrites);
    profTotalRdLat.init("rdlat", "Total latency experienced by read requests"); memStats->append(&profTotalRdLat);
    profTotalWrLat.init("wrlat", "Total latency experienced by write requests"); memStats->append(&profTotalWrLat);
    // profLoad.init("load", "Sum of load factors (0-100) per update"); memStats->append(&profLoad);
    // profUpdates.init("ups", "Number of latency updates"); memStats->append(&profUpdates);
    // profClampedLoads.init("clampedLoads", "Number of updates where the load was clamped to 95%"); memStats->append(&profClampedLoads);
    parentStat->append(memStats);

}

