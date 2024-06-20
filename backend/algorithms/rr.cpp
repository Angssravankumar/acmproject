#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

struct Task {
    int id;
    int arrivalTime;
    int burstTime;
};

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");
    int numTasks, quantum;
    inputFile >> numTasks;
    std::vector<Task> tasks(numTasks);
    for (int i = 0; i < numTasks; ++i) {
        inputFile >> tasks[i].id >> tasks[i].arrivalTime >> tasks[i].burstTime;
    }
    inputFile >> quantum;

    std::queue<Task> taskQueue;
    std::vector<int> waitTimes(numTasks), turnaroundTimes(numTasks), responseTimes(numTasks, -1);
    int currentTime = 0, remainingTasks = numTasks;
    int idx = 0;

    while (remainingTasks > 0) {
        while (idx < numTasks && tasks[idx].arrivalTime <= currentTime) {
            taskQueue.push(tasks[idx]);
            idx++;
        }

        if (!taskQueue.empty()) {
            Task currentTask = taskQueue.front();
            taskQueue.pop();
            if (responseTimes[currentTask.id] == -1) {
                responseTimes[currentTask.id] = currentTime - currentTask.arrivalTime;
            }
            if (currentTask.burstTime <= quantum) {
                currentTime += currentTask.burstTime;
                turnaroundTimes[currentTask.id] = currentTime - currentTask.arrivalTime;
                waitTimes[currentTask.id] = turnaroundTimes[currentTask.id] - currentTask.burstTime;
                remainingTasks--;
            } else {
                currentTime += quantum;
                currentTask.burstTime -= quantum;
                while (idx < numTasks && tasks[idx].arrivalTime <= currentTime) {
                    taskQueue.push(tasks[idx]);
                    idx++;
                }
                taskQueue.push(currentTask);
            }
        } else {
            currentTime++;
        }
    }

    double avgWaitTime = 0, avgTurnaroundTime = 0, avgResponseTime = 0;
    for (int i = 0; i < numTasks; ++i) {
        avgWaitTime += waitTimes[i];
        avgTurnaroundTime += turnaroundTimes[i];
        avgResponseTime += responseTimes[i];
    }
    avgWaitTime /= numTasks;
    avgTurnaroundTime /= numTasks;
    avgResponseTime /= numTasks;

    outputFile << avgWaitTime << std::endl;
    outputFile << avgTurnaroundTime << std::endl;
    outputFile << avgResponseTime << std::endl;

    return 0;
}
