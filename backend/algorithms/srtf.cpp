#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
};

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");
    int numProcesses;
    inputFile >> numProcesses;
    std::vector<Process> processes(numProcesses);
    for (int i = 0; i < numProcesses; ++i) {
        inputFile >> processes[i].id >> processes[i].arrivalTime >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }

    std::vector<int> waitingTimes(numProcesses), turnaroundTimes(numProcesses), responseTimes(numProcesses, -1);
    int currentTime = 0, completed = 0;

    while (completed < numProcesses) {
        int shortestProcessIdx = -1, minRemainingTime = INT_MAX;
        for (int i = 0; i < numProcesses; ++i) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime < minRemainingTime && processes[i].remainingTime > 0) {
                minRemainingTime = processes[i].remainingTime;
                shortestProcessIdx = i;
            }
        }

        if (shortestProcessIdx != -1) {
            if (responseTimes[shortestProcessIdx] == -1) {
                responseTimes[shortestProcessIdx] = currentTime - processes[shortestProcessIdx].arrivalTime;
            }
            currentTime++;
            processes[shortestProcessIdx].remainingTime--;
            if (processes[shortestProcessIdx].remainingTime == 0) {
                completed++;
                waitingTimes[shortestProcessIdx] = currentTime - processes[shortestProcessIdx].arrivalTime - processes[shortestProcessIdx].burstTime;
                turnaroundTimes[shortestProcessIdx] = currentTime - processes[shortestProcessIdx].arrivalTime;
            }
        } else {
            currentTime++;
        }
    }

    double avgWaitingTime = 0, avgTurnaroundTime = 0, avgResponseTime = 0;
    for (int i = 0; i < numProcesses; ++i) {
        avgWaitingTime += waitingTimes[i];
        avgTurnaroundTime += turnaroundTimes[i];
        avgResponseTime += responseTimes[i];
    }
    avgWaitingTime /= numProcesses;
    avgTurnaroundTime /= numProcesses;
    avgResponseTime /= numProcesses;

    outputFile << avgWaitingTime << std::endl;
    outputFile << avgTurnaroundTime << std::endl;
    outputFile << avgResponseTime << std::endl;

    return 0;
}
