#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() {
	return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
	string line;
	string key, value;
	vector < string > stat;
	float Proc_cpu, seconds;
	long total_time;

// In order to calculate cpu usage for each process we will use method mentioned in link below:
//https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599

	std::ifstream filestream(
			LinuxParser::kProcDirectory + to_string(pid_)
					+ LinuxParser::kStatFilename);
	if (filestream.is_open()) {
		std::getline(filestream, line);
		std::istringstream linestream(line);
		// we need to parse the first 22 element in the file
		for (int i = 0; i < 22; i++) {
			linestream >> value;
			stat.push_back(value);
		}
		//total_time = utime + stime
		total_time = stol(stat[13]) + stol(stat[14]);
		//total_time = total_time + cutime+ cstime
		total_time = total_time + stol(stat[15]) + stol(stat[17]);
		//seconds = uptime - (starttime / Hertz)
		seconds = LinuxParser::UpTime()
				- (stol(stat[21]) / sysconf(_SC_CLK_TCK));
		//cpu usage = (total_time / Hertz) / seconds
		Proc_cpu = ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
	}
	return Proc_cpu;
}

// TODO: Return the command that generated this process
string Process::Command() {
	return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
	return LinuxParser::Ram(pid_);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
	return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
	return LinuxParser::UpTime(pid_);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
	return (a.CpuUtilization() < CpuUtilization());
}
