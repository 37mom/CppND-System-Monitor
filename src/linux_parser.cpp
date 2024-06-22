#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
	string line;
	string key;
	string value;
	std::ifstream filestream(kOSPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ' ', '_');
			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "PRETTY_NAME") {
					std::replace(value.begin(), value.end(), '_', ' ');
					return value;
				}
			}
		}
	}
	return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
	string os, kernel, version;
	string line;
	std::ifstream stream(kProcDirectory + kVersionFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> os >> version >> kernel;
	}
	return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
	vector<int> pids;
	DIR *directory = opendir(kProcDirectory.c_str());
	struct dirent *file;
	while ((file = readdir(directory)) != nullptr) {
		// Is this a directory?
		if (file->d_type == DT_DIR) {
			// Is every character of the name a digit?
			string filename(file->d_name);
			if (std::all_of(filename.begin(), filename.end(), isdigit)) {
				int pid = stoi(filename);
				pids.push_back(pid);
			}
		}
	}
	closedir(directory);
	return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {

	string line;
	string key;
	float value;
	float fMemTotal;
	float fMemFree;
	std::ifstream filestream(kProcDirectory + kMeminfoFilename);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::replace(line.begin(), line.end(), 'k', ' ');
			std::replace(line.begin(), line.end(), 'B', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "MemTotal") {
					//std::replace(value.begin(), value.end(), '_', ' ');
					fMemTotal = value;
				}
				if (key == "MemFree") {
					//std::replace(value.begin(), value.end(), '_', ' ');
					fMemFree = value;

				}
			}
		}
	}
	return (fMemTotal - fMemFree) / fMemTotal;

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {

	string line;
	long Uptime;
	long IdleTime;
	std::ifstream stream(kProcDirectory + kUptimeFilename);

	if (stream.is_open()) {
		while (std::getline(stream, line)) {

			std::istringstream linestream(line);
			while (linestream >> Uptime >> IdleTime) {

			}
		}
	}

	return Uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
	return (UpTime() * sysconf(_SC_CLK_TCK));
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) {
	return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
	return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
	return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {  
  string line;
  string key,value;
  vector<string> cpuData;
   
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key;
        if (key == "cpu") {
          //Store in cpu vector the data got from in 10 columns in cpu line
          for(int i=0 ; i<10 ; i++)
          {
            linestream>>value;
            cpuData.push_back(value);
          }
        }
  }
  return cpuData;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {

	string key, line;
	int value, returnValue;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "processes") {
					returnValue = value;
				}
			}
		}
	}
	return returnValue;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {

	string key, line;
	int value, returnValue;
	std::ifstream filestream(kProcDirectory + kStatFilename);
	if (filestream.is_open()) {
		while (getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "procs_running") {
					returnValue = value;
				}
			}
		}
	}
	return returnValue;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {

	string command;
	std::ifstream filestream(
			kProcDirectory + to_string(pid) + kCmdlineFilename);
	if (filestream.is_open()) {
		getline(filestream, command);
	}
	return command;
}
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
	string key, line, returnValue;
	int value;
	std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
	if (filestream.is_open()) {
		while (getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "VmSize:") {
					returnValue = to_string(value / 1000);
				}
			}
		}
	}
	return returnValue;

}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
	string key, line;
	int value, retValue;
	std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
	if (filestream.is_open()) {
		while (getline(filestream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "Uid:") {
					retValue = value;
				}
			}
		}
	}
	return to_string(retValue);

}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
	string line;
	string name, password, UID, retUser;

	std::ifstream filestream(kPasswordPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ':', ' ');
			std::istringstream linestream(line);
			while (linestream >> name >> password >> UID) {
				if (UID == LinuxParser::Uid(pid)) {
					retUser = name;
				}
			}
		}
	}

	return retUser;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  string key,value;
  long Proc_stime;
  vector<string>time;
   
  std::ifstream filestream(kProcDirectory + "/" +to_string(pid)+kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      // Get element number 22 in order to calculate uptime for a process
          for(int i=0 ; i< 22; i++)
          {
            linestream>>value;
            time.push_back(value);
          }
          //devide uptime for the system - uptime for a process / Herz to get uptime in seconds
          Proc_stime = LinuxParser::UpTime()- (stol(time[21]) / sysconf(_SC_CLK_TCK)); 
          
  }
  return Proc_stime;
  }

