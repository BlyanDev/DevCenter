#include <signal.h>
#include <assert.h>

#define DUMP_BUF_SIZE 1024
#define DUMP_FILE_PATH "dump.txt"

static void *s_dump_buffer[DUMP_BUF_SIZE];
static const char *s_seg_start = ".....segment fault.....\n";
static const char *s_assert_start = ".....assert.....\n";
static const char *s_end = ".....end.....\n";

void do_assert(const char* file, int line)
{
	printf("assert! file:%s,line:%d,error no: %d\n", file, line, errno);

	int num;
	char **calls;
	num = backtrace(s_dump_buffer, DUMP_BUF_SIZE);
	calls = backtrace_symbols(s_dump_buffer, num);
	for (int i = 0; i < num; i++)
	{
		printf("%s\n", calls[i]);
	}

	char string[256];
	memset(string, 0, sizeof(string));

	FILE *fp = fopen(DUMP_FILE_PATH, "ab+");
	fwrite(s_assert_start, sizeof(unsigned char), strlen(s_assert_start), fp);

	sprintf(string, "file:%s,line:%d,error no: %d\n", file, line, errno);
	fwrite(string, sizeof(unsigned char), strlen(string), fp);

	for (int i = 0; i < num; i++)
	{
		fwrite(calls[i], sizeof(unsigned char), strlen(calls[i]), fp);
		fwrite("\n", sizeof(unsigned char), 1, fp);
	}
	fwrite(s_end, sizeof(unsigned char), strlen(s_end), fp);
	assert(0);
	_exit(-1);
}

static void write_dump_file(char **calls, int num)
{
	FILE *fp;
	fp = fopen(DUMP_FILE_PATH, "ab+");

	fwrite(s_seg_start, sizeof(unsigned char), strlen(s_seg_start), fp);
	if (fp != NULL)
	{
		for (int i = 0; i < num; i++)
		{
			fwrite(calls[i], sizeof(unsigned char), strlen(calls[i]), fp);
			fwrite("\n", sizeof(unsigned char), 1, fp);
		}
		fwrite(s_end, sizeof(unsigned char), strlen(s_end), fp);
		fclose(fp);
	}
}

static void dump_segment_fault(int n, siginfo_t *siginfo, void *myact)
{
	printf("segment fault! addr:0x%X\n", siginfo->si_addr);

	int num;
	char **calls;
	num = backtrace(s_dump_buffer, DUMP_BUF_SIZE);
	calls = backtrace_symbols(s_dump_buffer, num);
	for (int i = 0; i < num; i++)
	{
		printf("%s\n", calls[i]);
	}
	write_dump_file(calls, num);
	_exit(-1);
}

static void init_dump()
{
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = dump_segment_fault;
	sigaction(SIGSEGV, &act, NULL);
	signal(SIGCLD, SIG_IGN);
}