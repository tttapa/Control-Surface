#include <MIDI_Interfaces/USBMIDI/LowLevel/BulkTX.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <random>
#include <thread>
#include <vector>

namespace {

constexpr uint16_t PacketSize = 64;

struct TestBulkTX : cs::BulkTX<TestBulkTX, uint32_t, PacketSize> {
    void start_timeout();
    void cancel_timeout();
    void tx_start(const void *data, uint32_t size);
    void tx_start_timeout(const void *data, uint32_t size) {
        tx_start(data, size);
    }
    void tx_start_isr(const void *data, uint32_t size) { tx_start(data, size); }

    timer_t timeout_timerid {}, write_timerid {};
    itimerspec timeout_its {}, write_its {};

    std::vector<uint32_t> data_challenge;
    std::vector<uint32_t> data_response;

    ~TestBulkTX();
    TestBulkTX();
    TestBulkTX(const TestBulkTX &) = delete;
    const TestBulkTX &operator=(const TestBulkTX &) = delete;

    friend void timeout_handler(int, siginfo_t *, void *context);
    friend void write_handler(int, siginfo_t *, void *context);
};

void timeout_handler(int, siginfo_t *info, void *) {
    auto *self = reinterpret_cast<TestBulkTX *>(info->si_value.sival_ptr);
    self->timeout_callback();
}

void write_handler(int, siginfo_t *info, void *) {
    auto *self = reinterpret_cast<TestBulkTX *>(info->si_value.sival_ptr);
    self->tx_callback();
}

TestBulkTX::TestBulkTX() {
    // Set up the signal handler
    struct sigaction timeout_sa {};
    timeout_sa.sa_flags = SA_SIGINFO;
    timeout_sa.sa_sigaction = timeout_handler;
    sigemptyset(&timeout_sa.sa_mask);
    sigaction(SIGRTMIN, &timeout_sa, NULL);
    // Set up the timer
    struct sigevent timeout_sev {};
    timeout_sev.sigev_notify = SIGEV_SIGNAL;
    timeout_sev.sigev_signo = SIGRTMIN;
    timeout_sev.sigev_value.sival_ptr = this;
    timer_create(CLOCK_REALTIME, &timeout_sev, &timeout_timerid);
    // Set the timer expiration time
    timeout_its.it_value.tv_sec = 0;
    timeout_its.it_value.tv_nsec = 0;
    // Set the timer interval to zero for a one-shot timer
    timeout_its.it_interval.tv_sec = 0;
    timeout_its.it_interval.tv_nsec = 0;

    // Set up the signal handler
    struct sigaction write_sa {};
    write_sa.sa_flags = SA_SIGINFO;
    write_sa.sa_sigaction = write_handler;
    sigemptyset(&write_sa.sa_mask);
    sigaction(SIGRTMIN + 1, &write_sa, NULL);
    // Set up the timer
    struct sigevent write_sev {};
    write_sev.sigev_notify = SIGEV_SIGNAL;
    write_sev.sigev_signo = SIGRTMIN + 1;
    write_sev.sigev_value.sival_ptr = this;
    timer_create(CLOCK_REALTIME, &write_sev, &write_timerid);
    // Set the timer expiration time
    write_its.it_value.tv_sec = 0;
    write_its.it_value.tv_nsec = 0;
    // Set the timer interval to zero for a one-shot timer
    write_its.it_interval.tv_sec = 0;
    write_its.it_interval.tv_nsec = 0;
}

TestBulkTX::~TestBulkTX() {
    timer_delete(timeout_timerid);
    timer_delete(write_timerid);
}

void TestBulkTX::start_timeout() {
    std::chrono::microseconds us {10};
    timeout_its.it_value.tv_sec = 0;
    timeout_its.it_value.tv_nsec = 1000 * us.count();
    timer_settime(timeout_timerid, 0, &timeout_its, NULL);
}

void TestBulkTX::cancel_timeout() {
    timeout_its.it_value.tv_sec = 0;
    timeout_its.it_value.tv_nsec = 0;
    timer_settime(timeout_timerid, 0, &timeout_its, NULL);
}

void TestBulkTX::tx_start(const void *data, uint32_t size) {
    std::chrono::microseconds us {1};
    EXPECT_GT(size, 0);
    static_assert(sizeof(uint32_t) == 4);
    auto it = data_response.insert(data_response.end(), size / 4, uint32_t {0});
    std::memcpy(&*it, data, size);
    write_its.it_value.tv_sec = 0;
    write_its.it_value.tv_nsec = 1000 * us.count();
    timer_settime(write_timerid, 0, &write_its, NULL);
}

void run_experiment(TestBulkTX &midi) {
    using std::chrono::microseconds;
    size_t i = 0;
    size_t pi = 0;
    while (i < midi.data_challenge.size()) {
        if (i >= pi + 10'240) {
            std::printf("->%lu\n", i);
            pi += 10'240;
        }
        size_t size = (midi.data_challenge[i] % (PacketSize / 2));
        size = std::clamp<size_t>(size, 1, midi.data_challenge.size() - i);
        midi.write(midi.data_challenge.data() + i, size);
        std::this_thread::sleep_for(microseconds(midi.data_challenge[i] % 4));
        if (midi.data_challenge[i] % 8 == 0)
            midi.send_now();
        std::this_thread::sleep_for(microseconds(midi.data_challenge[i] % 4));
        i += size;
    }
    midi.send_now();
    std::printf("->%lu\n", i);
    std::this_thread::sleep_for(microseconds {100'000});
    EXPECT_TRUE(midi.is_done());
}

} // namespace

TEST(USB, BulkTX) {
    std::default_random_engine rng {12345};
    std::uniform_int_distribution<uint32_t> uniform;
    TestBulkTX tx;
    tx.data_challenge.resize(1024 * 1024);
    tx.data_response.reserve(tx.data_challenge.size());
    std::generate(tx.data_challenge.begin(), tx.data_challenge.end(),
                  [&] { return uniform(rng); });
    run_experiment(tx);
    EXPECT_EQ(tx.data_challenge, tx.data_response);
}
