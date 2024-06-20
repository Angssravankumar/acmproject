#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct Process {
    int id;
    int arrival;
    int burst;
};

bool shortestJob(const Process &a, const Process &b) {
    return a.burst < b.burst;
}

int main() {
    std::ifstream inFile("input.txt");
    std::ofstream outFile("output.txt");
    int numJobs;
    inFile >> numJobs;
    std::vector<Process> jobs(numJobs);
    for (int i = 0; i < numJobs; ++i) {
        inFile >> jobs[i].id >> jobs[i].arrival >> jobs[i].burst;
    }

    std::sort(jobs.begin(), jobs.end(), shortestJob);

    std::vector<int> waitTimes(numJobs), turnaroundTimes(numJobs), responseTimes(numJobs, -1);
    int currentTime = 0;
    int completedJobs = 0;

    while (completedJobs < numJobs) {
        int shortestJobIndex = -1;
        int minBurst = INT_MAX;
        for (int i = 0; i < numJobs; ++i) {
            if (jobs[i].arrival <= currentTime && jobs[i].burst < minBurst && jobs[i].burst > 0) {
                minBurst = jobs[i].burst;
                shortestJobIndex = i;
            }
        }

        if (shortestJobIndex != -1) {
            if (responseTimes[shortestJobIndex] == -1) {
                responseTimes[shortestJobIndex] = currentTime - jobs[shortestJobIndex].arrival;
            }
            currentTime += jobs[shortestJobIndex].burst;
            turnaroundTimes[shortestJobIndex] = currentTime - jobs[shortestJobIndex].arrival;
            waitTimes[shortestJobIndex] = turnaroundTimes[shortestJobIndex] - jobs[shortestJobIndex].burst;
            jobs[shortestJobIndex].burst = 0;
            completedJobs++;
        } else {
            currentTime++;
        }
    }

    double avgWaitTime = 0, avgTurnaroundTime = 0, avgResponseTime = 0;
    for (int i = 0; i < numJobs; ++i) {
        avgWaitTime += waitTimes[i];
        avgTurnaroundTime += turnaroundTimes[i];
        avgResponseTime += responseTimes[i];
    }
    avgWaitTime /= numJobs;
    avgTurnaroundTime /= numJobs;
    avgResponseTime /= numJobs;

    outFile << avgWaitTime << std::endl;
    outFile << avgTurnaroundTime << std::endl;
    outFile << avgResponseTime << std::endl;

    return 0;
}
