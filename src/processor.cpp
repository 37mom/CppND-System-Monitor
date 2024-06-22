#include <vector>
#include <string>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// TODO-Done: Return the aggregate CPU utilization
float Processor::Utilization() {
	float cpuUsage;
	vector<string> cpuData;
	long Idle, NonIdle, Total, TotalD, IdleD = 0;

	//Will calculate cpu usage for the system as method mention in link below
	//https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
	cpuData = LinuxParser::CpuUtilization();

	Idle = stol(cpuData[LinuxParser::kIdle_])
			+ stol(cpuData[LinuxParser::kIOwait_]);

	NonIdle = stol(cpuData[LinuxParser::kUser_])
			+ stol(cpuData[LinuxParser::kNice_])
			+ stol(cpuData[LinuxParser::kSystem_])
			+ stol(cpuData[LinuxParser::kIRQ_])
			+ stol(cpuData[LinuxParser::kSoftIRQ_])
			+ stol(cpuData[LinuxParser::kSteal_])
			+ stol(cpuData[LinuxParser::kGuest_])
			+ stol(cpuData[LinuxParser::kGuestNice_]);

	PrevTotal_ = PrevIdle_ + PrevNonIdle_;
	Total = Idle + NonIdle;

	TotalD = Total - PrevTotal_;
	IdleD = Idle - PrevIdle_;

	cpuUsage = ((TotalD - IdleD) / static_cast<float>(TotalD));

	return cpuUsage;
}
