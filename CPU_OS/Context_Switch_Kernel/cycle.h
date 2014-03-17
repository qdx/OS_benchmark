#define CPU_FREQUENCY 2516582.4 //from /proc/cpuinfo
#define LOOP_RATIO 7.04 //from loop overhead

typedef long long unsigned uint64;

//be careful with optimization
#define tick_init() unsigned tick_eax, tick_edx; \
uint64 tick_overhead; \
uint64 tick_overhead_1, tick_overhead_2; \
tick(tick_overhead_1); \
tick(tick_overhead_1); \
tick(tick_overhead_1); \
tick(tick_overhead_2); \
tick_overhead = tick_overhead_2 - tick_overhead_1


#define tick(x) asm volatile("rdtsc" : "=a" (tick_eax), "=d" (tick_edx)); \
(x) = (((uint64)tick_eax) | (((uint64)tick_edx) << 32))

#define tick_diff(tick1, tick2) ((tick2) - (tick1) - (tick_overhead))
#define tick_time(tick1, tick2) (tick_diff((tick1), (tick2)) / CPU_FREQUENCY)
#define tick_diff_loop(tick1, tick2, loop_size) (tick_diff((tick1),(tick2)) - (uint64)(LOOP_RATIO * (loop_size)))
#define tick_time_loop(tick1, tick2, loop_size) (tick_diff_loop((tick1), (tick2), (loop_size)) / CPU_FREQUENCY)

#define print_stat(sum, sqSum, n) printf("avg=%.2f, std=%.2e, avg_time=%.2ems, std_time=%.2e\n", sum / n, sqrt(sqSum / n - (sum / n * sum / n)), sum / n / CPU_FREQUENCY, sqrt(sqSum / n - (sum / n * sum / n)) / CPU_FREQUENCY)

#define print_stat_csv(sum, sqSum, n) printf("%.2f,%.2e,%.2e,%.2e\n", sum / n, sqrt(sqSum / n - (sum / n * sum / n)), sum / n / CPU_FREQUENCY, sqrt(sqSum / n - (sum / n * sum / n)) / CPU_FREQUENCY)

inline uint64 tick_find_overhead()
{
unsigned tick_eax, tick_edx;
uint64 tick1, tick2;
tick(tick1);
tick(tick2);
return tick2 - tick1;
}
