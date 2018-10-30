#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <dirent.h>
#include <unistd.h>

#include <grp.h>
#include <pwd.h>

#include <time.h>

char *getUname(int uid) {
  struct passwd *user;
  user = getpwuid(uid);

  return user->pw_name;
};

char *getGname(int gid) {
  struct group *grp;
  grp = getgrgid(gid);

  return grp->gr_name;
};

void getDate(const time_t time, char *date) {
  struct tm *_date;
  _date = localtime(&time);

  strftime(date, 255, "%b %d %R", _date);
};

void getRights(int inode, char *rights) {
  static char *RIGHTS[8] = {"---", "--x", "-w-", "-wx",
                            "r--", "r-x", "rw-", "rwx"};

  static char FILE_TYPE[7] = {'-', 'd', 'c', 'b', 's', 'p', 'l'};

  int mask[4];
  int i;
  for (i = 3; i > 0; i--) {
    mask[i] = inode % 010;
    inode /= 010;
  }

  switch (inode) {
    case 0140:
      mask[0] = 4;
      break;
    case 0120:
      mask[0] = 6;
      break;
    case 0100:
      mask[0] = 0;
      break;
    case 060:
      mask[0] = 3;
      break;
    case 040:
      mask[0] = 1;
      break;
    case 020:
      mask[0] = 2;
      break;
    case 010:
      mask[0] = 5;
      break;
    default:
      mask[0] = 0;
      break;
  }

  sprintf(rights, "%c%s%s%s", FILE_TYPE[mask[0]], RIGHTS[mask[1]],
          RIGHTS[mask[2]], RIGHTS[mask[3]]);
};

void list_dir(char *name) {
  DIR *dir;
  struct dirent *dp;
  struct stat statbuf;
  char date[255];
  char rights[10] = {};

  printf("%s\n", name);

  if ((dir = opendir(name)) == NULL) {
    perror("Blad");
  }
  while ((dp = readdir(dir)) != NULL) {
    if (stat(dp->d_name, &statbuf) == -1) continue;
	if (dp->d_name[0] == '.') continue;
    getDate(statbuf.st_mtime, date);
    getRights(statbuf.st_mode, rights);
    printf("%s %lu %s %s %ld %s %s\n",  rights,
               statbuf.st_nlink,
           getUname(statbuf.st_uid), getGname(statbuf.st_gid), statbuf.st_size,
           date, dp->d_name);
  }
  closedir(dir);
}

int main(int argc, char **argv) {
  
  chdir(*(++argv));
  list_dir(".");
  exit(0);
}
