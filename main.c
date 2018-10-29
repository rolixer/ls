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

void getDate(const time_t time, char *date){
	struct tm *_date;
	_date = localtime(&time);
	
	strftime(date, 255, "%b %d %R", _date);
	
};

void list_dir(char *name) {
	DIR *dir;
	struct dirent *dp;
	struct stat statbuf;
	char date[255];

	if ((dir = opendir(name)) == NULL) {
		perror("Blad");
	}
	while ((dp = readdir(dir)) != NULL) {
		if (stat(dp->d_name, &statbuf) == -1)
			continue;
		getDate(statbuf.st_mtime, date);
		printf("%o %lu %s %s %ld %s %s\n", statbuf.st_mode,
					statbuf.st_nlink,
					getUname(statbuf.st_uid),
				    	getGname(statbuf.st_gid),
					statbuf.st_size,
					date,
		 		    	dp->d_name);
	}
	closedir(dir);
}

int main(int argc, char **argv) {
	list_dir(".");
	exit(0);
}
