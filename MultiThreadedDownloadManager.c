#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#define FILE_SIZE 10000000 // 10 MB "file"
#define NUM_THREADS 4

// --- Global Shared Data ---
long total_bytes_downloaded = 0;
#ifdef _WIN32
CRITICAL_SECTION lock;
#else
pthread_mutex_t lock;
#endif

// --- Thread Data Structure ---
typedef struct {
    int thread_id;
    long chunk_size;
} ThreadData;

// --- Function Prototypes ---
#ifdef _WIN32
DWORD WINAPI download_chunk(LPVOID arg);
#else
void *download_chunk(void *arg);
#endif
void print_progress_bar();

// --- Main Function ---
int main() {
#ifdef _WIN32
    HANDLE threads[NUM_THREADS];
#else
    pthread_t threads[NUM_THREADS];
#endif
    ThreadData thread_data[NUM_THREADS];
    long chunk_size = FILE_SIZE / NUM_THREADS;

#ifdef _WIN32
    InitializeCriticalSection(&lock);
#else
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }
#endif

    printf("Simulating download of %d bytes with %d threads...\n", FILE_SIZE, NUM_THREADS);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].chunk_size = (i == NUM_THREADS - 1) ? (FILE_SIZE - (chunk_size * (NUM_THREADS - 1))) : chunk_size;
#ifdef _WIN32
        threads[i] = CreateThread(NULL, 0, download_chunk, &thread_data[i], 0, NULL);
#else
        pthread_create(&threads[i], NULL, download_chunk, &thread_data[i]);
#endif
    }

    // Print progress bar while threads are running
    while (total_bytes_downloaded < FILE_SIZE) {
        print_progress_bar();
#ifdef _WIN32
        Sleep(100); // Update progress bar every 100ms
#else
        usleep(100000); // Update progress bar every 100ms
#endif
    }
    print_progress_bar(); // Final print
    printf("\nDownload complete!\n");
#ifdef _WIN32
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);
    for(int i=0; i<NUM_THREADS; i++) {
        CloseHandle(threads[i]);
    }
#else
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
#endif

#ifdef _WIN32
    DeleteCriticalSection(&lock);
#else
    pthread_mutex_destroy(&lock);
#endif
    return 0;
}

// --- Core Logic ---

#ifdef _WIN32
DWORD WINAPI download_chunk(LPVOID arg) {
#else
void *download_chunk(void *arg) {
#endif
    ThreadData *data = (ThreadData *)arg;
    long downloaded_in_thread = 0;
    long bytes_to_download = data->chunk_size;
    long part = bytes_to_download / 10; // Simulate in 10 parts

    while (downloaded_in_thread < bytes_to_download) {
#ifdef _WIN32
        Sleep(rand() % 500); // Simulate network latency
#else
        usleep((rand() % 500) * 1000); // Simulate network latency
#endif
        long current_download = part;
        if (downloaded_in_thread + current_download > bytes_to_download) {
            current_download = bytes_to_download - downloaded_in_thread;
        }

#ifdef _WIN32
        EnterCriticalSection(&lock);
#else
        pthread_mutex_lock(&lock);
#endif
        total_bytes_downloaded += current_download;
#ifdef _WIN32
        LeaveCriticalSection(&lock);
#else
        pthread_mutex_unlock(&lock);
#endif

        downloaded_in_thread += current_download;
    }
    return 0; // Return 0 for DWORD, NULL for void* is effectively 0
}

void print_progress_bar() {
    int bar_width = 50;
    float progress = (float)total_bytes_downloaded / FILE_SIZE;
    int pos = bar_width * progress;

    printf("\r["); // Carriage return to start of line
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %.2f%%", progress * 100.0);
    fflush(stdout);
}
