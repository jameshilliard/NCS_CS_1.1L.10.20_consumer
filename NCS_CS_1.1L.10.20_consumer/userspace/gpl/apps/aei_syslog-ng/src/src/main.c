/***************************************************************************
 *
 * Copyright (c) 1999 Balázs Scheidler
 * Copyright (c) 1999-2001 BalaBit IT Ltd.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Inspired by nsyslog, originally written by Darren Reed.
 *
 * $Id: main.c,v 1.49.4.3 2006/03/13 23:34:13 bazsi Exp $
 *
 ***************************************************************************/

#include "syslog-ng.h"
#include "cfgfile.h"
#include "afinter.h"
#include "format.h"
#include "werror.h"
#include "queue.h"
#include "pkt_buffer.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>

#if HAVE_ARPA_NAMESER_H
#include <arpa/nameser.h>
#endif

#include <resolv.h>

#include "main.c.x"

static char cfgfilename[128] = PATH_SYSLOG_NG_CONF;
static char pidfilename[128] = PATH_PIDFILE;
static UINT32 gFileSizeLimit = 1024*16;
extern int linenum;

/* CLASS:
   (class
     (name reread_config)
     (super callback)
     (vars
       (backend object syslog_backend)
       (state simple int)))
*/

static void do_reread_config(struct callback *c)
{
	CAST(reread_config, self, c);

	switch (self->state) {
	case -1:
		self->backend->super.reloading = 1;
		self->state++;
		break;
	case 0:
	case 2:
		self->backend->persistent = make_persistent_config();
		CONFIG_DESTROY(self->backend->configuration, self->backend->persistent);
		self->state++;
		self->backend->oldconfig = self->backend->configuration;
		self->backend->configuration = self->backend->newconfig;
		io_callout(&self->backend->super, 0, c);
		break;
	case 1:
		if (CONFIG_INIT(self->backend->configuration, self->backend->persistent)) {
			self->backend->newconfig = NULL;
			self->backend->oldconfig = NULL;
			kill_persistent_config(self->backend->persistent);
			self->backend->persistent = NULL;
			self->backend->super.reloading = 0;
			if (!debug_flag) {
				set_internal_cfg(self->backend->configuration);
				set_error_internal();
			}
			notice("new configuration initialized\n");
		}
		else {
			/* initializing the new configuration was not 
			   successful, try to revert to the old one */
			self->state++;
			self->backend->newconfig = self->backend->oldconfig;
			self->backend->oldconfig = NULL;
			io_callout(&self->backend->super, 0, c);
		}
		break;
	case 3:
		if (CONFIG_INIT(self->backend->configuration, self->backend->persistent)) {
			kill_persistent_config(self->backend->persistent);
			self->backend->persistent = NULL;
			self->backend->super.reloading = 0;
			if (!debug_flag) {
				set_internal_cfg(self->backend->configuration);
				set_error_internal();
			}
			notice("Initializing new configuration failed, reverting to old config.\n");
		}
		else {
			/* this is a fatal error, but no stderr & no log output */
			exit(127);
		}
		break;
	}
}


/* CLASS:
   (class
     (name report_stats)
     (super callback)
     (vars
       (timeout simple int)
       (backend object syslog_backend)))
*/

static void do_report_stats(struct callback *c)
{
	CAST(report_stats, self, c);
	
	notice("STATS: dropped %i\n", pktbuf_dropped_pkts);
	pktbuf_dropped_pkts = 0;
	io_callout(&self->backend->super, self->timeout, &self->super);
}

static void do_flush_wakeup(struct callback *c)
{
	;
}

/* CLASS:
   (class
     (name syslog_backend)
     (super io_backend)
     (vars
       (newconfig object syslog_config)
       (oldconfig object syslog_config)
       (persistent object persistent_config)
       (configuration object syslog_config)))
*/

static int sighuprecvd = 0;

void sig_hup(int signo)
{
	sighuprecvd = 1;
	signal(SIGHUP, sig_hup);
}

static int sigtermrecvd = 0;

void sig_term(int signo)
{
	sigtermrecvd = 1;
	signal(SIGTERM, sig_term);
}

void sig_child(int signo)
{
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	signal(SIGCHLD, sig_child);
}

