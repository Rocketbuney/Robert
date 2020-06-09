typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned int size_t;
typedef int ssize_t;

ssize_t write(int fd, const void *buf, size_t count);

int main(int argc, char** argv) {
	write(1, "hello world\n", 12);
	return 0;
}
