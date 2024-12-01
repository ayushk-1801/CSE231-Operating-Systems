#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROTATIONAL_SPEED_RPM 72  // Rotational speed in RPM
#define SECTORS_PER_TRACK 100    // Number of sectors per track
#define SEEK_TIME_PER_TRACK 2    // Seek time in ms per track
#define INITIAL_HEAD_POSITION 100

// Function to calculate rotational latency
double calculate_rotational_latency(int start_sector, int target_sector) {
    double time_per_sector = (60.0 / ROTATIONAL_SPEED_RPM) / SECTORS_PER_TRACK; // Time for one sector in seconds
    int sector_difference = abs(target_sector - start_sector);
    return sector_difference * time_per_sector * 1000; // Convert to ms
}

// Function to find the closest request to the current head position
int find_closest_request(int current, int requests[], int visited[], int size) {
    int closest = -1;
    int min_distance = 1e9; // Large number

    for (int i = 0; i < size; i++) {
        if (!visited[i]) {
            int distance = abs(requests[i] - current);
            if (distance < min_distance) {
                min_distance = distance;
                closest = i;
            }
        }
    }

    return closest;
}

int main() {
    int requests[] = {78, 289, 21, 495};
    int size = sizeof(requests) / sizeof(requests[0]);
    int visited[size];
    for (int i = 0; i < size; i++) visited[i] = 0; // Initialize visited array

    int current_position = INITIAL_HEAD_POSITION;
    int current_track = current_position / SECTORS_PER_TRACK; 
    int total_seek_time = 0;
    double total_rotational_latency = 0.0;

    printf("Disk Scheduling Results:\n");

    for (int i = 0; i < size; i++) {
        int next_request = find_closest_request(current_position, requests, visited, size);
        visited[next_request] = 1;

        int next_position = requests[next_request];
        int next_track = next_position / SECTORS_PER_TRACK;

        // Calculate seek time
        int track_difference = abs(next_track - current_track);
        int seek_time = track_difference * SEEK_TIME_PER_TRACK;
        total_seek_time += seek_time;

        // Calculate rotational latency
        double rotational_latency = calculate_rotational_latency(current_position % SECTORS_PER_TRACK, next_position % SECTORS_PER_TRACK);
        total_rotational_latency += rotational_latency;

        // Output results
        printf("Request %d:\n", i + 1);
        printf("  Seek Time: %d ms\n", seek_time);
        printf("  Rotational Latency: %.2f ms\n", rotational_latency);

        // Update current position
        current_position = next_position;
        current_track = next_track;
    }

    printf("\nTotal Seek Time: %d ms\n", total_seek_time);
    printf("Total Rotational Latency: %.2f ms\n", total_rotational_latency);

    return 0;
}