int main_loop(struct syslog_backend *backend)
{
	static struct report_stats s = 
		{ { STATIC_HEADER, do_report_stats }, 0, NULL };
	static struct reread_config r = 
		{ { STATIC_HEADER, do_reread_config }, NULL, 0 };

	int exit_main_loop = 0, reload_config = 0;

	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, sig_hup);
	signal(SIGTERM, sig_term);
	signal(SIGCHLD, sig_child);
	
	s.timeout = backend->configuration->stats_freq;
	s.backend = backend;
	r.backend = backend;
	if (s.timeout > 0)
		io_callout(&backend->super, s.timeout, &s.super);

	while (io_iter(&backend->super)) {
		
		if (exit_main_loop) {
			CONFIG_DESTROY(backend->configuration, 0);
			break;
		}
		if (reload_config && r.state == 0) {
		        /* reload initiated and callout started by SIGHUP finished, e.g. r.state == 0 */
			backend->newconfig = make_syslog_config(cfgfilename, &backend->super);
			if (backend->newconfig) {
				
				io_callout_flush(&backend->super);

				s.timeout = backend->configuration->stats_freq;
				if (s.timeout > 0)
					io_callout(&backend->super, s.timeout, &s.super);
				
				r.state = 0;
				io_callout(&backend->super, 0, &r.super);
			}
			else {
				backend->super.reloading = 0;
				notice("Syntax error reading config file (line %i), using old configuration\n", linenum);
			}
			reload_config = 0;
		}

		if (sigtermrecvd) {
			static struct callback wakeup = 
				{ STATIC_HEADER, do_flush_wakeup };
			
			notice("syslog-ng version %z going down\n", VERSION);
			exit_main_loop = 1;
			io_callout(&backend->super, 1, &wakeup);
		}
		if (sighuprecvd) {
			sighuprecvd = 0;
			if (!backend->super.reloading && !reload_config) {
#if HAVE_RES_INIT
				res_init();
#endif
				notice("SIGHUP received, restarting syslog-ng\n");
				if (!debug_flag) {
					set_error_stream(2, 0);
					set_internal_cfg(NULL);
				}
				reload_config = 1;
				r.backend = backend;
				r.state = -1;
				/* wait a second to flush buffers */
				io_callout(&backend->super, 1, &r.super);
			}
		}

		if (backend->configuration->time_sleep > 0)
		{
			struct timespec ts;

			ts.tv_sec = 0;
			ts.tv_nsec = backend->configuration->time_sleep * 1E6;

			nanosleep(&ts, NULL);
		}
	}
	if (!exit_main_loop) {
		notice("No input/output channels opened, exiting...\n");
		CONFIG_DESTROY(backend->configuration, 0);
	}
	return 0;
}

int go_background()
{
	pid_t pid;
	int wakeup_pipe[2];
	char buf[1];
	
	if (pipe(wakeup_pipe) < 0) {
		werror("Error in pipe(), (%z)\n", strerror(errno));
		return -1;
	}
	pid = fork();
	if (pid == 0) {
		int fd;
		
		close(wakeup_pipe[0]);
		fd = open(pidfilename, O_CREAT | O_WRONLY | O_NOCTTY | O_TRUNC, 0600);
		if (fd != -1) {
			struct ol_string *pid_s = c_format("%i\n", getpid());
			write(fd, pid_s->data, pid_s->length);
			ol_string_free(pid_s);
			close(fd);
		}
		return wakeup_pipe[1];
	}
	else if (pid == -1) {
		werror("Cannot fork(), (%z)\n", strerror(errno));
		exit(1);
	}
	close(wakeup_pipe[1]);
	if (read(wakeup_pipe[0], buf, sizeof(buf)) == 1 && buf[0] == 0) {
		exit(0);
	}
	else {
		exit(1);
	}
	exit(0);
}

void setup_fds(void)
{
	int nullfd;
	
	nullfd = open("/dev/null", O_RDWR);
	dup2(nullfd, 0);
	dup2(nullfd, 1);
	dup2(nullfd, 2);
	if (nullfd != 0 && nullfd != 1 && nullfd != 2)
		close(nullfd);
}

int resolve_user_group(char *arg, uid_t *uid, gid_t *gid)
{
	char *user, *group;
	struct passwd *pw;
	struct group *gr;
	
	*uid = 0;
	user = strtok(arg, ":.");
	group = strtok(NULL, "");
	if (user) {
		pw = getpwnam(user);
		if (pw) {
			*uid = pw->pw_uid;
		}
		else {
			*uid = atoi(user);
		}
		if (*uid == 0)
			return 0;
	}
	else {
		return 0;
	}
	if (group && gid) {
		*gid = 0;
		gr = getgrnam(group);
		if (gr) {
			*gid = gr->gr_gid;
		}
		else {
			*gid = atoi(group); 
		}
		if (*gid == 0)
			return 0;
	}
	return 1;
}

int resolve_group(char *group, gid_t *gid)
{
	struct group *gr;

	*gid = 0;
	gr = getgrnam(group);
	if (gr) {
		*gid = gr->gr_gid;
	}
	else {
		*gid = atoi(group); 
	}
	if (*gid == 0)
		return 0;
	return 1;
}

void usage(void)
{
	werror("Usage: syslog-ng [options]\n"
	       "Accept and manage system log messages\n\n"
	       "Options:\n"
	       "  -s, --syntax-only                Only read and parse config file\n"
	       "  -d, --debug                      Turn on debugging messages\n"
	       "  -v, --verbose                    Be a bit more verbose\n"
	       "  -F, --foreground                 Don't fork into background\n"
	       "  -f <fname>, --cfgfile=<fname>    Set config file name, default=" PATH_SYSLOG_NG_CONF "\n"
	       "  -V, --version                    Display version number (" PACKAGE " " VERSION ")\n"
	       "  -p <fname>, --pidfile=<fname>    Set pid file name, default=" PATH_PIDFILE "\n"
               "  -C <dir>, --chroot=<dir>         Chroot to directory\n"
               "  -u <user>, --user=<user>         Switch to user\n"
               "  -g <group>, --group=<group>      Switch to group\n"
#ifdef YYDEBUG
	       "  -y, --yydebug                    Turn on yacc debug messages\n"
#endif
		);

	exit(0);
}

