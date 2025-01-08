#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	printf("start\n");
/* 	bool parent = true;
	int childexec = 0;
	
	pid_t pid = fork();
	if(pid == 0){
		parent = false;
		childexec++;
	}
	
	printf("hello from %s\n", parent ? "parent" : "child");
	
	if(parent)
	{
		printf("child pid is %d\n", pid);
	}
	
	printf("childexec is %d from %s thread\n", childexec, parent ? "parent" : "child");
	*/
	int ret = execl("/bin/vi", "vi", NULL);
	printf("command completed with status %d\n", ret);
}