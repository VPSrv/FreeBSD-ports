
/*
 *   check_reload_status.c
 *   part of the pfSense project
 *   (C)2005 Scott Ullrich
 *   All rights reserved
 *
 *   This file monitors for certain files to
 *   appear in /tmp and then takes action on them.
 *   It's a mini-daemon of sorts to kick off filter
 *   reloads, sshd starting, etc.   It may be expanded
 *   down to the road to kick off any type of tasks that
 *   take up too much time from the GUI perspective.
 *   
 */

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define LOGFILE		"/var/log/check_reload_status"
#define TMPDIR		"/tmp"

/* Default cycle time value 1 minute */
#define CYCLE		5

static char _sccsid[] = { " $Id$ " };

/* Check if file exists */
int fexist(char * filename)
{
  struct stat buf;

  if (( stat (filename, &buf)) < 0)
    return (0);

  if (! S_ISREG(buf.st_mode)) {
    return (0);
  }

  return(1);
}

void kill_ghost_shells(void) {
  system("/bin/sh /usr/local/bin/kill_ghosts.sh");
  system("/bin/sh /usr/local/bin/kill_ghosts.sh");
  system("/bin/sh /usr/local/bin/kill_ghosts.sh");
}

int main(void) {
	char argument[255];
	char temp[255];
	int cycle_time;
	FILE *f;
	cycle_time = CYCLE;
	/* daemonize */
	system("echo starting > /tmp/check_reload_status");
	if( fork() == 0 ) {
	  /* close stdin and stderr */
	  fclose( stdin );
	  fclose( stdout );
	  /* loop forever until the cows come home */
	  while(1) {
	      if(fexist("/tmp/restart_webgui") == 1) {
			  system("echo /tmp/restartwebgui > /tmp/check_reload_status");
		      system("/bin/rm /tmp/restart_webgui");
		      system("/usr/bin/nice -n20 /etc/rc.restart_webgui");
	      }
	      if(fexist("/tmp/rc.linkup") == 1) {
			  system("echo /tmp/rc.linkup > /tmp/check_reload_status");
		      char buf[FILENAME_MAX + 2];
		      if (!(f = fopen("/tmp/rc.linkup", "r"))) {
			      fprintf(stderr, "Could not open /tmp/rc.linkup for input.\n");
		      } else {
			  while (fgets(buf, sizeof buf, f))
			      fputs(buf, stdout);
				  fclose(f);
		      }
		      system("/bin/rm /tmp/rc.linkup");
		      sprintf(temp, "/usr/local/bin/php /etc/rc.linkup %s", buf);
		      system(temp);
	      }		  
	      if(fexist("/tmp/rc.newwanip") == 1) {
			  system("echo /tmp/rc.newwanip > /tmp/check_reload_status");
		      char buf[FILENAME_MAX + 2];
		      if (!(f = fopen("/tmp/rc.newwanip", "r"))) {
			      fprintf(stderr, "Could not open /tmp/rc.newwanip for input.\n");
		      } else {
			  while (fgets(buf, sizeof buf, f))
			      fputs(buf, stdout);
			  fclose(f);
		      }
		      system("/bin/rm /tmp/rc.newwanip");
		      sprintf(temp, "/usr/local/bin/php /etc/rc.newwanip %s", buf);
		      system(temp);
	      }	  
	      if(fexist("/tmp/filter_dirty") == 1) {
		      system("/bin/rm -f /tmp/filter_dirty");
		      system("/usr/bin/nice -n20 /usr/local/bin/php /etc/rc.filter_configure >/dev/null");
	      }
	      if(fexist("/tmp/reload_all") == 1) {
			  system("echo /tmp/reload_all > /tmp/check_reload_status");
		      system("/bin/rm /tmp/reload_all");
		      system("/usr/bin/nice -n20 /usr/local/bin/php /etc/rc.reload_all >/dev/null");
			  kill_ghost_shells();
			  
	      }
	      if(fexist("/tmp/reload_interfaces") == 1) {
			  system("echo /tmp/reload_interfaces > /tmp/check_reload_status");
		      system("/bin/rm /tmp/reload_interfaces");
		      system("/usr/bin/nice -n20 /usr/local/bin/php /etc/rc.reload_interfaces >/dev/null");
			  kill_ghost_shells();
			  
	      }
	      if(fexist("/tmp/update_dyndns") == 1) {
			  system("echo /tmp/update_dyndns > /tmp/check_reload_status");
		      system("/bin/rm /tmp/update_dyndns");
		      system("/usr/bin/nice -n20 /usr/local/bin/php /etc/rc.dyndns.update");
	      }
	      if(fexist("/tmp/interfaces_wan_configure") == 1) {
			  system("echo /tmp/interfaces_wan_configure > /tmp/check_reload_status");
		      system("/bin/rm  /tmp/interfaces_wan_configure");
		      system("/usr/bin/nice -n20 /usr/local/bin/php /etc/interfaces_wan_configure");
			  kill_ghost_shells();
	      }
	      if(fexist("/tmp/interfaces_opt_configure") == 1) {
			  system("echo /tmp/interfaces_opt_configure > /tmp/check_reload_status");
		      system("/bin/rm /tmp/interfaces_opt_configure");
		      system("/usr/bin/nice -n20 /usr/local/bin/php /etc/interfaces_opt_configure");
			  kill_ghost_shells();
			  
	      }
	      if(fexist("/tmp/start_sshd") == 1) {
			  system("echo /tmp/start_sshd > /tmp/check_reload_status");
		      system("/bin/rm /tmp/start_sshd");
		      system("/usr/bin/nice -n20 /etc/sshd");
	      }
		  system("echo sleeping > /tmp/check_reload_status");
	      sleep( cycle_time );
	  }
	  system("echo exiting > /tmp/check_reload_status");
	} else {
	      /* Exit parent process */
	      if( signal( SIGINT, SIG_DFL ) != SIG_DFL )
		      signal( SIGINT, SIG_DFL );
	      if( signal( SIGKILL, SIG_DFL ) != SIG_DFL )
		      signal( SIGKILL, SIG_DFL );
	      exit( 0 );	  
	}
	exit( 0 );
	return( 0 );
}