#ifdef YYDEBUG
extern int yydebug;
#endif

int main(int argc, char **argv)
{
	int do_fork = 1;
	int syntax_only = 0;
	int opt;
	int wakeup_fd = -1;
	char *chroot_dir = NULL;
	uid_t uid = 0;
	gid_t gid = 0;

	NEW(syslog_backend, backend);
	struct option syslog_ng_options[] = {
		{ "cfgfile", required_argument, NULL, 'f' },
		{ "pidfile", required_argument, NULL, 'p' },
		{ "debug", no_argument, NULL, 'd' },
		{ "syntax-only", no_argument, NULL, 's' },
		{ "verbose", no_argument, NULL, 'v' },
		{ "foreground", no_argument, NULL, 'F' },
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
                { "chroot", required_argument, NULL, 'C' },
                { "user", required_argument, NULL, 'u' },
                { "group", required_argument, NULL, 'g' },
                { "file-size-limit", required_argument, NULL, 'S' },
#ifdef YYDEBUG
		{ "yydebug", no_argument, NULL, 'y' },
#endif
		{ NULL, 0, NULL, 0 }
	};
#ifdef HAVE_MALLOPT
        mallopt(M_TOP_PAD, 0x10000);
#endif
	gc_idle_threshold = 100;
	gc_busy_threshold = 3000;
	
	tzset();

	while ((opt = getopt_long(argc, argv, "sFf:p:dvhyVC:u:g:S:", syslog_ng_options, NULL)) != -1) {
		switch (opt) {
		case 'f':
			strncpy(cfgfilename, optarg, sizeof(cfgfilename));
			break;
		case 'p':
			strncpy(pidfilename, optarg, sizeof(pidfilename));
			break;
		case 's':
			syntax_only = 1;
			break;
		case 'd':
			debug_flag++;
			break;
		case 'v':
			verbose_flag++;
			break;
		case 'F':
			do_fork = 0;
			break;
		case 'V':
			werror(PACKAGE " " VERSION "\n");
			return 0;
		case 'C':
			chroot_dir = optarg;
			break;
		case 'u':
			if (!resolve_user_group(optarg, &uid, gid ? NULL : &gid))
				usage();
			break;
		case 'g':
			if (!resolve_group(optarg, &gid))
				usage();
			break;
		case 'S':
			gFileSizeLimit = atoi(optarg);
			break;
#ifdef YYDEBUG
		case 'y':
			yydebug = 1;
			break;
#endif
		case 'h':
		default:
			usage();
			break;
		}
	}

	init_backend(&backend->super);
	
	backend->configuration = make_syslog_config(cfgfilename, &backend->super);
	if (!backend->configuration) {
		werror("Parse error reading configuration file, exiting. (line %i)\n", linenum);
		return 1;
	}
	backend->configuration->file_size_limit=gFileSizeLimit;
	fprintf(stderr, "%s:%s:%d ==after read config,backend->configuration->file_size_limit=%d==\n"
		,__FUNCTION__,__FILE__,__LINE__,backend->configuration->file_size_limit);
	if (syntax_only) {
		return 0;
	}
	if (debug_flag) {
		do_fork = 0;
	}
	if (do_fork) {
		wakeup_fd = go_background();
	}
	if (!CONFIG_INIT(backend->configuration, NULL)) {
		char res = 1;
		
		werror("Error initializing configuration, exiting.\n");
		if (wakeup_fd != -1) {
			write(wakeup_fd, &res, 1);
			close(wakeup_fd);
		}
		return 2;
	}
	if (chroot_dir) {
		if (chdir(chroot_dir) < 0) {
			werror("Error chdiring, exiting.\n");
			return 3;
		}
		if (chroot(".") < 0) {
			werror("Error chrooting, exiting.\n");
			return 3;
		}
	}
	
	if (uid || gid) {
		if (gid && setgid(gid) < 0) {
			werror("Error in setgid(), exiting.\n");
			return 4;
		}
		if (setgroups(0, NULL) < 0) {
			werror("Error in setgroups(), exiting.\n");
			return 4;
		}
		if (uid && setuid(uid) < 0) {
			werror("Error setuid(), exiting.\n");
			return 4;
		}
	}

	if (!debug_flag) {
		set_internal_cfg(backend->configuration);
		set_error_internal();
	}
	else {
		set_error_stream(2, 0);
	}

	if (do_fork) {
		setup_fds();
		setsid();
	}
	if (wakeup_fd != -1) {
		char res = 0;

		write(wakeup_fd, &res, 1);
		close(wakeup_fd);
	}
	notice("syslog-ng version %z starting\n", VERSION);
	return main_loop(backend);
}
