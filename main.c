#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <unistd.h>

#include <pwd.h>
#include <grp.h>

#include <time.h>

char *getUname(int uid){
	struct passwd *user;
	user = getpwuid(uid);

	return user->pw_name;
};

char *getGname(int gid){
	struct group *grp;
	grp = getgrgid(gid);

	return grp->gr_name;
};

char *getDate(time_t time){
	struct tm *date_time;
	date_time = gmtime(time);
	
	char *buff;
	sprintf(buff, 

	return buff;	
};

void list_dir(char *name) {
	DIR *dir;
	struct dirent *dp;
	struct stat statbuf;

	if ((dir = opendir(name)) == NULL) {
		perror("Blad");
	}
	while ((dp = readdir(dir)) != NULL) {
		if (stat(dp->d_name, &statbuf) == -1)
			continue;
		printf("%o %d %s %s %s  %d %s\n", statbuf.st_mode,
					statbuf.st_nlink,
					getUname(statbuf.st_uid),
				    	getGname(statbuf.st_gid),
					ctime(statbuf.st_atime),
					statbuf.st_size,
		 		    	dp->d_name);
	}
	closedir(dir);
}

int main(int argc, char **argv) {
	list_dir(".");
	exit(0);
}
