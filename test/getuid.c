#include <unistd.h>
#include <stdio.h>

int main() {
	printf("Real user ID: %d\n", getuid());
	printf("Effective user ID: %d\n", geteuid());
	return 0;
}
